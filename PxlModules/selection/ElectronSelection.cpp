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
        
        std::string _inputTightElectronName;
        std::string _inputEventViewName;
        std::string _tightElectronName;
        std::string _looseElectronName;
        
        bool _cleanEvent;

        int64_t _numTightElectrons; //Number of selected tight electrons
        int64_t _numLooseElectrons; //Number of selected loose electrons
      
        /*Tight Electron Related Criteria*/
        double _pTMinTightElectron;  //Minimum transverse momentum
        double _etaMaxTightElectron; //Maximum pseudorapidity

        /*Loose Electron Related Criteria*/
        double _pTMinLooseElectron;  //Minimum transverse momentum
        double _etaMaxLooseElectron; //Maximum pseudorapidity

      
    public:
        ElectronSelection():
            Module(),
            _inputTightElectronName("Electron"),
            _inputEventViewName("Reconstructed"),
            _tightElectronName("TightElectron"),
            _looseElectronName("LooseElectron"),
            _cleanEvent(true),

            _numTightElectrons(1),
            _numLooseElectrons(0),

            _pTMinTightElectron(35),
            _etaMaxTightElectron(2.1),

            _pTMinLooseElectron(20),
            _etaMaxLooseElectron(2.5)

        {
            addSink("input", "input");
            _outputSource = addSource("selected", "selected");
            _outputVetoSource = addSource("veto", "veto");

            addOption("Event view","name of the event view where electrons are selected",_inputEventViewName);
            addOption("Input electron name","name of particles to consider for selection",_inputTightElectronName);
            addOption("Name of selected tight electrons","",_tightElectronName);
            addOption("Name of selected loose electrons","",_looseElectronName);
            addOption("Clean event","this option will clean the event of all electrons falling tight or loose criteria",_cleanEvent);

            addOption("Number of TightElectrons to Select","",_numTightElectrons);
            addOption("Number of LooseElectrons to Select","",_numLooseElectrons);

            addOption("TightElectron Minimum pT","",_pTMinTightElectron);
            addOption("TightElectron Maximum eta","",_etaMaxTightElectron);

            addOption("LooseElectron Minimum pT","",_pTMinLooseElectron);
            addOption("LooseElectron Maximum eta","",_etaMaxLooseElectron);
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
            getOption("Event view",_inputEventViewName);
            getOption("Input electron name",_inputTightElectronName);
            getOption("Name of selected tight electrons",_tightElectronName);
            getOption("Name of selected loose electrons",_looseElectronName);
            getOption("Clean event",_cleanEvent);

            getOption("Number of TightElectrons to Select",_numTightElectrons);
            getOption("Number of LooseElectrons to Select",_numLooseElectrons);

            getOption("TightElectron Minimum pT",_pTMinTightElectron);
            getOption("TightElectron Maximum eta",_etaMaxTightElectron);

            getOption("LooseElectron Minimum pT",_pTMinLooseElectron);
            getOption("LooseElectron Maximum eta",_etaMaxLooseElectron);

        }

        bool passTightCriteria(pxl::Particle* particle)
        {
            //TODO: need to be extended to recommendation?
            if (not (particle->getPt()>_pTMinTightElectron))
            {
                return false;
            }
            if (not (fabs(particle->getEta())<_etaMaxTightElectron))
            {
                return false;
            }
            if (not particle->getUserRecord("phys14eleIDTight"))
            {
                return false;
            }
            return true;
        }

        bool passLooseCriteria(pxl::Particle* particle)
        {
            //TODO: need to be extended to recommendation?
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
            if (fabs(particle->getEta())<1.5660 && fabs(particle->getEta())>1.4442)
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
                    
                    std::vector<pxl::Particle*> tightElectrons;
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

                                if (particle->getName()==_inputTightElectronName)
                                {

                                    if (passTightCriteria(particle))
                                    {
                                        tightElectrons.push_back(particle);
                                    }
                                    else if (passLooseCriteria(particle))
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
                    
                        if (tightElectrons.size()==_numTightElectrons && looseElectrons.size()==_numLooseElectrons)
                        {
                            for (unsigned int i=0; i < tightElectrons.size(); ++i)
                            {
                                tightElectrons[i]->setName(_tightElectronName);
                            }
                            for (unsigned int i=0; i < looseElectrons.size(); ++i)
                            {
                                looseElectrons[i]->setName(_looseElectronName);
                            }
                            for (unsigned int i=0; _cleanEvent && (i < otherElectrons.size()); ++i)
                            {
                                eventView->removeObject(otherElectrons[i]);
                            }
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
