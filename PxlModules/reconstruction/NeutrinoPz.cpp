#include "pxl/hep.hh"
#include "pxl/core.hh"
#include "pxl/core/macros.hh"
#include "pxl/core/PluginManager.hh"
#include "pxl/modules/Module.hh"
#include "pxl/modules/ModuleFactory.hh"

#include "NeutrinoPzSolver.hpp"

static pxl::Logger logger("NeutrinoPz");

class NeutrinoPz : public pxl::Module
{
    private:
    pxl::Source* _output;

    std::string _inputEventViewName;
    std::string _metName;
    std::string _leptonName;
    std::string _neutrinoName;
    

    public:
    NeutrinoPz() :
        Module(),
        _inputEventViewName("Reconstructed"),
        _metName("MET"),
        _leptonName("TightMuon"),
        _neutrinoName("Neutrino")

    {
        addSink("input", "Input");
        _output = addSource("output", "output");

        addOption("event view","name of the event view",_inputEventViewName);
        addOption("met name","name of the MET",_metName);
        addOption("lepton name","name of the lepton",_leptonName);
        addOption("neutrino name","name of the neutrino",_neutrinoName);

    }

    ~NeutrinoPz()
    {
    }

    // every Module needs a unique type
    static const std::string &getStaticType()
    {
        static std::string type ("NeutrinoPz");
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
        getOption("lepton name",_leptonName);
        getOption("met name",_metName);
        getOption("neutrino name",_neutrinoName);
    }

    void endJob()
    {
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
                for (unsigned ieventView=0; ieventView<eventViews.size();++ieventView)
                {
                    pxl::EventView* eventView = eventViews[ieventView];
                    if (eventView->getName()==_inputEventViewName)
                    {
                        std::vector<pxl::Particle*> particles;
                        eventView->getObjectsOfType(particles);
                        pxl::Particle* met=0;
                        pxl::Particle* lepton=0;
                        pxl::Particle* neutrino=0;
                        for (unsigned iparticle=0; iparticle<particles.size();++iparticle)
                        {
                            pxl::Particle* particle = particles[iparticle];
                            if (met==0 &&(particle->getName()==_metName))
                            {
                                met=particle;
                            }
                            if (lepton==0 &&(particle->getName()==_leptonName))
                            {
                                lepton=particle;
                            }
                        }
                        if (met!=0 && lepton!=0)
                        {
                            neutrino=eventView->create<pxl::Particle>();
                            neutrino->setName(_neutrinoName);
                            solveNu4Momentum(neutrino,lepton->getVector(),met->getPx(),met->getPy());
                            pxl::Particle p1;
                            pxl::Particle p2;
                            p1.getVector()+=met->getVector();
                            p1.getVector()+=lepton->getVector();
                            p2.getVector()+=neutrino->getVector();
                            p2.getVector()+=lepton->getVector();
                            neutrino->setUserRecord("mtw_beforePzSolution",sqrt(p1.getMass()*p1.getMass()+p1.getPx()*p1.getPx()+p1.getPy()*p1.getPy()));
                            neutrino->setUserRecord("mtw_afterPzSolution",sqrt(p2.getMass()*p2.getMass()+p2.getPx()*p2.getPx()+p2.getPy()*p2.getPy()));
                            //neutrino->setUserRecord("wmass_afterPzSolution",p2.getMass());
                        }
                    }
                }

                _output->setTargets(event);
                return _output->processTargets();
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

PXL_MODULE_INIT(NeutrinoPz)
PXL_PLUGIN_INIT

