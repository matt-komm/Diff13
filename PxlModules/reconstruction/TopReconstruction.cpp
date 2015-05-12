#include "pxl/hep.hh"
#include "pxl/core.hh"
#include "pxl/core/macros.hh"
#include "pxl/core/PluginManager.hh"
#include "pxl/modules/Module.hh"
#include "pxl/modules/ModuleFactory.hh"

static pxl::Logger logger("TopReconstruction");

class TopReconstruction:
    public pxl::Module
{
    private:
        pxl::Source* _outputSource;
        
        std::string _inputEventViewName;
        std::string _leptonName;
        std::string _neutrinoName;
        std::string _bJetName;
        std::string _lightJetName;
        
        std::string _wbosonName;
        std::string _topName;

        bool _addBestTopHypothesis;
        
    public:
        TopReconstruction():
            Module(),
            _inputEventViewName("Reconstructed"),
            _leptonName("TightMuon"),
            _neutrinoName("Neutrino"),
            _bJetName("SelectedBJet"),
            _lightJetName("SelectedJet"),
            _wbosonName("W"),
            _topName("Top"),
            _addBestTopHypothesis(true)
        {
            addSink("input", "input");
            _outputSource = addSource("selected","selected");

            addOption("event view","name of the event view",_inputEventViewName);
            addOption("lepton","name of the lepton from top decay",_leptonName);
            addOption("neutrino","name of the neutrino from top decay",_neutrinoName);
            addOption("b-jet","name of the b-jet from top decay",_bJetName);
            addOption("light jet","name of the light jet from top decay (optional)",_lightJetName);
            
            addOption("W boson","name of the reconstructed W boson",_wbosonName);
            addOption("top","name of the reconstructed top",_topName);

            addOption("add bestTop","includes a best top candidate",_addBestTopHypothesis);
        }

        ~TopReconstruction()
        {
        }

        // every Module needs a unique type
        static const std::string &getStaticType()
        {
            static std::string type ("TopReconstruction");
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
            getOption("lepton",_leptonName);
            getOption("neutrino",_neutrinoName);
            getOption("b-jet",_bJetName);
            getOption("light jet",_lightJetName);
            getOption("W boson",_wbosonName);
            getOption("top",_topName);

            getOption("add bestTop",_addBestTopHypothesis);
        }
        
        float angleInRestFrame(const pxl::LorentzVector& p1, const pxl::Basic3Vector& boost1, const pxl::LorentzVector& p2, const pxl::Basic3Vector& boost2)
        {
            pxl::LorentzVector boostedP1 = p1;
            boostedP1.boost(-boost1);
            pxl::LorentzVector boostedP2 = p2;
            boostedP2.boost(-boost2);

            return (boostedP1.getPx()*boostedP2.getPx()+boostedP1.getPy()*boostedP2.getPy()+boostedP1.getPz()*boostedP2.getPz())/(boostedP1.getMag()*boostedP2.getMag());
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
                    
                    for (unsigned ieventView=0; ieventView<eventViews.size();++ieventView)
                    {
                        pxl::EventView* eventView = eventViews[ieventView];
                        if (eventView->getName()==_inputEventViewName)
                        {
                            std::vector<pxl::Particle*> particles;
                            eventView->getObjectsOfType(particles);

                            pxl::Particle* lepton = nullptr;
                            pxl::Particle* neutrino = nullptr;
                            pxl::Particle* bjet = nullptr;
                            pxl::Particle* lightjet = nullptr;

                            for (unsigned int iparticle = 0; iparticle<particles.size(); ++iparticle)
                            {
                                pxl::Particle* particle = particles[iparticle];
                                if (!lepton and particle->getName()==_leptonName)
                                {
                                    lepton=particle;
                                }
                                if (!neutrino and particle->getName()==_neutrinoName)
                                {
                                    neutrino=particle;
                                }
                                if (!bjet and particle->getName()==_bJetName)
                                {
                                    bjet=particle;
                                }
                                if (!lightjet and particle->getName()==_lightJetName)
                                {
                                    lightjet=particle;
                                }
                            }
                            if (lepton && neutrino)
                            {
                                pxl::Particle* wboson = eventView->create<pxl::Particle>();
                                wboson->setName(_wbosonName);
                                wboson->addP4(lepton);
                                wboson->addP4(neutrino);

                                if (bjet)
                                {
                                    pxl::Particle* top = eventView->create<pxl::Particle>();
                                    top->setName(_topName);
                                    top->addP4(lepton);
                                    top->addP4(bjet);
                                    top->addP4(neutrino);
                                    pxl::Basic3Vector normalAxis = lightjet->getVector().cross(wboson->getVector());
                                    pxl::Basic3Vector transverseAxis = wboson->getVector().cross(normalAxis);
                                    //top polarization
                                    eventView->setUserRecord("cosTheta_tP",angleInRestFrame(lepton->getVector(),top->getBoostVector(),lightjet->getVector(),top->getBoostVector()));
                                    //w polarization - helicity basis
                                    eventView->setUserRecord("cosTheta_wH",angleInRestFrame(lepton->getVector(),wboson->getBoostVector(),-top->getVector(),wboson->getBoostVector()));
                                    //w polarization - normal basis
                                    eventView->setUserRecord("cosTheta_wN",angleInRestFrame(lepton->getVector(),wboson->getBoostVector(),normalAxis,wboson->getBoostVector()));
                                    //w polarization - transvers basis
                                    eventView->setUserRecord("cosTheta_wT",angleInRestFrame(lepton->getVector(),wboson->getBoostVector(),transverseAxis,wboson->getBoostVector()));
                                                                        
                                    if (lightjet && _addBestTopHypothesis)
                                    {
                                        pxl::Particle* bestTop = eventView->create<pxl::Particle>();
                                        bestTop->setName(_topName+"_best");
                                        bestTop->addP4(lepton);
                                        bestTop->addP4(lightjet);
                                        bestTop->addP4(neutrino);
                                        if (fabs(top->getMass()-173.0)<fabs(bestTop->getMass()-173.0))
                                        {
                                            bestTop->setP4(top->getVector());
                                        }
                                    }
                                }
                            }
                        }
                    }
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

PXL_MODULE_INIT(TopReconstruction)
PXL_PLUGIN_INIT
