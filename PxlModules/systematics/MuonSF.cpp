#include "pxl/hep.hh"
#include "pxl/core.hh"
#include "pxl/core/macros.hh"
#include "pxl/core/PluginManager.hh"
#include "pxl/modules/Module.hh"
#include "pxl/modules/ModuleFactory.hh"

#include <string>
#include <vector>
#include <stdexcept>
#include <cstdlib>

#include "TH2.h"
#include "TFile.h"

static pxl::Logger logger("MuonSF");

class MuonSF:
    public pxl::Module
{
    private:
        struct ScaleFactor
        {
            std::string prefix;
            
            std::string fileName;
            std::string histogramName;
            double additionalUncertainty;
            
            TH2* rootHistogramPtEta;
            
            ScaleFactor(
                pxl::Module* module,
                const std::string& prefix
            ):
                prefix(prefix),
                fileName(),
                histogramName(),
                additionalUncertainty(0),
                rootHistogramPtEta(nullptr)
            {
                module->addOption(prefix+" file name","","",pxl::OptionDescription::USAGE_FILE_OPEN);
                module->addOption(prefix+" hist name","","");
                module->addOption(prefix+" add uncertainty", "", 0.0);
            }
            
            void init(pxl::Module* module)
            {
                module->getOption(prefix+" file name",fileName);
                module->getOption(prefix+" hist name",histogramName);
                module->getOption(prefix+" add uncertainty", additionalUncertainty);
            
                TFile rootFile(fileName.c_str());
                TH2* h = dynamic_cast<TH2*>(rootFile.Get(histogramName.c_str()));
                if (!h)
                {
                    throw std::runtime_error("Histogram '"+histogramName+"' does not exist in file '"+fileName+"'");
                }
                if (h->GetXaxis()->GetXmax()>50.0 and h->GetYaxis()->GetXmax()<5.0)
                {
                    rootHistogramPtEta = (TH2*)h->Clone((std::string("histMuonSF")+std::to_string(std::rand())).c_str());
                    rootHistogramPtEta->SetDirectory(0);
                }
                else
                {
                    throw std::runtime_error("Histogram '"+histogramName+"' in file '"+fileName+"' does not seem to be binned x=pt and y=eta");
                }
            }
            
            float getScaleFactor(double pt, double eta,int shift=0)
            {
                if (rootHistogramPtEta)
                {
                    int xbin = rootHistogramPtEta->GetXaxis()->FindBin(pt);
                    int ybin = rootHistogramPtEta->GetYaxis()->FindBin(fabs(eta));
                    if (xbin<=0)
                    {
                        throw std::runtime_error("Got muon with pt="+std::to_string(pt)+" that underflows the SF histogram '"+histogramName+"'");
                    }
                    else if (xbin>rootHistogramPtEta->GetXaxis()->GetNbins())
                    {
                        xbin=rootHistogramPtEta->GetXaxis()->GetNbins();
                    }
                    if (ybin<=0)
                    {
                        throw std::runtime_error("Got muon with eta="+std::to_string(eta)+" that underflows the SF histogram '"+histogramName+"'");
                    }
                    else if (ybin>rootHistogramPtEta->GetYaxis()->GetNbins())
                    {
                        ybin=rootHistogramPtEta->GetYaxis()->GetNbins();
                    }
                    float sf = rootHistogramPtEta->GetBinContent(xbin,ybin);
                    if (sf<0.1)
                    {
                        throw std::runtime_error("Got muon SF of "+std::to_string(sf)+"(<0.1) for pt="+std::to_string(pt)+" eta="+std::to_string(eta)+" from histogram '"+histogramName+"'");
                    }
                    if (shift>=1)
                    {
                        sf += fabs(rootHistogramPtEta->GetBinError(xbin,ybin))+fabs(additionalUncertainty);
                    }
                    else if (shift<=-1)
                    {
                        sf -= fabs(rootHistogramPtEta->GetBinError(xbin,ybin))+fabs(additionalUncertainty);
                    }
                    return sf;
                }
                else
                {
                    throw std::runtime_error("Histogram '"+histogramName+"' not yet read from file or it has been deleted");
                }
            }
        };
    
        std::string _eventViewName;
        std::string _muonName;
        
        pxl::Source* _outputSource;
        
        
        ScaleFactor* _triggerSF;
        ScaleFactor* _isoSF;
        ScaleFactor* _idSF;
        
    public:
        MuonSF():
            Module(),
            _eventViewName("Reconstructed"),
            _muonName("TightMuon")
        {
            addSink("input", "input");
            addOption("event view name", "", _eventViewName);
            addOption("muon name", "", _muonName);
            
            _outputSource = addSource("output","output");
            
            _triggerSF=new ScaleFactor(this,"trigger");
            _isoSF=new ScaleFactor(this,"iso");
            _idSF=new ScaleFactor(this,"ID");
        }

        ~MuonSF()
        {
        }

        // every Module needs a unique type
        static const std::string &getStaticType()
        {
            static std::string type ("MuonSF");
            return type;
        }

        // static and dynamic methods are needed
        const std::string &getType() const
        {
            return getStaticType();
        }

        bool isRunnable() const
        {
            // this module does not provide events, so return false
            return false;
        }

        void initialize() throw (std::runtime_error)
        {
        }

        void beginJob() throw (std::runtime_error)
        {
            getOption("event view name", _eventViewName);
            getOption("muon name", _muonName);


        }
        
        bool analyse(pxl::Sink *sink) throw (std::runtime_error)
        {
            
            try
            {
                pxl::Event *event  = dynamic_cast<pxl::Event*>(sink->get());
                if (event)
                {
                    std::vector<pxl::EventView*> eventViews;
                    event->getObjectsOfType(eventViews);
                    
                    pxl::Particle* muon = nullptr;
                    for (pxl::EventView* eventView: eventViews)
                    {
                        
                        if (eventView->getName()==_eventViewName)
                        {
                            std::vector<pxl::Particle*> particles;
                            eventView->getObjectsOfType(particles);
                            for (pxl::Particle* particle: particles)
                            {
                                if (particle->getName()==_muonName)
                                {
                                    if (muon==nullptr)
                                    {
                                        muon=particle;
                                    }
                                    else
                                    {
                                        throw std::runtime_error("Found multiple muons with name '"+_muonName+"' in event");
                                    }
                                }
                            }
                        }
                    }
                    if (!muon)
                    {
                        throw std::runtime_error("Found no muon with name '"+_muonName+"' in event");
                    }
                    
                    muon->setUserRecord("trigger_SF_nominal",_triggerSF->getScaleFactor(muon->getPt(),muon->getEta(),0));
                    muon->setUserRecord("trigger_SF_up",_triggerSF->getScaleFactor(muon->getPt(),muon->getEta(),1));
                    muon->setUserRecord("trigger_SF_down",_triggerSF->getScaleFactor(muon->getPt(),muon->getEta(),-1));
                    
                    muon->setUserRecord("iso_SF_nominal",_isoSF->getScaleFactor(muon->getPt(),muon->getEta(),0));
                    muon->setUserRecord("iso_SF_up",_isoSF->getScaleFactor(muon->getPt(),muon->getEta(),1));
                    muon->setUserRecord("iso_SF_down",_isoSF->getScaleFactor(muon->getPt(),muon->getEta(),-1));
                    
                    muon->setUserRecord("id_SF_nominal",_idSF->getScaleFactor(muon->getPt(),muon->getEta(),0));
                    muon->setUserRecord("id_SF_up",_idSF->getScaleFactor(muon->getPt(),muon->getEta(),1));
                    muon->setUserRecord("id_SF_down",_idSF->getScaleFactor(muon->getPt(),muon->getEta(),-1));
      
                    _outputSource->setTargets(event);
                    return _outputSource->processTargets();
                }
            }
            catch(std::exception &e)
            {
                throw std::runtime_error(getName()+": "+e.what());
            }
            catch(...)
            {
                throw std::runtime_error(getName()+": unknown exception");
            }

            logger(pxl::LOG_LEVEL_ERROR , "Analysed event is not an pxl::Event !");
            return false;
        }
        
        void endJob() throw (std::runtime_error)
        {
        }

        void shutdown() throw(std::runtime_error)
        {
        }

        void destroy() throw (std::runtime_error)
        {
            delete this;
        }
};


PXL_MODULE_INIT(MuonSF)
PXL_PLUGIN_INIT
