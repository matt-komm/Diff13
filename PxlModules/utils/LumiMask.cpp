#include "pxl/hep.hh"
#include "pxl/core.hh"
#include "pxl/core/macros.hh"
#include "pxl/core/PluginManager.hh"
#include "pxl/modules/Module.hh"
#include "pxl/modules/ModuleFactory.hh"

#include "rapidjson/document.h"

#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>

static pxl::Logger logger("LumiMask");


class LumiMask:
    public pxl::Module
{
    private:
        pxl::Source* _outputSource;
        pxl::Source* _outputVetoSource;

        std::string _lumiField;
        std::string _runField;
        std::string _maskFileName;
        
    public:
        LumiMask():
            Module()
        {
            addSink("input", "input");
            _outputSource = addSource("output","output");
            _outputVetoSource = addSource("veto","veto");
            
            addOption("name of lumi field","",_lumiField);
            addOption("name of run field","",_runField);
            addOption("mask file","",_maskFileName,pxl::OptionDescription::USAGE_FILE_OPEN);
        }

        ~LumiMask()
        {
        }

        // every Module needs a unique type
        static const std::string &getStaticType()
        {
            static std::string type ("LumiMask");
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
            getOption("name of lumi field",_lumiField);
            getOption("name of run field",_runField);
            getOption("mask file",_maskFileName);
            
            std::stringstream maskData;
            std::ifstream f(_maskFileName, std::ios::in);
            std::string line;
            if (f.is_open())
            {
                while ( getline (f,line) )
                {
                    maskData << line;
                }
                f.close();
            }
            rapidjson::Document document;
            const char* data = maskData.str().c_str();
            document.Parse<0>(data);
            for (rapidjson::Value::ConstMemberIterator it = document.MemberBegin(); it != document.MemberEnd(); ++it)
            {
                printf("member %s\n", it->name.GetString());
            }
        }

        bool analyse(pxl::Sink *sink) throw (std::runtime_error)
        {
            try
            {
                pxl::Event *event  = dynamic_cast<pxl::Event*>(sink->get());
                if (event)
                {
                    //std::string processName = event->getUserRecord(_processNameField);
		    

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

PXL_MODULE_INIT(LumiMask)
PXL_PLUGIN_INIT
