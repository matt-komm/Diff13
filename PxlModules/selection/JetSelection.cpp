#include "pxl/hep.hh"
#include "pxl/core.hh"
#include "pxl/core/macros.hh"
#include "pxl/core/PluginManager.hh"
#include "pxl/modules/Module.hh"
#include "pxl/modules/ModuleFactory.hh"

static pxl::Logger logger("JetSelection");

class JetSelection:
    public pxl::Module
{
    private:
        pxl::Source* _output1JetsSource;
        pxl::Source* _output2JetsSource;
        pxl::Source* _output3JetsSource;
        pxl::Source* _output4JetsSource;
        pxl::Source* _outputOtherJetsSource;

        std::string _inputJetName;
        std::string _inputEventViewName;
        std::string _selectedJetName;
        
        bool _cleanEvent;
        
        double _pTmin;
        double _etamax;
        
        bool _dRInvert;
        double _dR;
        std::vector<std::string> _dRObjects;

    public:
        JetSelection():
            Module(),
            _inputJetName("Jet"),
            _inputEventViewName("Reconstructed"),
            _selectedJetName("SelectedJet"),
            _cleanEvent(true),
            _pTmin(40),
            _etamax(4.5),
            _dRInvert(false),
            _dR(0.4)
        {
            addSink("input", "input");
            _output1JetsSource = addSource("1 jets", "1 jets");
            _output2JetsSource = addSource("2 jets", "2 jets");
            _output3JetsSource = addSource("3 jets", "3 jets");
            _output4JetsSource = addSource("4 jets", "4 jets");
            _outputOtherJetsSource = addSource("other", "other");
            
            addOption("event view","name of the event view where jets are selected",_inputEventViewName);
            addOption("input jet name","name of particles to consider for selection",_inputJetName);
            addOption("name of selected jets","",_selectedJetName);
            addOption("clean event","this option will clean the event of all jets falling cuts",_cleanEvent);
            
            addOption("Jet min pT cut","",_pTmin);
            addOption("Get max eta cut","",_etamax);
            
            addOption("invert dR","inverts dR cleaning",_dRInvert);
            
            addOption("dR cut","remove jets close to other objects, e.g. leptons",_dR);
            addOption("dR objects","object names to which the jets should not be close to",_dRObjects);
        }

        ~JetSelection()
        {
        }

        // every Module needs a unique type
        static const std::string &getStaticType()
        {
            static std::string type ("JetSelection");
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
            getOption("input jet name",_inputJetName);
            getOption("name of selected jets",_selectedJetName);
            getOption("clean event",_cleanEvent);
            
            getOption("pT cut",_pTmin);
            getOption("eta cut",_etamax);
            
            getOption("invert dR",_dRInvert);
            getOption("dR cut",_dR);
            getOption("dR objects",_dRObjects);
            if (_dRObjects.size()==0)
            {
                _dR=-1;
            }
            
        }

        bool passJetSelection(pxl::Particle* particle)
        {
            //TODO: need to be extended to recommendation
            if (not (particle->getPt()>_pTmin)) {
                return false;
            }
            if (not (fabs(particle->getEta())<_etamax)) {
                return false;
            }
            return true;
        }
        
        void applyDRcleaning(pxl::EventView* eventView, std::vector<pxl::Particle*>& selectedJets, std::vector<pxl::Particle*>& dRCleaningObjects)
        {
            for (std::vector<pxl::Particle*>::iterator it = selectedJets.begin(); it != selectedJets.end(); )
            {
                pxl::Particle* selectedJet = *it;
                double dRmin = 100.0;
                for (unsigned iparticle=0; iparticle<dRCleaningObjects.size();++iparticle)
                {
                    pxl::Particle* dRCleanObj = dRCleaningObjects[iparticle];
                    double dR = selectedJet->getVector().deltaR(&dRCleanObj->getVector());
                    if (!_dRInvert && dR<_dR)
                    {
                        eventView->removeObject(selectedJet);
                        selectedJets.erase(it);
                        selectedJet=nullptr;
                        break;
                    }
                    else
                    {
                        dRmin=std::min(dR,dRmin);
                    }
                }
                if (selectedJet)
                {
                    selectedJet->setUserRecord("dRmin",float(dRmin));
                    ++it;
                }
            }
        }
        
        
        
        bool analyse(pxl::Sink *sink) throw (std::runtime_error)
        {
            try
            {
                pxl::Event *event  = dynamic_cast<pxl::Event*>(sink->get());
                if (event)
                {
                    std::vector<pxl::EventView*> eventViews;
                    event->getObjectsOfType(eventViews);
                    
                    std::vector<pxl::Particle*> selectedJets;
                    
                    std::vector<pxl::Particle*> dRCleaningObjects;
                    
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

                                if (particle->getName()==_inputJetName)
                                {
                                    if (passJetSelection(particle))
                                    {
                                        particle->setName(_selectedJetName);
                                        selectedJets.push_back(particle);
                                    } else if (_cleanEvent) {
                                        eventView->removeObject(particle);
                                    }
                                }
                                for (unsigned int iname = 0; iname < _dRObjects.size(); ++iname)
                                {
                                    if (particle->getName()==_dRObjects[iname])
                                    {
                                        dRCleaningObjects.push_back(particle);
                                        break;
                                    }
                                }
                            }
                        }
                        
                        applyDRcleaning(eventView,selectedJets,dRCleaningObjects);
                        
                        switch (selectedJets.size())
                        {
                            case 1:
                                _output1JetsSource->setTargets(event);
                                return _output1JetsSource->processTargets();
                            case 2:
                                _output2JetsSource->setTargets(event);
                                return _output2JetsSource->processTargets();
                            case 3:
                                _output3JetsSource->setTargets(event);
                                return _output3JetsSource->processTargets();
                            case 4:
                                _output4JetsSource->setTargets(event);
                                return _output4JetsSource->processTargets();  
                            default:
                                _outputOtherJetsSource->setTargets(event);
                                return _outputOtherJetsSource->processTargets();
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

PXL_MODULE_INIT(JetSelection)
PXL_PLUGIN_INIT
