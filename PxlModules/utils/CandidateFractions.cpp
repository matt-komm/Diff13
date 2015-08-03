#include "pxl/hep.hh"
#include "pxl/core.hh"
#include "pxl/core/macros.hh"
#include "pxl/core/PluginManager.hh"
#include "pxl/modules/Module.hh"
#include "pxl/modules/ModuleFactory.hh"

#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iostream>

static pxl::Logger logger("CandidateFractions");


class CandidateFractions:
    public pxl::Module
{
    private:
        pxl::Source* _outputSource;

        std::string _eventViewName;
        std::string _prefix;
        std::vector<std::string> _candidateNames;
        
        bool _hardPVLink;
        bool _noHardPVLink;
        
        

    public:
        CandidateFractions():
            Module(),
            _eventViewName("Candidates"),
            _candidateNames{{"NC"}},
            _hardPVLink(true),
            _noHardPVLink(true)
        {
            addSink("input", "input");
            _outputSource = addSource("output","output");
            
            addOption("event view name","",_eventViewName);
            addOption("candidate names", "", _candidateNames);
            
            
            addOption("hard PV link", "", _hardPVLink);
            addOption("no hard PV link", "", _noHardPVLink);
            
            addOption("prefix", "", _prefix);
        }

        ~CandidateFractions()
        {
        }

        // every Module needs a unique type
        static const std::string &getStaticType()
        {
            static std::string type ("CandidateFractions");
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
            getOption("event view name",_eventViewName);
            getOption("candidate names", _candidateNames);
            
            
            getOption("hard PV link", _hardPVLink);
            getOption("no hard PV link", _noHardPVLink);
            
            getOption("prefix", _prefix);
        }
        
        void calculate(pxl::EventView* eventView, unsigned int key, const std::vector<const pxl::Particle*>& bin)
        {
            std::string name = _prefix+"_"+std::to_string(key)+"_";

            pxl::LorentzVector v(0,0,0,0);
            for (const pxl::Particle* p: bin)
            {
                v+=p->getVector();
            }
            
            eventView->setUserRecord(name+"n",(int)bin.size());
            eventView->setUserRecord(name+"pt",v.getPt());
            eventView->setUserRecord(name+"eta",v.getEta());
            eventView->setUserRecord(name+"phi",v.getPhi());
        }
        
        void makeBinsAndCalculate(pxl::EventView* eventView, const std::vector<const pxl::Particle*>& particles)
        {
            
            std::vector<double> ptBins{{0.0,0.8,1.0,1.2,1.5,2.0,10.0,100.0,5000.0}};
            std::vector<double> etaBins{{-5.5,-4.5,-4.0,-3.0,-2.4,-1.2,0.0,1.2,2.4,3.0,4.0,4.5,5.5}};
            std::vector<double> phiBins{{-3.2,-1.4,0.0,1.4,3.2}};
            
            /*std::vector<double> ptBins{{0.0,0.8,1.0,1.5,3.0,10.0,100.0,5000.0}};
            std::vector<double> etaBins;
            std::vector<double> phiBins;
            for (unsigned int i=0; i<25;++i)
            {
                phiBins.push_back(-3.2+0.01*i*6.4);
            }
            for (unsigned int i=0; i<23;++i)
            {
                etaBins.push_back(-5.5+0.01*i*11.0);
            }
            */
            std::unordered_map<unsigned int,std::vector<const pxl::Particle*>> mapping;
            
            for (const pxl::Particle* particle: particles)
            {
                const double pt = particle->getPt();
                const double eta = particle->getEta();
                const double phi = particle->getPhi();
                
                for (unsigned int ipt = 0; ipt<(ptBins.size()-1); ++ipt)
                {
                    const double ptmin=ptBins[ipt];
                    const double ptmax=ptBins[ipt+1];
                    if (not (pt>ptmin and pt<ptmax))
                    {
                        continue;
                    }
                    for (unsigned int ieta = 0; ieta<(etaBins.size()-1); ++ieta)
                    {
                        const double etamin=etaBins[ieta];
                        const double etamax=etaBins[ieta+1];
                        if (not (eta>etamin and eta<etamax))
                        {
                            continue;
                        }
                        for (unsigned int iphi = 0; iphi<(phiBins.size()-1); ++iphi)
                        {
                            const double phimin=phiBins[iphi];
                            const double phimax=phiBins[iphi+1];
                            if (not (phi>phimin and phi<phimax))
                            {
                                continue;
                            }
                            unsigned int key = ipt*1000*1000+ieta*1000+iphi;
                            mapping[key].push_back(particle);
                        }
                    }
                }
            }
            
            for (auto bin: mapping)
            {
                unsigned int t = bin.first;
                const std::vector<const pxl::Particle*>& particles = bin.second;
                calculate(eventView,t,particles);
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
                    for (pxl::EventView* eventView: eventViews)
                    {
                        if (eventView->getName()!=_eventViewName)
                        {
                            continue;
                        }
                        std::vector<pxl::Particle*> particles;
                        eventView->getObjectsOfType(particles);
                        
                        std::vector<pxl::Vertex*> vertices;
                        eventView->getObjectsOfType(vertices);
                        const pxl::Vertex* primaryVertex = nullptr;
                        for (pxl::Vertex* vertex: vertices)
                        {
                            if (primaryVertex)
                            {
                                if (vertex->numberOfDaughters()>primaryVertex->numberOfDaughters())
                                {
                                    primaryVertex=vertex;
                                }
                            }
                            else
                            {
                                primaryVertex=vertex;
                            }
                        }
                        
                        
                        std::vector<const pxl::Particle*> selectedParticles;
                        for (const pxl::Particle* particle: particles)
                        {
                            if (std::find(_candidateNames.cbegin(), _candidateNames.cend(),particle->getName())!=_candidateNames.cend())
                            {
                                if ((_hardPVLink and particle->getMother()==primaryVertex) or (_noHardPVLink and particle->getMother()!=primaryVertex))
                                {
                                    selectedParticles.push_back(particle);
                                }
                            }
                        }
                        makeBinsAndCalculate(eventView,selectedParticles);
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

PXL_MODULE_INIT(CandidateFractions)
PXL_PLUGIN_INIT
