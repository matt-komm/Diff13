#include "pxl/hep.hh"
#include "pxl/core.hh"
#include "pxl/core/macros.hh"
#include "pxl/core/PluginManager.hh"
#include "pxl/modules/Module.hh"
#include "pxl/modules/ModuleFactory.hh"

#include <string>
#include <vector>
#include <array>
#include <unordered_map>

static pxl::Logger logger("JetMETVariations");

class JetMETVariations:
    public pxl::Module
{
    private:
        std::string _eventViewName;
    
        std::string _nominalJetName;
        std::string _variationJetNamePrefix;
        
        std::string _nominalMETName;
        std::string _variationMETNamePrefix;
        
        std::string _renamedJet;
        std::string _renamedMET;
        
        bool _removeOtherVariations;
        
        const static std::vector<std::string> variations;
        pxl::Source* _nominalSource;
        std::unordered_map<std::string, std::array<pxl::Source*,2>> _sources;
        std::unordered_map<std::string, std::array<bool,2>> _activeVariations;
        
    public:
        JetMETVariations():
            Module(),
            _eventViewName("Reconstructed"),
            _nominalJetName("JetRes"),
            _variationJetNamePrefix("Jet"),
            _nominalMETName("METRes"),
            _variationMETNamePrefix("MET"),
            _renamedJet("Jet"),
            _renamedMET("MET"),
            _removeOtherVariations(true)
        {
            addSink("input", "input");
            
            addOption("event view name", "", _eventViewName);
            
            addOption("nominal jet name","",_nominalJetName);
            addOption("nominal MET name","",_nominalMETName);
            
            addOption("variation jet prefix","",_variationJetNamePrefix);
            addOption("variation MET prefix","",_variationMETNamePrefix);
            
            addOption("rename jet","",_renamedJet);
            addOption("rename MET","",_renamedMET);
            
            addOption("remove other variations","",_removeOtherVariations);

            for (const std::string& variation: variations)
            {
                _sources[variation]={addSource(variation+"Up",variation+"Up"),addSource(variation+"Down",variation+"Down")};
                _activeVariations[variation]={true,true};
                
                addOption("variate "+variation+"Up","",_activeVariations[variation][0]);
                addOption("variate "+variation+"Down","",_activeVariations[variation][1]);
            }
            
            _nominalSource = addSource("nominal","nominal");
            
            
            
        }

        ~JetMETVariations()
        {
        }

        // every Module needs a unique type
        static const std::string &getStaticType()
        {
            static std::string type ("JetMETVariations");
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
            getOption("event view name", _eventViewName);
        
            getOption("nominal jet name",_nominalJetName);
            getOption("nominal MET name",_nominalMETName);

            getOption("variation jet prefix",_variationJetNamePrefix);
            getOption("variation MET prefix",_variationMETNamePrefix);
            
            getOption("rename jet",_renamedJet);
            getOption("rename MET",_renamedMET);

            getOption("remove other variations",_removeOtherVariations);

            for (const std::string& variation: variations)
            {
                getOption("variate "+variation+"Up",_activeVariations[variation][0]);
                getOption("variate "+variation+"Down",_activeVariations[variation][1]);
            }
        }
        
        
        void renameJetMET(pxl::Event* event, const std::string& jetName, const std::string& metName)
        {
            std::vector<pxl::EventView*> eventViews;
            event->getObjectsOfType(eventViews);
            
            for (pxl::EventView* eventView: eventViews)
            {
                if (eventView->getName()==_eventViewName)
                {
                    std::vector<pxl::Particle*> particles;
                    eventView->getObjectsOfType(particles);
                    for (pxl::Particle* particle: particles)
                    {
                        if (particle->getName()==jetName)
                        {
                            particle->setName(_renamedJet);
                        }
                        else if (particle->getName()==metName)
                        {
                            particle->setName(_renamedMET);
                        }
                        else if (_removeOtherVariations and std::equal(_renamedJet.begin(),_renamedJet.end(),particle->getName().begin()))
                        {
                            eventView->removeObject(particle);
                        }
                        else if (_removeOtherVariations and std::equal(_renamedMET.begin(),_renamedMET.end(),particle->getName().begin()))
                        {
                            eventView->removeObject(particle);
                        }
                    }
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
                    bool success = true;
                    for (const std::string& variation: variations)
                    {
                        if (_activeVariations[variation][0])
                        {
                            pxl::Event* eventShiftedUp = dynamic_cast<pxl::Event*>(event->clone());
                            if (not eventShiftedUp)
                            {
                                throw std::runtime_error("cloning of Event failed");
                            }
                            renameJetMET(eventShiftedUp,_variationJetNamePrefix+variation+"Up",_variationMETNamePrefix+variation+"Up");
                            _sources[variation][0]->setTargets(eventShiftedUp);
                            success &= _sources[variation][0]->processTargets();
                            delete eventShiftedUp;
                        }
                        if (_activeVariations[variation][1])
                        {
                            pxl::Event* eventShiftedDown = dynamic_cast<pxl::Event*>(event->clone());
                            if (not eventShiftedDown)
                            {
                                throw std::runtime_error("cloning of Event failed");
                            }
                            renameJetMET(eventShiftedDown,_variationJetNamePrefix+variation+"Down",_variationMETNamePrefix+variation+"Down");
                            _sources[variation][1]->setTargets(eventShiftedDown);
                            success &= _sources[variation][1]->processTargets();
                            delete eventShiftedDown;
                        }
                    }
                    
                    renameJetMET(event,_nominalJetName,_nominalMETName);
                    _nominalSource->setTargets(event);
                    success &= _nominalSource->processTargets();
                    
                    return success;
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
        
        void endJob() throw (std::runtime_error)
        {
        }

        void shutdown() throw(std::runtime_error)
        {
        }

        void destroy() throw (std::runtime_error)
        {
            delete this;
        }
};

const std::vector<std::string> JetMETVariations::variations = {
"Res",
"En"
};

PXL_MODULE_INIT(JetMETVariations)
PXL_PLUGIN_INIT
