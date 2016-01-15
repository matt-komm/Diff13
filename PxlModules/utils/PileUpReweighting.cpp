#include "pxl/hep.hh"
#include "pxl/core.hh"
#include "pxl/core/macros.hh"
#include "pxl/core/PluginManager.hh"
#include "pxl/modules/Module.hh"
#include "pxl/modules/ModuleFactory.hh"

#include <string>
#include <iostream>
#include <cstdlib>

#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"

static pxl::Logger logger("PileUpReweighting");


class PileUpReweighting:
    public pxl::Module
{
    private:
        pxl::Source* _outputSource;
        
        std::string _mcFile;
        std::string _mcHistName;
        
        std::vector<std::string> _dataFiles;
        std::string _dataHistName;
        
        std::vector<std::pair<std::string,TH1F>> _reweightingHists;

    public:
    
        PileUpReweighting():
            Module()
        {
            addSink("input", "input");
            _outputSource = addSource("output","output");
            
            addOption("mc histogram file","file containing the number of PU interactions in MC",_mcFile,pxl::OptionDescription::USAGE_FILE_OPEN);
            addOption("mc histogram name","",_mcHistName);
            
            addOption("data histogram files","files containing the number of PU interactions in data",_dataFiles,pxl::OptionDescription::USAGE_FILE_OPEN);
            addOption("data histogram name","",_dataHistName);

        }

        ~PileUpReweighting()
        {
        }

        // every Module needs a unique type
        static const std::string &getStaticType()
        {
            static std::string type ("PileUpReweighting");
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
            getOption("mc histogram file",_mcFile);
            getOption("mc histogram name",_mcHistName);
            
            getOption("data histogram files",_dataFiles);
            getOption("data histogram name",_dataHistName);
            
            
            TFile mcFile(_mcFile.c_str());
            TH1* histMC = dynamic_cast<TH1*>(mcFile.Get(_mcHistName.c_str()));
            if (not histMC)
            {
                throw std::runtime_error(getName()+": Failed to retrieve MC PU histogram from file '"+_mcFile+"' with name '"+_mcHistName+"'");
            }
            histMC->SetDirectory(0);
            histMC->Scale(1.0/histMC->Integral());
            
            for (unsigned int idataFile = 0; idataFile < _dataFiles.size(); ++idataFile)
            {
                const int pos = _dataFiles[idataFile].find_last_of('/')+1;
                const int end = _dataFiles[idataFile].find_first_of('.',pos+1);
                std::string puName = std::string(_dataFiles[idataFile],pos,end-pos);
                _reweightingHists.push_back(std::make_pair(puName,TH1F((std::string("reweightingHist")+std::to_string(idataFile)+std::to_string(std::rand())).c_str(),";N true interactions; weight",52,0,52)));
                
                TH1F& weightHist = _reweightingHists.back().second;
                weightHist.SetDirectory(0);
                
                TFile dataFile(_dataFiles[idataFile].c_str());
                TH1* histData = dynamic_cast<TH1*>(dataFile.Get(_dataHistName.c_str()));
                if (not histData)
                {
                    throw std::runtime_error(getName()+": Failed to retrieve data PU histogram from file '"+_dataFiles[idataFile]+"' with name '"+_dataHistName+"'");
                }
                histData->SetDirectory(0);
                histData->Scale(1.0/histData->Integral());
                
                for (unsigned int ibin = 0; ibin < std::min({histData->GetNbinsX(),histMC->GetNbinsX(),weightHist.GetNbinsX()}); ++ibin)
                {
                    const double binCenter = weightHist.GetBinCenter(ibin+1);
                    
                    const double puMC = histMC->GetBinContent(histMC->FindBin(binCenter));
                    const double puData = histData->GetBinContent(histData->FindBin(binCenter));


                    //cut off events with too less statistics <0.01% or unphysical weights >10
                    if (puMC>0.0001 and (puData/puMC)<10)
                    {
                        weightHist.SetBinContent(ibin+1,puData/puMC);
                    }
                }
                
                dataFile.Close();
            }
            
            mcFile.Close();
            
            /*
            for (unsigned int ihist = 0; ihist < _reweightingHists.size(); ++ihist)
            {
                TCanvas cv("cv","",800,600);
                _reweightingHists[ihist].second.Draw();
                cv.Print((_reweightingHists[ihist].first+".pdf").c_str());
            }
            */
            
        }

        bool analyse(pxl::Sink *sink) throw (std::runtime_error)
        {
            try
            {
                pxl::Event *event  = dynamic_cast<pxl::Event*>(sink->get());
                if (event)
                {
                    const pxl::BasicNVector* trueInteractions = nullptr;
                    pxl::EventView* recoEventView = nullptr;
                
                    std::vector<pxl::EventView*> eventViews;
                    event->getObjectsOfType(eventViews);
                    for (unsigned ieventView=0; ieventView<eventViews.size();++ieventView)
                    {
                        if (eventViews[ieventView]->getName()=="Reconstructed")
                        {
                            recoEventView=eventViews[ieventView];
                            std::vector<pxl::Particle*> particles;
                            eventViews[ieventView]->getObjectsOfType(particles);

                            for (unsigned int iparticle = 0; iparticle<particles.size(); ++iparticle)
                            {
                                pxl::Particle* particle = particles[iparticle];
                                if (particle->getName()=="PU")
                                {
                                    trueInteractions = dynamic_cast<const pxl::BasicNVector*>(&particle->getUserRecord("nTrueInteractions0").asSerializable());
                                }
                            }
                        }
                    }
                    if (trueInteractions)
                    {
                        for (unsigned int ihist = 0; ihist < _reweightingHists.size(); ++ihist)
                        {
                            const double ntrueInteractionsAtBX0 = trueInteractions->getElement(1);
                            if (ntrueInteractionsAtBX0<_reweightingHists[ihist].second.GetNbinsX())
                            {
                                const int bin = _reweightingHists[ihist].second.FindBin(ntrueInteractionsAtBX0);
                                const float weight = _reweightingHists[ihist].second.GetBinContent(bin);
                                recoEventView->setUserRecord(_reweightingHists[ihist].first+"_weight",weight);
                            }
                            else
                            {
                                recoEventView->setUserRecord(_reweightingHists[ihist].first+"_weight",1.0);
                            }
                        }
                    }
                    else
                    {
                        throw std::runtime_error("Number of true interactions not found in event!");
                    }

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

        void shutdown() throw(std::runtime_error)
        {
        }

        void destroy() throw (std::runtime_error)
        {
            delete this;
        }
};

PXL_MODULE_INIT(PileUpReweighting)
PXL_PLUGIN_INIT
