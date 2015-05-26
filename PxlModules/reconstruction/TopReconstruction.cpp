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
        
        std::string _inputEventViewNameLepton;
        std::string _leptonName;
        
        std::string _inputEventViewNameNeutrino;
        std::string _neutrinoName;
        
        std::string _inputEventViewNameJets;
        std::string _bJetName;
        std::string _lightJetName;
        
        std::string _outputEventViewName;
        std::string _wbosonName;
        std::string _topName;
        


        
    public:
        TopReconstruction():
            Module(),
            _inputEventViewNameLepton("Reconstructed"),
            _leptonName("TightMuon"),
            
            _inputEventViewNameNeutrino("SingleTop"),
            _neutrinoName("Neutrino"),
            
            _inputEventViewNameJets("Reconstructed"),
            _bJetName("SelectedBJet"),
            _lightJetName("SelectedJet"),
            
            _outputEventViewName("SingleTop"),
            _wbosonName("W"),
            _topName("Top")
        {
            addSink("input", "input");
            _outputSource = addSource("selected","selected");

            addOption("input event view lepton","",_inputEventViewNameLepton);
            addOption("lepton","",_leptonName);
            
            addOption("input event view neutrino","",_inputEventViewNameNeutrino);
            addOption("neutrino","",_neutrinoName);
            
            addOption("input event view jets","",_inputEventViewNameJets);
            addOption("b-jet","",_bJetName);   
            addOption("light jet","",_lightJetName);
            
            addOption("output event view","",_outputEventViewName);
            addOption("W boson","",_wbosonName);
            addOption("top","",_topName);


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
            getOption("input event view lepton",_inputEventViewNameLepton);
            getOption("lepton",_leptonName);
            
            getOption("input event view neutrino",_inputEventViewNameNeutrino);
            getOption("neutrino",_neutrinoName);
            
            getOption("input event view jets",_inputEventViewNameJets);
            getOption("b-jet",_bJetName);   
            getOption("light jet",_lightJetName);
            
            getOption("output event view",_outputEventViewName);
            getOption("W boson",_wbosonName);
            getOption("top",_topName);

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
            if (particles.size()>=2)
            {
                float minCosTheta = 100;
                float maxCosTheta = -100;
                float minDY = 100;
                float maxDY = -100;
                float minDEta = 100;
                float maxDEta = -100;
                float minDR = 100;
                float maxDR = -100;
                float minDPhi = 100;
                float maxDPhi = -100;
                for (unsigned int i = 0; i < particles.size(); ++i)
                {
                    for (unsigned int j = i+1; j < particles.size(); ++j)
                    {
                        const pxl::Particle* p1 = particles[i];
                        const pxl::Particle* p2 = particles[j];
                        
                        const float cosTheta = angle(p1->getVector(),p2->getVector());
                        minCosTheta=std::min(minCosTheta,cosTheta);
                        maxCosTheta=std::max(maxCosTheta,cosTheta);
                        
                        const float y1 = 0.5*std::log((p1->getE()+p1->getPz())/(p1->getE()-p1->getPz()));
                        const float y2 = 0.5*std::log((p2->getE()+p2->getPz())/(p2->getE()-p2->getPz()));
                        const float deltaY = fabs(y1-y2);
                        minDY=std::min(minDY,deltaY);
                        maxDY=std::max(maxDY,deltaY);
                        
                        const float deltaEta = fabs(p1->getEta()-p2->getEta());
                        minDEta=std::min(minDEta,deltaEta);
                        maxDEta=std::max(maxDEta,deltaEta);
                        
                        const float deltaR = p1->getVector().deltaR(&(p2->getVector()));
                        minDR=std::min(minDR,deltaR);
                        maxDR=std::max(maxDR,deltaR);
                        
                        const float deltaPhi = p1->getVector().deltaPhi(&(p2->getVector()));
                        minDPhi=std::min(minDPhi,deltaPhi);
                        maxDPhi=std::max(maxDPhi,deltaPhi);
                    }
                }
                if (particles.size()>2)
                {
                    cm->setUserRecord("minCosTheta",minCosTheta);
                    cm->setUserRecord("maxCosTheta",maxCosTheta);
                    
                    cm->setUserRecord("minDY",minDY);
                    cm->setUserRecord("maxDY",maxDY);
                    
                    cm->setUserRecord("minDEta",minDEta);
                    cm->setUserRecord("maxDEta",maxDEta);
                    
                    cm->setUserRecord("minDR",minDR);
                    cm->setUserRecord("maxDR",maxDR);
                    
                    cm->setUserRecord("minDPhi",minDPhi);
                    cm->setUserRecord("maxDPhi",maxDPhi);
                }
                else if (particles.size()==2)
                {
                    cm->setUserRecord("CosTheta",minCosTheta);
                    cm->setUserRecord("DY",minDY);
                    cm->setUserRecord("DEta",minDEta);
                    cm->setUserRecord("DR",minDR);
                    cm->setUserRecord("DPhi",minDPhi);
                }
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
                    lightjet=(pxl::Particle*)lightjets[0]->clone();
                    bjet=(pxl::Particle*)lightjets[1]->clone();
                    eventView->insertObject(lightjet);
                    eventView->insertObject(bjet);
                    top = makeTop(eventView,wboson,bjet);
                }
                else if (nbjets==1)
                {
                    wboson = makeWboson(eventView,lepton,neutrino);
                    lightjet=(pxl::Particle*)lightjets[0]->clone();
                    bjet=(pxl::Particle*)bjets[0]->clone();
                    eventView->insertObject(lightjet);
                    eventView->insertObject(bjet);
                    top = makeTop(eventView,wboson,bjet);
                }
                else if (nbjets==2)
                {
                    //take the jet with the higher pT as the one from the top
                    wboson = makeWboson(eventView,lepton,neutrino);
                    std::sort(bjets.begin(),bjets.end(),SortByPt());
                    lightjet=(pxl::Particle*)bjets[1]->clone();
                    bjet=(pxl::Particle*)bjets[0]->clone();
                    eventView->insertObject(lightjet);
                    eventView->insertObject(bjet);
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
                    lightjet=(pxl::Particle*)lightjets[0]->clone();
                    bjet=(pxl::Particle*)lightjets[2]->clone();
                    eventView->insertObject(lightjet);
                    eventView->insertObject(bjet);
                    top = makeTop(eventView,wboson,bjet);
                }
                else if (nbjets==1)
                {
                    //take the central jet as the one from the top
                    wboson = makeWboson(eventView,lepton,neutrino);
                    std::sort(lightjets.begin(),lightjets.end(),SortByEta());
                    lightjet=(pxl::Particle*)lightjets[0]->clone();
                    bjet=(pxl::Particle*)bjets[0]->clone();
                    eventView->insertObject(lightjet);
                    eventView->insertObject(bjet);
                    top = makeTop(eventView,wboson,bjet);
                }
                else if (nbjets==2)
                {
                    //take the jet with the higher pT as the one from the top
                    wboson = makeWboson(eventView,lepton,neutrino);
                    std::sort(bjets.begin(),bjets.end(),SortByPt());
                    lightjet=(pxl::Particle*)lightjets[0]->clone();
                    bjet=(pxl::Particle*)bjets[0]->clone();
                    eventView->insertObject(lightjet);
                    eventView->insertObject(bjet);
                    top = makeTop(eventView,wboson,bjet);
                }
                else if (nbjets==3)
                {
                    //take the jet with the higher pT as the one from the top
                    wboson = makeWboson(eventView,lepton,neutrino);
                    std::sort(bjets.begin(),bjets.end(),SortByPt());
                    lightjet=(pxl::Particle*)bjets[2]->clone();
                    bjet=(pxl::Particle*)bjets[0]->clone();
                    eventView->insertObject(lightjet);
                    eventView->insertObject(bjet);
                    top = makeTop(eventView,wboson,bjet);
                }
            }
            if (lightjet)
            {
                lightjet->setName("LightJet");
            }
            if (bjet)
            {
                bjet->setName("BJet");
            }
            calculateAngles(eventView, lepton, neutrino, wboson, bjet, top, lightjet);
            
            if (bjet && lightjet)
            {
                makeCMSystem(eventView,"Dijet",{{bjet,lightjet}});
                makeCMSystem(eventView,"Shat",{{bjet,lightjet,lepton,neutrino}});
            }
            /*
            if (njets>0)
            {
                std::vector<pxl::Particle*> allHadronic;
                std::copy (lightjets.begin(),lightjets.end(),back_inserter(allHadronic));
                std::copy (bjets.begin(),bjets.end(),back_inserter(allHadronic));
                makeCMSystem(eventView,"Hadronic",allHadronic);
            }
            */
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
                    
                    pxl::Particle* lepton = nullptr;
                    pxl::Particle* neutrino = nullptr;
                    std::vector<pxl::Particle*> bjets;
                    std::vector<pxl::Particle*> lightjets;
            
                    pxl::EventView* outputEventView = nullptr;
                    for (unsigned ieventView=0; ieventView<eventViews.size();++ieventView)
                    {
                        if (eventViews[ieventView]->getName()==_outputEventViewName)
                        {
                            outputEventView=eventViews[ieventView];
                            break;
                        }
                    }
                    if (!outputEventView)
                    {
                        outputEventView = event->create<pxl::EventView>();
                        outputEventView->setName(_outputEventViewName);
                    }
                    
                    for (unsigned ieventView=0; ieventView<eventViews.size();++ieventView)
                    {
                        pxl::EventView* inputEventView = eventViews[ieventView];
                        
                        
                            
                        std::vector<pxl::Particle*> particles;
                        inputEventView->getObjectsOfType(particles);

                        for (unsigned int iparticle = 0; iparticle<particles.size(); ++iparticle)
                        {
                            pxl::Particle* particle = particles[iparticle];
                            if (!lepton and inputEventView->getName()==_inputEventViewNameLepton and particle->getName()==_leptonName)
                            {
                                if (_inputEventViewNameLepton!=_outputEventViewName)
                                {
                                    lepton=(pxl::Particle*)particle->clone();
                                    outputEventView->insertObject(lepton);
                                }
                                else
                                {
                                    lepton=particle;
                                }
                            }
                            if (!neutrino and inputEventView->getName()==_inputEventViewNameNeutrino and particle->getName()==_neutrinoName)
                            {
                                if (_inputEventViewNameNeutrino!=_outputEventViewName)
                                {
                                    neutrino=(pxl::Particle*)particle->clone();
                                    outputEventView->insertObject(neutrino);
                                }
                                else
                                {
                                    neutrino=particle;
                                }
                            }
                            if (inputEventView->getName()==_inputEventViewNameJets)
                            {
                                if ( particle->getName()==_bJetName)
                                {
                                    bjets.push_back(particle);
                                }
                                else if (particle->getName()==_lightJetName)
                                {
                                    lightjets.push_back(particle);
                                }
                            }
                        }
                    }
                    
                    if (lepton && neutrino)
                    {
                        reconstructEvent(outputEventView,lepton,neutrino,lightjets,bjets);
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
