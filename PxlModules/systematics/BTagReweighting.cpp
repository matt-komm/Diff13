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

static pxl::Logger logger("BTagReweighting");

class BTagReweighting:
    public pxl::Module
{
    private:
        pxl::Source* _outputSource;
        
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
            
            
            BTagCalibration calib("csvv1", _sfFile);
            BTagCalibrationReader reader(
                &calib,               // calibration instance
                BTagEntry::OP_TIGHT,  // operating point
                "comb",               // measurement type
                "central"             // systematics type
            );           
            BTagCalibrationReader reader_up(&calib, BTagEntry::OP_TIGHT, "comb", "up");  // sys up
            BTagCalibrationReader reader_do(&calib, BTagEntry::OP_TIGHT, "comb", "down");  // sys down
                        
            BWGHT::WorkingPoint tightWP(0.97);
            
            tightWP.setEfficiencyFunction(new BWGHT::ConstEfficiencyFunction(1.0));
            tightWP.setScaleFactorFunction(new BWGHT::LambdaScaleFactorFunction([&](const BWGHT::Jet& jet, BWGHT::SYS::TYPE sys) -> double
            {
            
                    float pt = jet.pt; 
                    float eta = jet.eta;
                    bool doubleUncertainty = false;
                    if (pt>MaxBJetPt)  
                    {
                        pt = MaxBJetPt; 
                        doubleUncertainty = true;
                    }  

                    // Note: this is for b jets, for c jets (light jets) use FLAV_C (FLAV_UDSG)
                    double jet_scalefactor = reader.eval(BTagEntry::FLAV_B, eta, pt); 
                    double jet_scalefactor_up =  reader_up.eval(BTagEntry::FLAV_B, eta, pt); 
                    double jet_scalefactor_do =  reader_do.eval(BTagEntry::FLAV_B, eta, pt); 

                    if (doubleUncertainty)
                    {
                        jet_scalefactor_up = 2*(jet_scalefactor_up - jet_scalefactor) + jet_scalefactor; 
                        jet_scalefactor_do = 2*(jet_scalefactor_do - jet_scalefactor) + jet_scalefactor; 
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
                                if (std::find(_jetNames.cbegin(),_jetNames.cend(),particle->getName())!=_jetNames.cend())
                                {
                                    jets.emplace_back(particle->getUserRecord(_bTaggingAlgorithmName),particle->getUserRecord("partonFlavor"),particle->getPt(),particle->getEta());
                                }
                            }
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
