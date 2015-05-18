#include "pxl/hep.hh"
#include "pxl/core.hh"
#include "pxl/core/macros.hh"
#include "pxl/core/PluginManager.hh"
#include "pxl/modules/Module.hh"
#include "pxl/modules/ModuleFactory.hh"

#include <algorithm>

static pxl::Logger logger("TopReconstruction");

struct SortByUserRecord
{
    const std::string urName;
    SortByUserRecord(std::string urName):
        urName(urName)
    {
    }
    
    bool operator()(const pxl::Particle* p1, const pxl::Particle* p2) const
    {
        return p1->getUserRecord(urName).toFloat()>p2->getUserRecord(urName).toFloat();
    }
};

struct SortByPt
{
    bool operator()(const pxl::Particle* p1, const pxl::Particle* p2) const
    {
        return p1->getPt()>p2->getPt();
    }
};

struct SortByEta
{
    bool operator()(const pxl::Particle* p1, const pxl::Particle* p2) const
    {
        return fabs(p1->getEta())>fabs(p2->getEta());
    }
};


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
        
        float angle(const pxl::Basic3Vector& v1, const pxl::Basic3Vector& v2)
        {
            return (v1.getX()*v2.getX()+v1.getY()*v2.getY()+v1.getZ()*v2.getZ())/(v1.getMag()*v2.getMag());
        }
        
        float angleInRestFrame(const pxl::LorentzVector& p1, const pxl::Basic3Vector& boost1, const pxl::LorentzVector& p2, const pxl::Basic3Vector& boost2)
        {
            pxl::LorentzVector boostedP1 = p1;
            boostedP1.boost(-boost1);
            pxl::LorentzVector boostedP2 = p2;
            boostedP2.boost(-boost2);

            return angle(boostedP1,boostedP2);
        }
        
        void calculateAngles(pxl::EventView* eventView, pxl::Particle* lepton, pxl::Particle* neutrino, pxl::Particle* wboson, pxl::Particle* bjet, pxl::Particle* top, pxl::Particle* lightjet)
        {
            if (eventView && lepton && wboson && top)
            {
                //w polarization - helicity basis
                eventView->setUserRecord("cosTheta_wH",angleInRestFrame(lepton->getVector(),wboson->getBoostVector(),top->getVector(),wboson->getBoostVector()));
            }
            if (eventView && lepton && wboson && top && lightjet)
            {
                //w polarization - normal basis
                pxl::Basic3Vector normalAxis = lightjet->getVector().cross(wboson->getVector());
                eventView->setUserRecord("cosTheta_wN",angleInRestFrame(lepton->getVector(),wboson->getBoostVector(),normalAxis,wboson->getBoostVector()));
                //w polarization - transvers basis
                pxl::Basic3Vector transverseAxis = wboson->getVector().cross(normalAxis);
                eventView->setUserRecord("cosTheta_wT",angleInRestFrame(lepton->getVector(),wboson->getBoostVector(),transverseAxis,wboson->getBoostVector()));

                //top polarization - lepton
                eventView->setUserRecord("cosTheta_tPL",angleInRestFrame(lepton->getVector(),top->getBoostVector(),lightjet->getVector(),top->getBoostVector()));
            }
            if (eventView && top && neutrino && bjet && neutrino)
            {
                //top polarization - bjet
                eventView->setUserRecord("cosTheta_tPB",angleInRestFrame(bjet->getVector(),top->getBoostVector(),lightjet->getVector(),top->getBoostVector()));
                //top polarization - neutrino
                eventView->setUserRecord("cosTheta_tPN",angleInRestFrame(neutrino->getVector(),top->getBoostVector(),lightjet->getVector(),top->getBoostVector()));                    
            }
            
        }
        
        pxl::Particle* makeWboson(pxl::EventView* eventView, pxl::Particle* p1, pxl::Particle* p2)
        {
            pxl::Particle* wboson = eventView->create<pxl::Particle>();
            wboson->setName(_wbosonName);
            wboson->linkDaughter(p1);
            wboson->linkDaughter(p2);
            wboson->setP4FromDaughters();
            return wboson;
        }
        
        pxl::Particle* makeTop(pxl::EventView* eventView, pxl::Particle* p1, pxl::Particle* p2)
        {
            pxl::Particle* top = eventView->create<pxl::Particle>();
            top->setName(_topName);
            top->linkDaughter(p1);
            top->linkDaughter(p2);
            top->setP4FromDaughters();
            return top;
        }
        
        pxl::Particle* makeCMSystem(pxl::EventView* eventView, const std::string& name, std::vector<pxl::Particle*> particles)
        {
            pxl::Particle* cm = eventView->create<pxl::Particle>();
            cm->setName(name);
            //linking too much will crash the gui :-(
            for (pxl::Particle* p: particles)
            {
                cm->addP4(p);
            }
            return cm;
        }
        
        void reconstructEvent(pxl::EventView* eventView, pxl::Particle* lepton, pxl::Particle* neutrino, std::vector<pxl::Particle*>& lightjets, std::vector<pxl::Particle*>& bjets) 
        {
            const unsigned int nljets = lightjets.size();
            const unsigned int nbjets = bjets.size();
            const unsigned int njets = nljets+nbjets;
            
            pxl::Particle* wboson = nullptr;
            pxl::Particle* top = nullptr;
            pxl::Particle* lightjet = nullptr;
            pxl::Particle* bjet = nullptr;
            
            if (njets==0)
            {
                wboson = makeWboson(eventView,lepton,neutrino);
            }
            if (njets==1)
            {
                wboson = makeWboson(eventView,lepton,neutrino);
                if (nljets==1)
                {
                    top = makeTop(eventView,wboson,lightjets[0]);
                }
                else if (nbjets==1)
                {
                    top = makeTop(eventView,wboson,bjets[0]);
                }
            }
            else if (njets==2)
            {   
                if (nbjets==0)
                {
                    //take the central jet as the one from the top
                    wboson = makeWboson(eventView,lepton,neutrino);
                    std::sort(lightjets.begin(),lightjets.end(),SortByEta());
                    lightjet=lightjets[0];
                    bjet=lightjets[1];
                    top = makeTop(eventView,wboson,bjet);
                }
                else if (nbjets==1)
                {
                    wboson = makeWboson(eventView,lepton,neutrino);
                    lightjet=lightjets[0];
                    bjet=bjets[0];
                    top = makeTop(eventView,wboson,bjet);
                }
                else if (nbjets==2)
                {
                    //take the jet with the higher pT as the one from the top
                    wboson = makeWboson(eventView,lepton,neutrino);
                    std::sort(bjets.begin(),bjets.end(),SortByPt());
                    lightjet=bjets[1];
                    bjet=bjets[0];
                    top = makeTop(eventView,wboson,bjet);
                }
            }
            else if (njets==3)
            {
                if (nbjets==0)
                {
                    //take the central jet as the one from the top
                    wboson = makeWboson(eventView,lepton,neutrino);
                    std::sort(lightjets.begin(),lightjets.end(),SortByEta());
                    lightjet=lightjets[0];
                    bjet=lightjets[2];
                    top = makeTop(eventView,wboson,bjet);
                }
                else if (nbjets==1)
                {
                    //take the central jet as the one from the top
                    wboson = makeWboson(eventView,lepton,neutrino);
                    std::sort(lightjets.begin(),lightjets.end(),SortByEta());
                    lightjet=lightjets[0];
                    bjet=bjets[0];
                    top = makeTop(eventView,wboson,bjet);
                }
                else if (nbjets==2)
                {
                    //take the jet with the higher pT as the one from the top
                    wboson = makeWboson(eventView,lepton,neutrino);
                    std::sort(bjets.begin(),bjets.end(),SortByPt());
                    lightjet=lightjets[0];
                    bjet=bjets[0];
                    top = makeTop(eventView,wboson,bjet);
                }
                else if (nbjets==3)
                {
                    //take the jet with the higher pT as the one from the top
                    wboson = makeWboson(eventView,lepton,neutrino);
                    std::sort(bjets.begin(),bjets.end(),SortByPt());
                    lightjet=bjets[2];
                    bjet=bjets[0];
                    top = makeTop(eventView,wboson,bjet);
                }
            }
            
            calculateAngles(eventView, lepton, neutrino, wboson, bjet, top, lightjet);
            
            if (top && lightjet)
            {
                makeCMSystem(eventView,"Shat",{{top,lightjet}});
            }
            if (bjet && lightjet)
            {
                pxl::Particle* dijetSystem = makeCMSystem(eventView,"Dijet",{{bjet,lightjet}});
                dijetSystem->setUserRecord("cosTheta",angle(bjet->getVector(),lightjet->getVector()));
                const double y1 = 0.5*std::log((lightjet->getE()+lightjet->getPz())/(lightjet->getE()-lightjet->getPz()));
                const double y2 = 0.5*std::log((bjet->getE()+bjet->getPz())/(bjet->getE()-bjet->getPz()));
                dijetSystem->setUserRecord("chi",std::exp(fabs(y1-y2)));
            }
            if (njets>0)
            {
                std::vector<pxl::Particle*> allHadronic;
                std::copy (lightjets.begin(),lightjets.end(),back_inserter(allHadronic));
                std::copy (bjets.begin(),bjets.end(),back_inserter(allHadronic));
                makeCMSystem(eventView,"Hadronic",allHadronic);
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
                    
                    for (unsigned ieventView=0; ieventView<eventViews.size();++ieventView)
                    {
                        pxl::EventView* eventView = eventViews[ieventView];
                        if (eventView->getName()==_inputEventViewName)
                        {
                            std::vector<pxl::Particle*> particles;
                            eventView->getObjectsOfType(particles);

                            pxl::Particle* lepton = nullptr;
                            pxl::Particle* neutrino = nullptr;
                            std::vector<pxl::Particle*> bjets;
                            std::vector<pxl::Particle*> lightjets;

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
                                if (particle->getName()==_bJetName)
                                {
                                    bjets.push_back(particle);
                                }
                                if (particle->getName()==_lightJetName)
                                {
                                    lightjets.push_back(particle);
                                }
                            }
                            
                            
                            if (lepton && neutrino)
                            {
                                reconstructEvent(eventView,lepton,neutrino,lightjets,bjets);
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
