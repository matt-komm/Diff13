#include "pxl/hep.hh"
#include "pxl/core.hh"
#include "pxl/core/macros.hh"
#include "pxl/core/PluginManager.hh"
#include "pxl/modules/Module.hh"
#include "pxl/modules/ModuleFactory.hh"

static pxl::Logger logger("TriggerSelection");

class TriggerSelection:
    public pxl::Module
{
    private:
        pxl::Source* _outputSource;
        pxl::Source* _outputVetoSource;
        
        std::string _inputEventViewName;
        std::vector<std::string> _triggerFlags;
        std::string _combinedFlag;
        
        bool _requireAllFlags;
        bool _cleanNoneRequired;

    public:
        TriggerSelection():
            Module(),
            _inputEventViewName("Reconstructed"),
            _triggerFlags({"HLT_IsoMu24_eta2p1_IterTrk02_v1","HLT_IsoMu24_IterTrk02_v1"}),
            _requireAllFlags(false),
            _cleanNoneRequired(true)
        {
            addSink("input", "input");
            _outputSource = addSource("selected","selected");
            _outputVetoSource = addSource("veto", "veto");

            addOption("Event view","name of the event view",_inputEventViewName);
            addOption("required trigger flags","",_triggerFlags);


            addOption("require all","this option requires all triggers (if set to true) or at least one (if set to false) to be fired",_requireAllFlags);
            addOption("remove other HLT","this option removes all HLT* trigger flags which are not in the list",_cleanNoneRequired);
            addOption("combined flag","if not empty a new user record will be added to the event view storing the selection result",_combinedFlag);
            
        }

        ~TriggerSelection()
        {
        }

        // every Module needs a unique type
        static const std::string &getStaticType()
        {
            static std::string type ("TriggerSelection");
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
            getOption("Event view",_inputEventViewName);
            getOption("required trigger flags",_triggerFlags);
            getOption("combined flag",_combinedFlag);
            getOption("require all",_requireAllFlags);
            getOption("remove other HLT",_cleanNoneRequired);
    
        }

        bool passTriggerSelection(pxl::EventView* eventView)
        {
            bool accepted = _requireAllFlags; //concatenate with AND if true. Otherwise with OR if false.
            for (pxl::UserRecords::const_iterator it = eventView->getUserRecords().begin(); _cleanNoneRequired && it!=eventView->getUserRecords().end(); ++it)
            {
                //std::cout<<"remove "<<it->first<<std::endl;
                if (it->first.find("HLT")!=std::string::npos && std::find(_triggerFlags.begin(),_triggerFlags.end(),it->first)==_triggerFlags.end())
                {
                    eventView->getUserRecords().erase(it->first);
                }
            }
            for (unsigned int itrigger = 0; itrigger<_triggerFlags.size(); ++itrigger)
            {
                const std::string& triggerName = _triggerFlags[itrigger];
                if (eventView->hasUserRecord(triggerName))
                {
                    if (_requireAllFlags)
                    {
                        accepted = accepted and eventView->getUserRecord(triggerName);
                    }
                    else
                    {
                        accepted = accepted or eventView->getUserRecord(triggerName);
                    }
                }
                //if true & accepted=false -> return false
                //if false & accepted=true -> return true
                if (_requireAllFlags!=accepted)
                {
                    return accepted;
                }
            }
            
            return accepted;
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
                            bool result = passTriggerSelection(eventView);
                            
                            if (_combinedFlag.size()>0)
                            {
                                eventView->setUserRecord(_combinedFlag,result);
                            }
                            if (result)
                            {
                                _outputSource->setTargets(event);
                                return _outputSource->processTargets();
                            }
                            else
                            {
                                _outputVetoSource->setTargets(event);
                                return _outputVetoSource->processTargets();
                            }
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

PXL_MODULE_INIT(TriggerSelection)
PXL_PLUGIN_INIT
