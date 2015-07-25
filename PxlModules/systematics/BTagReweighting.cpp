#include "pxl/hep.hh"
#include "pxl/core.hh"
#include "pxl/core/macros.hh"
#include "pxl/core/PluginManager.hh"
#include "pxl/modules/Module.hh"
#include "pxl/modules/ModuleFactory.hh"

#include "BTagWeightCalculator.hpp"

#include <string>
#include <unordered_map>

static pxl::Logger logger("BTagReweighting");

class BTagReweighting:
    public pxl::Module
{
    private:
        pxl::Source* _outputSource;

    public:
        BTagReweighting():
            Module()
        {
            addSink("input", "input");
            _outputSource = addSource("output","output");
            
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
            
            using namespace BWGHT;
            BTagWeightCalculator calc;
            WorkingPoint testWP(0.2);
            testWP.setEfficiencyFunction(new ConstEfficiencyFunction(0.7));
            testWP.setScaleFactorFunction(new ConstScaleFactorFunction(1.0));
            calc.addWorkingPoint(testWP);
            WorkingPoint testWP2(0.6);
            testWP2.setEfficiencyFunction(new ConstEfficiencyFunction(0.25));
            testWP2.setScaleFactorFunction(new ConstScaleFactorFunction(1.0));
            calc.addWorkingPoint(testWP2);
            calc.getEventWeight({Jet(0.1),Jet(0.7),Jet(0.5)});
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
