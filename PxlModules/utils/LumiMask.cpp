#include "pxl/hep.hh"
#include "pxl/core.hh"
#include "pxl/core/macros.hh"
#include "pxl/core/PluginManager.hh"
#include "pxl/modules/Module.hh"
#include "pxl/modules/ModuleFactory.hh"

#include "rapidjson/document.h"

#include <string>
#include <unordered_map>
#include <unordered_set>
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
        
        std::unordered_map<unsigned int,std::unordered_set<unsigned int>> _mask;
        
    public:
        LumiMask():
            Module(),
            _lumiField("LuminosityBlock"),
            _runField("Run")
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
            
            try
            {
            
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

                printf("Parsing lumi mask .....\n");
                for (rapidjson::Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr)
                {
                    unsigned int runNumber = std::atol(itr->name.GetString());
                    printf("run %u: ", runNumber);
                    const rapidjson::Value& lumis = document[itr->name.GetString()];
                    unsigned int nLumis=0;
                    for (rapidjson::SizeType i = 0; i < lumis.Size(); i++)
                    {
                        rapidjson::SizeType first = 0;
                        rapidjson::SizeType last = 1;
                        unsigned int lumiStart = lumis[i][first].GetUint();
                        unsigned int lumiEnd = lumis[i][last].GetUint();
                        printf("%u-%u,", lumiStart,lumiEnd);
                        
                        for (unsigned int ilumi = lumiStart; ilumi<lumiEnd+1; ++ilumi)
                        {
                            _mask[runNumber].insert(ilumi);
                            ++nLumis;
                        }
                        
                    }
                    printf(" total lumis = %u\n",nLumis);
                }
                printf("   ..... done!\n");
            } 
            catch (...)
            {
                throw std::runtime_error(getName()+": error during parsing of lumi mask!");
            }
        }

        bool analyse(pxl::Sink *sink) throw (std::runtime_error)
        {
            try
            {
                pxl::Event *event  = dynamic_cast<pxl::Event*>(sink->get());
                if (event)
                {
                    unsigned int runNumber = event->getUserRecord(_runField).toUInt32();
                    unsigned int lumiNumber = event->getUserRecord(_lumiField).toUInt32();
		            
		            auto lumiSet = _mask.find(runNumber);
		            if (lumiSet!=_mask.end())
		            {
		                
		                auto lumi = lumiSet->second.find(lumiNumber);
		                if (lumi!=lumiSet->second.end())
		                {
		                    _outputSource->setTargets(event);
                            return _outputSource->processTargets();
		                }
		            }
                    _outputVetoSource->setTargets(event);
                    return _outputVetoSource->processTargets();
                    
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
