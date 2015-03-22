#include "pxl/hep.hh"
#include "pxl/core.hh"
#include "pxl/core/macros.hh"
#include "pxl/core/PluginManager.hh"
#include "pxl/modules/Module.hh"
#include "pxl/modules/ModuleFactory.hh"

static pxl::Logger logger("ElectronSelection");

class ElectronSelection:
    public pxl::Module
{
    private:
        pxl::Source* _outputSource;
        pxl::Source* _outputVetoSource;
        
        std::string _inputElectronName;
        std::string _inputEventViewName;
        std::string _tightElectronName;
        std::string _looseElectronName;
        
        bool _cleanEvent;
        
        int64_t _numTightElectrons;
        int64_t _numLooseElectrons;

    public:
        ElectronSelection():
            Module(),
            _inputElectronName("Electron"),
            _inputEventViewName("Reconstructed"),
            _tightElectronName("TightElectron"),
            _looseElectronName("LooseElectron"),
            _cleanEvent(true),
            _numTightElectrons(1),
            _numLooseElectrons(0)
        {
            addSink("input", "input");
            _outputSource = addSource("selected", "selected");
            _outputVetoSource = addSource("veto", "veto");

            addOption("event view","name of the event view where Electrons are selected",_inputEventViewName);
            addOption("input electron name","name of particles to consider for selection",_inputElectronName);
            addOption("name of selected tight electrons","",_tightElectronName);
            addOption("name of selected loose electrons","",_looseElectronName);
            addOption("clean event","this option will clean the event of all electrons falling tight or loose criteria",_cleanEvent);
            
            addOption("numTightElectrons","number of tight electrons",_numTightElectrons);
            addOption("numLooseElectrons","number of loose electrons",_numLooseElectrons);
        }

        ~ElectronSelection()
        {
        }

        // every Module needs a unique type
        static const std::string &getStaticType()
        {
            static std::string type ("ElectronSelection");
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
            getOption("event view",_inputEventViewName);
            getOption("input electron name",_inputElectronName);
            getOption("name of selected tight electrons",_tightElectronName);
            getOption("name of selected loose electrons",_looseElectronName);
            getOption("clean event",_cleanEvent);
            
            getOption("numTightElectrons",_numTightElectrons);
            getOption("numLooseElectrons",_numLooseElectrons);
        }

        bool passTightCriteria(pxl::Particle* particle)
        {
            if (not (particle->getPt()>26.0)) {
                return false;
            }
            if (not (fabs(particle->getEta())<2.1)) {
                return false;
            }
            
            return true;
        }

        bool passLooseCriteria(pxl::Particle* particle)
        {
            //TODO: need to be extended to recommendation
            if (not (particle->getPt()>10.0)) {
                return false;
            }
            if (not (fabs(particle->getEta())<2.5)) {
                return false;
            }
            if (particle->getUserRecord("isInEB-EE").toBool())
            {
                return false;
            }
            if (particle->getUserRecord("lostHits").toInt32()==0)
            {
                return false;
            }
            return true;
        }

        bool analyse(pxl::Sink *sink) throw (std::runtime_error)
        {
            try
            {
                pxl::Event *event  = dynamic_cast<pxl::Event *> (sink->get());
                if (event)
                {
                    unsigned int numTightElectrons=0;
                    unsigned int numLooseElectrons=0;
                    std::vector<pxl::EventView*> eventViews;
                    event->getObjectsOfType(eventViews);
                    for (unsigned ieventView=0; ieventView<eventViews.size();++ieventView)
                    {
                        pxl::EventView* eventView = eventViews[ieventView];
                        if (eventView->getName()==_inputEventViewName)
                        {
                            std::vector<pxl::Particle*> particles;
                            eventView->getObjectsOfType(particles);

                            for (unsigned iparticle=0; iparticle<particles.size();++iparticle)
                            {
                                pxl::Particle* particle = particles[iparticle];

                                if (particle->getName()==_inputElectronName)
                                {
                                    if (passTightCriteria(particle))
                                    {
                                        particle->setName(_tightElectronName);
                                        ++numTightElectrons;
                                    } else if (passLooseCriteria(particle)) {
                                        particle->setName(_looseElectronName);
                                        ++numLooseElectrons;
                                    } else if (_cleanEvent) {
                                        eventView->removeObject(particle);
                                    }

                                }
                            }

                        }
                    }
                    if (numTightElectrons==_numTightElectrons && numLooseElectrons==_numLooseElectrons)
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

PXL_MODULE_INIT(ElectronSelection)
PXL_PLUGIN_INIT
