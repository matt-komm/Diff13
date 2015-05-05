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
        pxl::Source* _output0JetSource;
        pxl::Source* _output1JetSource;
        pxl::Source* _output2JetsSource;
        pxl::Source* _output3JetsSource;
        pxl::Source* _output4JetsSource;
        pxl::Source* _outputOtherNJetsSource;

        std::string _inputJetName;
        std::string _inputEventViewName;
        std::string _selectedJetName;
        
        bool _cleanEvent;

        double _pTMinJet; //Minimum transverse energy
        double _etaMaxJet; //Maximum pseudorapidity

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
            _pTMinJet(40.),
            _etaMaxJet(4.7),

            _dRInvert(false),
            _dR(0.3),
            _dRObjects({"TightMuon","TightElectron"})
            /*Initial Values taken from TOP JetMET Analysis (Run2) */
            /*https://twiki.cern.ch/twiki/bin/view/CMS/TopJME#General_Information */
        {
            addSink("input", "input");



            _outputOtherNJetsSource = addSource(">4 Jets", ">4 Jets");
            _output4JetsSource = addSource("4 Jets", "4 Jets");
            _output3JetsSource = addSource("3 Jets", "3 Jets");
            _output2JetsSource = addSource("2 Jets", "2 Jets");
            _output1JetSource = addSource("1 Jet", "1 Jet");
            _output0JetSource = addSource("0 Jets", "0 Jets");

            addOption("event view","name of the event view where jets are selected",_inputEventViewName);
            addOption("input jet name","name of particles to consider for selection",_inputJetName);
            addOption("name of selected jets","",_selectedJetName);
            addOption("clean event","this option will clean the event of all jets falling cuts",_cleanEvent);

            addOption("PF Jet Minimum pT","",_pTMinJet);
            addOption("PF Jet Maximum Eta","",_etaMaxJet);

            addOption("invert dR","inverts dR cleaning",_dRInvert);
            addOption("dR cut","remove jets close to other objects, e.g. leptons",_dR);
            addOption("dR objects","object names to which the jets should NOT be close to",_dRObjects);
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

            getOption("PF Jet Minimum pT",_pTMinJet);
            getOption("PF Jet Maximum Eta",_etaMaxJet);

            getOption("invert dR",_dRInvert);
            getOption("dR cut",_dR);
            getOption("dR objects",_dRObjects);
            if (_dRObjects.size()==0)
            {
                _dR=-1;
            }

        }

        bool passesJetSelection(pxl::Particle* particle)
        {
            if (not (particle->getPt()>_pTMinJet))
            {
                return false;
            }
            if (not (fabs(particle->getEta())<_etaMaxJet))
            {
                return false;
            }
           
            if (particle->hasUserRecord("neutralHadronEnergyFraction"))
            {
                if (not (particle->getUserRecord("neutralHadronEnergyFraction").toFloat()<0.99))
                {
                    return false;
                }
            }
            
            if (particle->hasUserRecord("neutralEmEnergyFraction"))
            {
                if (not (particle->getUserRecord("neutralEmEnergyFraction").toFloat()<0.99))
                {
                    return false;
                }
            }

            if (not (particle->getUserRecord("nConstituents").toInt32()>1))
            {
                return false;
            }
            
            if (particle->hasUserRecord("muonEnergyFraction"))
            {
                if (not (particle->getUserRecord("muonEnergyFraction").toFloat()<0.8))
                {
                    return false;
                }
            }

            //additional selection if jet is central
            if (fabs(particle->getEta())<2.4)
            {
                if (particle->hasUserRecord("chargedHadronEnergyFraction"))
                {
                    if (not (particle->getUserRecord("chargedHadronEnergyFraction").toFloat()>0))
                    {
                        return false;
                    }
                }
                if (not (particle->getUserRecord("chargedMultiplicity").toFloat()>0))
                {
                    return false;
                }
                
                //same as 'chargedEmEnergy'
                if (particle->hasUserRecord("electronEnergyFraction"))
                {
                    if (not (particle->getUserRecord("electronEnergyFraction").toFloat()<0.99))
                    {
                        return false;
                    }
                }
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
                                    if (passesJetSelection(particle))
                                    {
                                        particle->setName(_selectedJetName);
                                        selectedJets.push_back(particle);
                                    }
                                    else if (_cleanEvent)
                                    {
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
                    }

                    switch (selectedJets.size())
                    {
                        case 0:
                            _output0JetSource->setTargets(event);
                            return _output0JetSource->processTargets();
                        case 1:
                            _output1JetSource->setTargets(event);
                            return _output1JetSource->processTargets();
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
                            _outputOtherNJetsSource->setTargets(event);
                            return _outputOtherNJetsSource->processTargets();
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
