#include "pxl/hep.hh"
#include "pxl/core.hh"
#include "pxl/core/macros.hh"
#include "pxl/core/PluginManager.hh"
#include "pxl/modules/Module.hh"
#include "pxl/modules/ModuleFactory.hh"

#include "BTagWeightCalculator.hpp"
#include "BTagCalibrationStandalone.hpp"

#include <string>
#include <unordered_map>
#include <memory>

#include "TFile.h"
#include "TH2F.h"


static pxl::Logger logger("BTagReweighting");

class BTagReweighting:
    public pxl::Module
{
    private:
        pxl::Source* _outputSource;
        
        
        BTagCalibration _btagCalib;
        BTagCalibrationReader _readerNominal_mujets;
        BTagCalibrationReader _readerUp_mujets;
        BTagCalibrationReader _readerDown_mujets;
        
        BTagCalibrationReader _readerNominal_comb;
        BTagCalibrationReader _readerUp_comb;
        BTagCalibrationReader _readerDown_comb;
        
        std::shared_ptr<TH2F> mcEff_b;
        std::shared_ptr<TH2F> mcEff_c;
        std::shared_ptr<TH2F> mcEff_q;
        std::shared_ptr<TH2F> mcEff_other;
            
        BWGHT::BTagWeightCalculator _btagWeightCalc;
        
        constexpr static float MaxBJetPt = 670;
        constexpr static float MaxLJetPt = 1000;
        
        std::string _bTaggingAlgorithmName;
        
        std::string _sfFile;
        std::string _mcFile;
        std::string _eventViewName;
        std::vector<std::string> _jetNames;
        std::string _ljetName;

    public:
        BTagReweighting():
            Module(),
            _bTaggingAlgorithmName("pfCombinedInclusiveSecondaryVertexV2BJetTags"),
            _eventViewName("Reconstructed"),
            _jetNames({"SelectedBJet","SelectedJet"})
        {
            addSink("input", "input");
            _outputSource = addSource("output","output");
            
            addOption("b tagging algorithm","",_bTaggingAlgorithmName);
            
            addOption("SF csv file","",_sfFile,pxl::OptionDescription::USAGE_FILE_OPEN);
            addOption("MC efficiency file","",_mcFile,pxl::OptionDescription::USAGE_FILE_OPEN);
            addOption("event view name","",_eventViewName);
            addOption("jet names","",_jetNames);
            
            
        }

        ~BTagReweighting()
        {
        }

        // every Module needs a unique type
        static const std::string &getStaticType()
        {
            static std::string type ("BTagReweighting");
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
            /*
            using namespace BWGHT;
            BTagWeightCalculator calc;
            WorkingPoint testWP(0.2);
            testWP.setEfficiencyFunction(new ConstEfficiencyFunction(0.7));
            testWP.setScaleFactorFunction(new ConstScaleFactorFunction(1.0));
            //calc.addWorkingPoint(testWP);
            WorkingPoint testWP2(0.6);
            testWP2.setEfficiencyFunction(new ConstEfficiencyFunction(0.5));
            testWP2.setScaleFactorFunction(new ConstScaleFactorFunction(1.5));
            calc.addWorkingPoint(testWP2);
            calc.getEventWeight({Jet(0.1),Jet(0.7),Jet(0.5)});
            */
            getOption("b tagging algorithm",_bTaggingAlgorithmName);
            getOption("SF csv file",_sfFile);
            getOption("MC efficiency file",_mcFile);
            getOption("event view name",_eventViewName);
            getOption("jet names",_jetNames);
            
            TFile mcEffFile(_mcFile.c_str());
            
            TH2F* hist_b = dynamic_cast<TH2F*>(mcEffFile.Get("b"));
            hist_b->SetDirectory(0);
            mcEff_b.reset(hist_b);
            
            TH2F* hist_c = dynamic_cast<TH2F*>(mcEffFile.Get("c"));
            hist_c->SetDirectory(0);
            mcEff_c.reset(hist_c);
            
            TH2F* hist_other = dynamic_cast<TH2F*>(mcEffFile.Get("other"));
            hist_other->SetDirectory(0);
            mcEff_other.reset(hist_other);
            
            mcEffFile.Close();

            
            _btagCalib=BTagCalibration("csvv1", _sfFile);
            _readerNominal_mujets = BTagCalibrationReader(
                &_btagCalib,               // calibration instance
                BTagEntry::OP_TIGHT,  // operating point
                "mujets",               // measurement type
                "central"             // systematics type
            );           
            _readerUp_mujets = BTagCalibrationReader(&_btagCalib, BTagEntry::OP_TIGHT, "mujets", "up");  // sys up
            _readerDown_mujets = BTagCalibrationReader(&_btagCalib, BTagEntry::OP_TIGHT, "mujets", "down");  // sys down
                        
            _readerNominal_comb = BTagCalibrationReader(
                &_btagCalib,               // calibration instance
                BTagEntry::OP_TIGHT,  // operating point
                "incl",               // measurement type
                "central"             // systematics type
            );           
            _readerUp_comb = BTagCalibrationReader(&_btagCalib, BTagEntry::OP_TIGHT, "incl", "up");  // sys up
            _readerDown_comb = BTagCalibrationReader(&_btagCalib, BTagEntry::OP_TIGHT, "incl", "down");  // sys down
   
                        
                        
            BWGHT::WorkingPoint tightWP(0.935);
            
            tightWP.setEfficiencyFunction(new BWGHT::LambdaEfficiencyFunction([&](const BWGHT::Jet& jet, BWGHT::SYS::TYPE sys) -> double
            {
                float pt = jet.pt; 
                float eta = fabs(jet.eta);
                
                double efficiency =0.0;
                
                if (jet.flavor==5)
                {
                    int etaBin = mcEff_b->GetXaxis()->FindBin(eta);
                    int ptBin = mcEff_b->GetYaxis()->FindBin(pt);
                    efficiency = mcEff_b->GetBinContent(etaBin,ptBin);
                }
                else if (jet.flavor==4)
                {
                    int etaBin = mcEff_c->GetXaxis()->FindBin(eta);
                    int ptBin = mcEff_c->GetYaxis()->FindBin(pt);
                    efficiency = mcEff_c->GetBinContent(etaBin,ptBin);
                }
                else
                {
                    int etaBin = mcEff_other->GetXaxis()->FindBin(eta);
                    int ptBin = mcEff_other->GetYaxis()->FindBin(pt);
                    efficiency = mcEff_other->GetBinContent(etaBin,ptBin);
                }
                if (efficiency<0.01)
                {
                    efficiency+=0.002;
                }
                return efficiency;
            
            }));
            tightWP.setScaleFactorFunction(new BWGHT::LambdaScaleFactorFunction([&](const BWGHT::Jet& jet, BWGHT::SYS::TYPE sys) -> double
            {
                
                float pt = jet.pt; 
                float eta = jet.eta;
                bool doubleUncertainty = false;
                
                BTagEntry::JetFlavor flavor = BTagEntry::FLAV_B;
                double jet_scalefactor =  1.0;
                double jet_scalefactor_up =  1.0;
                double jet_scalefactor_down = 1.0;
                
                if (jet.flavor==5)
                {
                    flavor = BTagEntry::FLAV_B;
                    if (pt>MaxBJetPt)  
                    {
                        pt = MaxBJetPt; 
                        doubleUncertainty = true;
                    }
                    jet_scalefactor = _readerNominal_mujets.eval(flavor, eta, pt); 
                    jet_scalefactor_up = _readerUp_mujets.eval(flavor, eta, pt); 
                    jet_scalefactor_down = _readerDown_mujets.eval(flavor, eta, pt);   
                } 
                else if (jet.flavor==4)
                {
                    flavor = BTagEntry::FLAV_C;
                    if (pt>MaxBJetPt)  
                    {
                        pt = MaxBJetPt; 
                        doubleUncertainty = true;
                    }
                    jet_scalefactor = _readerNominal_mujets.eval(flavor, eta, pt); 
                    jet_scalefactor_up = _readerUp_mujets.eval(flavor, eta, pt); 
                    jet_scalefactor_down = _readerDown_mujets.eval(flavor, eta, pt); 
                } 
                else
                {
                    flavor = BTagEntry::FLAV_UDSG;
                    if (pt>MaxLJetPt)  
                    {
                        pt = MaxLJetPt; 
                        doubleUncertainty = true;
                    }
                    jet_scalefactor = _readerNominal_comb.eval(flavor, eta, pt); 
                    jet_scalefactor_up = _readerUp_comb.eval(flavor, eta, pt); 
                    jet_scalefactor_down = _readerDown_comb.eval(flavor, eta, pt);   
                }

                if (doubleUncertainty)
                {
                    jet_scalefactor_up = 2*(jet_scalefactor_up - jet_scalefactor) + jet_scalefactor; 
                    jet_scalefactor_down = 2*(jet_scalefactor_down - jet_scalefactor) + jet_scalefactor; 
                }

                else if (sys== BWGHT::SYS::BC_UP and (flavor==BTagEntry::FLAV_B or flavor==BTagEntry::FLAV_C))
                {
                    return jet_scalefactor_up;
                }
                else if (sys== BWGHT::SYS::BC_DOWN and (flavor==BTagEntry::FLAV_B or flavor==BTagEntry::FLAV_C))
                {
                    return jet_scalefactor_down;
                }
                else if (sys== BWGHT::SYS::L_UP and flavor==BTagEntry::FLAV_UDSG)
                {
                    return jet_scalefactor_up;
                }
                else if (sys== BWGHT::SYS::L_DOWN and flavor==BTagEntry::FLAV_UDSG)
                {
                    return jet_scalefactor_down;
                }
                return jet_scalefactor;
            
            }));
            _btagWeightCalc.addWorkingPoint(tightWP);
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
            
                    for (pxl::EventView* eventView: eventViews)
                    {
                        if (eventView->getName()==_eventViewName)
                        {
                            std::vector<pxl::Particle*> particles;
                            eventView->getObjectsOfType(particles);
                            
                            
                            std::vector<BWGHT::Jet> jets;
                            for (pxl::Particle* particle: particles)
                            {
                                //skip particles outside b-tagging acceptance
                                if (fabs(particle->getEta())>2.4)
                                {
                                    continue;
                                }
                                
                                if (std::find(_jetNames.cbegin(),_jetNames.cend(),particle->getName())!=_jetNames.cend())
                                {
                                    //use parton flavor 0 if no genParton found
                                    jets.emplace_back(particle->getUserRecord(_bTaggingAlgorithmName).toFloat(),abs(particle->hasUserRecord("partonFlavour") ? particle->getUserRecord("partonFlavour").toInt32() : 0),particle->getPt(),particle->getEta());
                                }
                            }
                            eventView->setUserRecord("btagging_nominal",_btagWeightCalc.getEventWeight(jets,BWGHT::SYS::NOMINAL));
                            eventView->setUserRecord("btagging_bc_up",_btagWeightCalc.getEventWeight(jets,BWGHT::SYS::BC_UP));
                            eventView->setUserRecord("btagging_bc_down",_btagWeightCalc.getEventWeight(jets,BWGHT::SYS::BC_DOWN));
                            eventView->setUserRecord("btagging_l_up",_btagWeightCalc.getEventWeight(jets,BWGHT::SYS::L_UP));
                            eventView->setUserRecord("btagging_l_down",_btagWeightCalc.getEventWeight(jets,BWGHT::SYS::L_DOWN));
                        }
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

PXL_MODULE_INIT(BTagReweighting)
PXL_PLUGIN_INIT
