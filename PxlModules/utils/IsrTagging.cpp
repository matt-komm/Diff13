#include "pxl/hep.hh"
#include "pxl/core.hh"
#include "pxl/core/macros.hh"
#include "pxl/core/PluginManager.hh"
#include "pxl/modules/Module.hh"
#include "pxl/modules/ModuleFactory.hh"

#include <cmath>
#include <vector>
#include <set>
#include <string>

static pxl::Logger logger("JetSelection");

class IsrTagging:
    public pxl::Module
{
    private:
        pxl::Source* _outputJetSource;

        std::set<std::string> _inputJetNames;
        std::string _inputEventViewName;
        double _etaMinJet;

    public:
        IsrTagging():
            Module(),
            _inputJetNames(),
            _inputEventViewName("Reconstructed"),
            _etaMinJet(4.7)
        {
            addSink("input", "input");
            
            _outputJetSource = addSource("output","output");

            addOption("event view","name of the event view where jets are selected",_inputEventViewName);
            addOption("input jet names","names of particles to consider for tagging",std::vector<std::string>());
            addOption("min eta","minimum eta",_etaMinJet);
        }

        ~IsrTagging()
        {
        }

        // every Module needs a unique type
        static const std::string &getStaticType()
        {
            static std::string type ("IsrTagging");
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
            
            std::vector<std::string> jetNames;
            getOption("input jet names",jetNames);
            for (const std::string& name: jetNames)
            {
                _inputJetNames.insert(name);
            }
            getOption("min eta",_etaMinJet);
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
                            
                            std::vector<pxl::Particle*> selectedJets;

                            for (unsigned iparticle=0; iparticle<particles.size();++iparticle)
                            {
                                pxl::Particle* particle = particles[iparticle];

                                if (_inputJetNames.find(particle->getName())!=_inputJetNames.end())
                                {
                                    selectedJets.push_back(particle);
                                }
                            }
                        
                            if (selectedJets.size()>1)
                            {
                                for (unsigned int ijet = 0; ijet < selectedJets.size(); ++ijet)
                                {
                                    double minPtRatio = 1000000.0;
                                    double maxPtRatio = 0.0000001;
                                    
                                    for (unsigned int jjet = 0; jjet < selectedJets.size(); ++jjet)
                                    {
                                        if (ijet==jjet)
                                        {
                                            continue;
                                        }
                                        minPtRatio=std::min(minPtRatio,selectedJets[ijet]->getPt()/selectedJets[jjet]->getPt());
                                        maxPtRatio=std::max(maxPtRatio,selectedJets[ijet]->getPt()/selectedJets[jjet]->getPt());
                                    }
                                    selectedJets[ijet]->setUserRecord("minPtRatio",minPtRatio);
                                    selectedJets[ijet]->setUserRecord("maxPtRatio",maxPtRatio);
                                }
                            }
                            _outputJetSource->setTargets(event);
                            return _outputJetSource->processTargets();
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

PXL_MODULE_INIT(IsrTagging)
PXL_PLUGIN_INIT
