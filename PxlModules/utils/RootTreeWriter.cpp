#include "pxl/hep.hh"
#include "pxl/core.hh"
#include "pxl/core/macros.hh"
#include "pxl/core/PluginManager.hh"
#include "pxl/modules/Module.hh"
#include "pxl/modules/ModuleFactory.hh"

#include "OutputStore.hpp"

static pxl::Logger logger("RootTreeWriter");

class RootTreeWriter:
    public pxl::Module
{
    private:
        pxl::Source* _outputSource;
        
        std::string _outputFileName;
        
        OutputStore* _store;
        
    public:
        RootTreeWriter():
            Module(),
            _store(nullptr)
        {
            addSink("input", "input");
            _outputSource = addSource("output", "output");
            
            addOption("root file","",_outputFileName,pxl::OptionDescription::USAGE_FILE_SAVE);
        }

        ~RootTreeWriter()
        {
        }

        // every Module needs a unique type
        static const std::string &getStaticType()
        {
            static std::string type ("RootTreeWriter");
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
            getOption("root file",_outputFileName);
            _store = new OutputStore(_outputFileName);
        }

        bool analyse(pxl::Sink *sink) throw (std::runtime_error)
        {
            try
            {
                pxl::Event *event  = dynamic_cast<pxl::Event *> (sink->get());
                if (event)
                {
                    Tree* tree = _store->getTree(event->getUserRecord("Process"));

                    tree->storeVariable("event_number",(int)event->getUserRecord("Event number").asUInt64());
                
                    tree->fill();
                    
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
        
        void endJob()
        {
            if (_store)
            {
                _store->close();
                delete _store;
            }
        }

        void shutdown() throw(std::runtime_error)
        {
        }

        void destroy() throw (std::runtime_error)
        {
            delete this;
        }
};

PXL_MODULE_INIT(RootTreeWriter)
PXL_PLUGIN_INIT
