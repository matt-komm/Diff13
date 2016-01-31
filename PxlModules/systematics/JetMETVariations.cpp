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
        struct Variation
        {
            std::array<pxl::Source*,2> sources;
            std::array<bool,2> active;
            std::string inputJetName;
            std::string inputMETName;
        };
    
        std::string _eventViewName;
    
        std::string _nominalJetName;
        std::string _nominalMETName;
        
        std::string _renamedJet;
        std::string _renamedMET;
        
        bool _removeOtherVariations;
        
        const std::vector<std::string> _variationNames;
        pxl::Source* _nominalSource;
        std::unordered_map<std::string, Variation> _variations;
        
    public:
        JetMETVariations():
            Module(),
            _eventViewName("Reconstructed"),
            _nominalJetName("JetRes"),
            _nominalMETName("METRes"),
            _renamedJet("Jet"),
            _renamedMET("MET"),
            _removeOtherVariations(true),
            _variationNames({"Res","En","UnclEn"})
        {
            addSink("input", "input");
            
            addOption("event view name", "", _eventViewName);
            
            addOption("nominal jet name","",_nominalJetName);
            addOption("nominal MET name","",_nominalMETName);
            
            addOption("rename jet","",_renamedJet);
            addOption("rename MET","",_renamedMET);
            
            addOption("remove other variations","",_removeOtherVariations);

            for (const std::string& name: _variationNames)
            {
                Variation variation;
                variation.sources={addSource(name+"Up",name+"Up"),addSource(name+"Down",name+"Down")};
                
                addOption(name+" jet name","","");
                addOption(name+" met name","","");

                variation.active={true,true};
                
                addOption("variate "+name+"Up","",true);
                addOption("variate "+name+"Down","",true);
                _variations[name]=std::move(variation);
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
            
            getOption("rename jet",_renamedJet);
            getOption("rename MET",_renamedMET);

            getOption("remove other variations",_removeOtherVariations);

            for (auto variation = _variations.begin(); variation!= _variations.end();++variation)
            {
                getOption(variation->first+" jet name",variation->second.inputJetName);
                getOption(variation->first+" met name",variation->second.inputMETName);
                
                getOption("variate "+variation->first+"Up",variation->second.active[0]);
                getOption("variate "+variation->first+"Down",variation->second.active[1]);
            }
        }
        
        
        void renameJetMET(pxl::Event* event, const std::string& jetName, const std::string& metName)
        {
            std::vector<pxl::EventView*> eventViews;
            event->getObjectsOfType(eventViews);
            
            bool metRenamed = false;
            
            pxl::Particle* nominalMET = nullptr;
            
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
                            if (metRenamed)
                            {
                                throw std::runtime_error("Multiple MET objects found of name '"+metName+"'");
                            }
                            metRenamed=true;
                        }
                        else if (particle->getName()==_nominalMETName)
                        {
                            nominalMET = particle;
                        }
                        
                        //remove particle if it begins with _renamedJet
                        else if (_removeOtherVariations and std::equal(_renamedJet.begin(),_renamedJet.end(),particle->getName().begin()))
                        {
                            eventView->removeObject(particle);
                        }
                        //remove particle if it begins with _renamedMET
                        else if (_removeOtherVariations and std::equal(_renamedMET.begin(),_renamedMET.end(),particle->getName().begin()))
                        {
                            eventView->removeObject(particle);
                        }
                        
                    }
                    
                    //need MET in event for neutrino pz
                    if (not metRenamed)
                    {
                        nominalMET->setName(_renamedMET);
                    }
                    else if (nominalMET)
                    {
                        eventView->removeObject(nominalMET);
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
                    for (auto variation = _variations.begin(); variation!= _variations.end();++variation)
                    {
                        if (variation->second.active[0])
                        {
                            pxl::Event* eventShiftedUp = dynamic_cast<pxl::Event*>(event->clone());
                            if (not eventShiftedUp)
                            {
                                throw std::runtime_error("cloning of Event failed");
                            }
                            std::string jetName = variation->second.inputJetName+"Up";
                            if (variation->second.inputJetName=="")
                            {
                                jetName=_nominalJetName;
                            }
                            std::string metName = variation->second.inputMETName+"Up";
                            if (variation->second.inputMETName=="")
                            {
                                metName=_nominalMETName;
                            }
                            renameJetMET(eventShiftedUp,jetName,metName);
                            variation->second.sources[0]->setTargets(eventShiftedUp);
                            success &= variation->second.sources[0]->processTargets();
                            delete eventShiftedUp;
                        }
                        if (variation->second.active[1])
                        {
                            pxl::Event* eventShiftedDown = dynamic_cast<pxl::Event*>(event->clone());
                            if (not eventShiftedDown)
                            {
                                throw std::runtime_error("cloning of Event failed");
                            }
                            std::string jetName = variation->second.inputJetName+"Down";
                            if (variation->second.inputJetName=="")
                            {
                                jetName=_nominalJetName;
                            }
                            std::string metName = variation->second.inputMETName+"Down";
                            if (variation->second.inputMETName=="")
                            {
                                metName=_nominalMETName;
                            }
                            renameJetMET(eventShiftedDown,jetName,metName);
                            variation->second.sources[1]->setTargets(eventShiftedDown);
                            success &= variation->second.sources[1]->processTargets();
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


PXL_MODULE_INIT(JetMETVariations)
PXL_PLUGIN_INIT
