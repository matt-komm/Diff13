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
        
        std::string _inputightElectronName;
        std::string _inputEventViewName;
        std::string _tightElectronName;
        std::string _looseElectronName;
        
        bool _cleanEvent;
      
        /*Tight Electron Related Criteria*/
        double _pTmintightElectron;  //Minimum transverse momentum
        double _etamaxtightElectron; //Maximum pseudorapidity
        std::string _idstightElectron; //Electron Conditions as provided by the user (USED ONLY FOR ONE ELECTRON ID for the moment)
        //std::vector<std::string> _idtightElectron; //Vector to store each Electron Condition sepately (IDLE for the moment)
        int64_t _losthitstightElectron; // Number of lost hits 
  
        int64_t _numtightElectrons; //Number of selected tight electrons

        /*Loose Electron Related Criteria*/
        double _pTminlooseElectron;  //Minimum transverse momentum
        double _etamaxlooseElectron; //Maximum pseudorapidity

        int64_t _numlooseElectrons; //Number of selected loose electrons
      
    public:
        ElectronSelection():
            Module(),
            _inputightElectronName("Electron"),
            _inputEventViewName("Reconstructed"),
            _tightElectronName("TightElectron"),
            _looseElectronName("LooseElectron"),
            _cleanEvent(true),
	    _pTmintightElectron(10),
	    _etamaxtightElectron(2.5),
	    _idstightElectron("phys14eleIDVeto"),
	    _losthitstightElectron(0),
	    _numtightElectrons(1),
	    _pTminlooseElectron(10),
            _etamaxlooseElectron(2.5),
            _numlooseElectrons(0)

        {
            addSink("input", "input");
            _outputSource = addSource("selected", "selected");
            _outputVetoSource = addSource("veto", "veto");

            addOption("Event view","name of the event view where electrons are selected",_inputEventViewName);
            addOption("Input electron name","name of particles to consider for selection",_inputightElectronName);
            addOption("Name of selected tight electrons","",_tightElectronName);
            addOption("Name of selected loose electrons","",_looseElectronName);
            addOption("Clean event","this option will clean the event of all electrons falling tight or loose criteria",_cleanEvent);

	    addOption("TightElectron Minimum pT","",_pTmintightElectron);
            addOption("TightElectron Maximum eta","",_etamaxtightElectron);
	    addOption("TightElectron ID","",_idstightElectron);
	    addOption("TightElectron Lost Hits","",_losthitstightElectron);
            addOption("Number of TightElectrons to Select","",_numtightElectrons);

	    addOption("LooseElectron Minimum pT","",_pTminlooseElectron);
            addOption("LooseElectron Maximum eta","",_etamaxlooseElectron);
            addOption("Number of LooseElectrons to Select","",_numlooseElectrons);
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
            getOption("Input electron name",_inputightElectronName);
            getOption("Name of selected tight electrons",_tightElectronName);
            getOption("Name of selected loose electrons",_looseElectronName);
            getOption("Clean event",_cleanEvent);

            getOption("TightElectron Minimum pT",_pTmintightElectron);
            getOption("TightElectron Maximum eta",_etamaxtightElectron);
	    getOption("TightElectron ID",_idstightElectron);
	    getOption("TightElectron Lost Hits",_losthitstightElectron);
            getOption("Number of TightElectrons to Select",_numtightElectrons);

	    getOption("LooseElectron Minimum pT",_pTminlooseElectron);
            getOption("LooseElectron Maximum eta",_etamaxlooseElectron);
            getOption("Number of LooseElectrons to Select",_numlooseElectrons);

        }

        bool passTightCriteria(pxl::Particle* particle)
        {
            //TODO: need to be extended to recommendation!
            if (not (particle->getPt()>_pTmintightElectron)) {
                return false;
            }
            if (not (fabs(particle->getEta())<_etamaxtightElectron)) {
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

        bool passLooseCriteria(pxl::Particle* particle)
        {
	  
            //TODO: need to be extended to recommendation
            if (not (particle->getPt()>_pTminlooseElectron)) {
                return false;
            }
            if (not (fabs(particle->getEta())<_etamaxlooseElectron)) {
                return false;
            }
	    if (particle->hasUserRecord(_idstightElectron.data())) {
	        if (not (particle->getUserRecord(_idstightElectron.data()).toBool())) {
		    return false;
		}
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

                                if (particle->getName()==_inputightElectronName)
                                {

                                    if (passTightCriteria(particle))
                                    {
                                        tightElectrons.push_back(particle);
                                    } else if (passLooseCriteria(particle)) {
                                        looseElectrons.push_back(particle);
                                    } else {
                                        otherElectrons.push_back(particle);
                                    }
                                }
                            }
                        }
                    
                        if (tightElectrons.size()==_numtightElectrons && looseElectrons.size()==_numlooseElectrons)
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
