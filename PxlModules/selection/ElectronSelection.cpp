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
        std::string _tElectronName;
        std::string _lElectronName;
        
        bool _cleanEvent;
      
        /*Tight Electron Related Criteria*/
        double _pTmintElectron;  //Minimum transverse momentum
        double _etamaxtElectron; //Maximum pseudorapidity
        std::string _idstElectron; //Electron Conditions as provided by the user 
        std::vector<std::string> _idtElectron; //Vector to store each Electron Condition sepately
        int64_t _losthitstElectron; // Number of lost hits 
  
        int64_t _numtElectrons; //Number of selected tight muons

        /*Loose Electron Related Criteria*/
        double _pTminlElectron;  //Minimum transverse momentum
        double _etamaxlElectron; //Maximum pseudorapidity

        int64_t _numlElectrons; //Number of selected loose muons
      
    public:
        ElectronSelection():
            Module(),
            _inputElectronName("Electron"),
            _inputEventViewName("Reconstructed"),
            _tElectronName("TightElectron"),
            _lElectronName("LooseElectron"),
            _cleanEvent(true),
	    _pTmintElectron(10),
	    _etamaxtElectron(2.5),
	    _idstElectron("isInEB-EE"),
	    _losthitstElectron(0),
	    _numtElectrons(1),
	    _pTminlElectron(10),
            _etamaxlElectron(2.5),
            _numlElectrons(0)

        {
            addSink("input", "input");
            _outputSource = addSource("selected", "selected");
            _outputVetoSource = addSource("veto", "veto");

            addOption("Event view","name of the event view where electrons are selected",_inputEventViewName);
            addOption("Input electron name","name of particles to consider for selection",_inputElectronName);
            addOption("Name of selected tight electrons","",_tElectronName);
            addOption("Name of selected loose electrons","",_lElectronName);
            addOption("Clean event","this option will clean the event of all electrons falling tight or loose criteria",_cleanEvent);

	    addOption("TightElectron Minimum pT","",_pTmintElectron);
            addOption("TightElectron Maximum eta","",_etamaxtElectron);
	    addOption("TightElectron ID","",_idstElectron);
	    addOption("TightElectron Lost Hits","",_losthitstElectron);
            addOption("Number of TightElectrons to Select","",_numtElectrons);

	    addOption("LooseElectron Minimum pT","",_pTminlElectron);
            addOption("LooseElectron Maximum eta","",_etamaxlElectron);
            addOption("Number of LooseElectrons to Select","",_numlElectrons);
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
            getOption("Input electron name",_inputElectronName);
            getOption("Name of selected tight electrons",_tElectronName);
            getOption("Name of selected loose electrons",_lElectronName);
            getOption("Clean event",_cleanEvent);

            getOption("TightElectron Minimum pT",_pTmintElectron);
            getOption("TightElectron Maximum eta",_etamaxtElectron);
	    getOption("TightElectron ID",_idstElectron);
	    getOption("TightElectron Lost Hits",_losthitstElectron);
            getOption("Number of TightElectrons to Select",_numtElectrons);

	    getOption("LooseElectron Minimum pT",_pTminlElectron);
            getOption("LooseElectron Maximum eta",_etamaxlElectron);
            getOption("Number of LooseElectrons to Select",_numlElectrons);

        }

        bool passTightCriteria(pxl::Particle* particle)
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

        bool passLooseCriteria(pxl::Particle* particle)
        {
            //TODO: need to be extended to recommendation
            if (not (particle->getPt()>10.0)) {
                return false;
            }
            if (not (fabs(particle->getEta())<2.5)) {
                return false;
            }

            /*
            if (not (particle->getUserRecord("relIso").toFloat()<0.2)) {
                return false;
            }
            if (not (fabs(particle->getUserRecord("dxy").toFloat())<0.2)) {
                return false;
            }
            if (not (fabs(particle->getUserRecord("dz").toFloat())<0.5)) {
                return false;
            }
            */
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
                    
                    std::vector<pxl::Particle*> tElectrons;
                    std::vector<pxl::Particle*> lElectrons;
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
                                    if (passTightCriteria(particle))
                                    {
                                        tElectrons.push_back(particle);
                                    } else if (passLooseCriteria(particle)) {
                                        lElectrons.push_back(particle);
                                    } else {
                                        otherElectrons.push_back(particle);
                                    }
                                }
                            }
                        }
                    
                        if (tElectrons.size()==_numtElectrons && lElectrons.size()==_numlElectrons)
                        {
                            for (unsigned int i=0; i < tElectrons.size(); ++i)
                            {
                                tElectrons[i]->setName(_tElectronName);
                            }
                            for (unsigned int i=0; i < lElectrons.size(); ++i)
                            {
                                lElectrons[i]->setName(_lElectronName);
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
