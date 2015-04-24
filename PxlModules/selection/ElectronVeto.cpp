#include "pxl/hep.hh"
#include "pxl/core.hh"
#include "pxl/core/macros.hh"
#include "pxl/core/PluginManager.hh"
#include "pxl/modules/Module.hh"
#include "pxl/modules/ModuleFactory.hh"

static pxl::Logger logger("ElectronVeto");

class ElectronVeto:
    public pxl::Module
{
    private:
        pxl::Source* _outputVetoSource;
        pxl::Source* _outputOtherSource;
        
        std::string _inputEventViewName;
        std::string _inputElectronName;
        std::string _looseElectronName;
        
        bool _cleanEvent;
        
        /*Loose Electron Related Criteria*/
        double _pTMinLooseElectron;  //Minimum transverse momentum
        double _etaMaxLooseElectron; //Maximum pseudorapidity

      
    public:
        ElectronVeto():
            Module(),
            _inputEventViewName("Reconstructed"),
            _inputElectronName("Electron"),
            _looseElectronName("LooseElectron"),
            
            _cleanEvent(true),

            _pTMinLooseElectron(20),
            _etaMaxLooseElectron(2.5)

        {
            addSink("input", "input");
            _outputVetoSource = addSource("veto loose electrons", "veto");
            _outputOtherSource = addSource("other", "other");

            addOption("Event view","name of the event view where electrons are selected",_inputEventViewName);
            addOption("Input electron name","name of particles to consider for selection",_inputElectronName);
            addOption("Name of selected loose electrons","",_looseElectronName);
            addOption("Clean event","this option will clean the event of all electrons falling loose criteria",_cleanEvent);

            addOption("LooseElectron Minimum pT","",_pTMinLooseElectron);
            addOption("LooseElectron Maximum eta","",_etaMaxLooseElectron);
        }

        ~ElectronVeto()
        {
        }

        // every Module needs a unique type
        static const std::string &getStaticType()
        {
            static std::string type ("ElectronVeto");
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
            getOption("Event view",_inputEventViewName);
            getOption("Input electron name",_inputElectronName);
            getOption("Name of selected loose electrons",_looseElectronName);
            
            getOption("Clean event",_cleanEvent);
            
            getOption("LooseElectron Minimum pT",_pTMinLooseElectron);
            getOption("LooseElectron Maximum eta",_etaMaxLooseElectron);

        }

        bool passesLooseCriteria(pxl::Particle* particle)
        {
            if (not (particle->getPt()>_pTMinLooseElectron))
            {
                return false;
            }
            if (not (fabs(particle->getEta())<_etaMaxLooseElectron))
            {
                return false;
            }
            if (not particle->getUserRecord("phys14eleIDVeto"))
            {
                return false;
            }
            if (fabs(particle->getEta())>1.4442 && fabs(particle->getEta())<1.5660)
            {
                return false;
            }
            if (not particle->getUserRecord("passConversionVeto"))
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
                    std::vector<pxl::EventView*> eventViews;
                    event->getObjectsOfType(eventViews);
                    
                    std::vector<pxl::Particle*> looseElectrons;
                    std::vector<pxl::Particle*> otherElectrons;
                    
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
                                    if (passesLooseCriteria(particle))
                                    {
                                        looseElectrons.push_back(particle);
                                    }
                                    else
                                    {
                                        otherElectrons.push_back(particle);
                                    }
                                }
                            }
                        }
                        if (_cleanEvent)
                        {
                            for (unsigned int iparticle = 0; iparticle < otherElectrons.size(); ++iparticle)
                            {
                                eventView->removeObject(otherElectrons[iparticle]);
                            }
                        }

                        if (looseElectrons.size()==0)
                        {
                            _outputVetoSource->setTargets(event);
                            return _outputVetoSource->processTargets();
                        }
                        else
                        {
                            for (unsigned int i=0; i < looseElectrons.size(); ++i)
                            {
                                looseElectrons[i]->setName(_looseElectronName);
                            }
                            _outputOtherSource->setTargets(event);
                            return _outputOtherSource->processTargets();
                        }
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

PXL_MODULE_INIT(ElectronVeto)
PXL_PLUGIN_INIT
