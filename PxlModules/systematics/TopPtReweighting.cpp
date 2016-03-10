#include "pxl/hep.hh"
#include "pxl/core.hh"
#include "pxl/core/macros.hh"
#include "pxl/core/PluginManager.hh"
#include "pxl/modules/Module.hh"
#include "pxl/modules/ModuleFactory.hh"

#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <cstdlib>
#include <regex>

#include "TH2.h"
#include "TFile.h"

static pxl::Logger logger("TopPtReweighting");

class TopPtReweighting:
    public pxl::Module
{
    private:
        
    
        std::vector<std::regex> _processNames;
        
        pxl::Source* _outputSource;
        

    public:
        TopPtReweighting():
            Module(),
            _processNames()
        {
            addSink("input", "input");
            
            addOption("processNames", "", std::vector<std::string>({"TT_[A-Za-z0-9_\\-]*"}));
            
            _outputSource = addSource("output","output");

        }

        ~TopPtReweighting()
        {
        }

        // every Module needs a unique type
        static const std::string &getStaticType()
        {
            static std::string type ("TopPtReweighting");
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
            std::vector<std::string> strNames;
            getOption("processNames", strNames);
            for (const std::string& name: strNames)
            {
                _processNames.emplace_back(name);
            }

        }
        
        std::unordered_map<std::string,bool> resolveFlags(pxl::Particle* particle)
        {
            unsigned int statusBits = particle->getUserRecord("statusBits");
            const static std::vector<std::string> mask = {
                "IsPrompt", 
                "IsDecayedLeptonHadron", 
                "IsTauDecayProduct", 
                "IsPromptTauDecayProduct",
                "IsDirectTauDecayProduct", 
                "IsDirectPromptTauDecayProduct", 
                "IsDirectHadronDecayProduct", 
                "IsHardProcess",
                "FromHardProcess", 
                "IsHardProcessTauDecayProduct", 
                "IsDirectHardProcessTauDecayProduct", 
                "FromHardProcessBeforeFSR",
                "IsFirstCopy", 
                "IsLastCopy", 
                "IsLastCopyBeforeFSR"
            };
            
            unsigned int power = 1;
            
            std::unordered_map<std::string,bool> results;
            for (unsigned int imask = 0; imask < mask.size(); ++imask)
            {
                bool result = (statusBits & power) == power;
                results[mask[imask]]=result;
                particle->setUserRecord(mask[imask],result);
                power <<= 1;
            }
            return std::move(results);
        }
        
        double getSF(double pt)
        {
            return std::exp(0.199-0.00166*pt);
        }
        
        bool analyse(pxl::Sink *sink) throw (std::runtime_error)
        {
            
            try
            {
                pxl::Event *event  = dynamic_cast<pxl::Event*>(sink->get());
                if (event)
                {
                    std::string processName = event->getUserRecord("ProcessName");
                    bool isSelected = false;
                    for (const std::regex& processMatch: _processNames)
                    {
                        if (std::regex_match(processName,processMatch))
                        {
                            isSelected=true;
                            break;
                        }
                    }
                    if (not isSelected)
                    {
                        _outputSource->setTargets(event);
                        return _outputSource->processTargets();
                    }
                
                    std::vector<pxl::EventView*> eventViews;
                    event->getObjectsOfType(eventViews);

                    for (pxl::EventView* eventView: eventViews)
                    {
                        if (eventView->getName()=="Generated")
                        {
                            std::vector<pxl::Particle*> particles;
                            eventView->getObjectsOfType(particles);
                            
                            std::vector<pxl::Particle*> topQuarks;
                            
                            for (pxl::Particle* particle: particles)
                            {
                                if (abs(particle->getPdgNumber())==6)
                                {
                                    std::unordered_map<std::string,bool> flags = resolveFlags(particle);
                                    if (flags["IsLastCopy"])
                                    {
                                        topQuarks.push_back(particle);
                                    }
                                }
                            }
                            
                            if (topQuarks.size()!=2)
                            {
                                throw std::runtime_error("Wrong number of generated top quark found: "+std::to_string(topQuarks.size()));
                            }
                            
                            
                            float weight = std::sqrt(getSF(topQuarks[0]->getPt())*getSF(topQuarks[1]->getPt()));
                            eventView->setUserRecord("top_pt_rew",weight);
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


PXL_MODULE_INIT(TopPtReweighting)
PXL_PLUGIN_INIT
