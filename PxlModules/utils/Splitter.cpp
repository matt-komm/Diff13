#include "pxl/hep.hh"
#include "pxl/core.hh"
#include "pxl/core/macros.hh"
#include "pxl/core/PluginManager.hh"
#include "pxl/modules/Module.hh"
#include "pxl/modules/ModuleFactory.hh"

#include <string>
#include <random>

static pxl::Logger logger("Splitter");


class Splitter:
    public pxl::Module
{
    private:
        pxl::Source* _outputSource;
        pxl::Source* _outputVetoSource;
        
        double _percentage;
        int64_t _maxEvents;
        bool _random;
        
        unsigned int _passedEvents;
        unsigned int _selectedEvents;
        
        std::random_device _randomDevice;
        std::mt19937 _generator;
        std::uniform_real_distribution<> _uniformDist;
        
    public:
        Splitter():
            Module(),
            _percentage(0.5),
            _maxEvents(-1),
            _random(true),
            _passedEvents(0),
            _selectedEvents(0),
            _generator(_randomDevice()),
            _uniformDist(0, 1)
            
        {
            addSink("input", "input");
            _outputSource = addSource("output","output");
            _outputVetoSource = addSource("veto","veto");
            
            addOption("percentage","percentage of events to select",_percentage);
            addOption("max events","limit maximal total amount of selected events",_maxEvents);
            addOption("random","select events randomly",_random);
        }

        ~Splitter()
        {
        }

        // every Module needs a unique type
        static const std::string &getStaticType()
        {
            static std::string type ("Splitter");
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
            getOption("percentage",_percentage);
            getOption("max events",_maxEvents);
            getOption("random",_random);
            

        }

        bool analyse(pxl::Sink *sink) throw (std::runtime_error)
        {
            try
            {
                pxl::Event *event  = dynamic_cast<pxl::Event*>(sink->get());
                
                _passedEvents+=1;
                if ((_maxEvents>=0) and _selectedEvents>=_maxEvents)
                {
                    _outputVetoSource->setTargets(event);
                    return _outputVetoSource->processTargets();
                }
                
                
                if (_random)
                {
                    if (_uniformDist(_generator)<_percentage)
                    {
                        _outputSource->setTargets(event);
                        return _outputSource->processTargets();
                    }
                    else
                    {
                        _outputVetoSource->setTargets(event);
                        return _outputVetoSource->processTargets();
                    }
                }
                
                else
                {
                    double fraction = 1.0*_selectedEvents/_passedEvents;
                    if (fraction<_percentage)
                    {
                        _selectedEvents+=1;
                        _outputSource->setTargets(event);
                        return _outputSource->processTargets();
                    }
                    else
                    {
                        _outputVetoSource->setTargets(event);
                        return _outputVetoSource->processTargets();
                    }
                    
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

PXL_MODULE_INIT(Splitter)
PXL_PLUGIN_INIT
