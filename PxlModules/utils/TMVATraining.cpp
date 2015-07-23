#include "pxl/hep.hh"
#include "pxl/core.hh"
#include "pxl/core/macros.hh"
#include "pxl/core/PluginManager.hh"
#include "pxl/modules/Module.hh"
#include "pxl/modules/ModuleFactory.hh"

#include <TString.h>
#include <TMVA/Tools.h>
#include <TMVA/Factory.h>
#include <TMVA/MethodBDT.h>
#include <TMVA/MethodBase.h>
#include <TTree.h>
#include <TFile.h>
#include <TRandom.h>

#include <TNtuple.h>

#include <string>
#include <unordered_map>

static pxl::Logger logger("EventWeight");

class TMVATraining:
    public pxl::Module
{
    private:
        pxl::Source* _outputSource;

        TMVA::Factory* factory;
    public:
        TMVATraining():
            Module()
        {
            addSink("input", "input");
            _outputSource = addSource("output","output");
        }

        ~TMVATraining()
        {
        }

        // every Module needs a unique type
        static const std::string &getStaticType()
        {
            static std::string type ("TMVATraining");
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
            TMVA::Tools::Instance();
            std::string jobName = "TMVA";
            TFile f("test.root","RECREATE");
            factory = new TMVA::Factory(jobName,&f,"!V:AnalysisType=Classification");
            
            TTree signal;
            TTree background;
            double var1 = 0;
            double var2 = 0;
            signal.Branch("var1",&var1);
            signal.Branch("var2",&var2);
            background.Branch("var1",&var1);
            background.Branch("var2",&var2);
            for (unsigned int i=0; i<1000; ++i)
            {
                var1=gRandom->Uniform(-1,1);
                var2=gRandom->Gaus(0.0,0.2)*var1+gRandom->Uniform(-0.1,0.1);
                signal.Fill();
                var2=gRandom->Gaus(0.0,1.0)*var1+gRandom->Uniform(-0.2,0.2);
                background.Fill();
            }
            factory->AddSignalTree(&signal,1.0);
            factory->AddBackgroundTree(&background,1.0);
            
            factory->SetSignalWeightExpression("1.0");
            factory->SetBackgroundWeightExpression("1.0");
            
            factory->AddVariable("var1");
            factory->AddVariable("var2");
            
            factory->BookMethod(TMVA::Types::kBDT,"BDT","BoostType=AdaBoost:AdaBoostBeta=0.3");
            factory->TrainAllMethods();
            factory->TestAllMethods();
            factory->EvaluateAllMethods();

        }

        bool analyse(pxl::Sink *sink) throw (std::runtime_error)
        {
            try
            {
                pxl::Event *event  = dynamic_cast<pxl::Event*>(sink->get());
                if (event)
                {
                    
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

PXL_MODULE_INIT(TMVATraining)
PXL_PLUGIN_INIT
