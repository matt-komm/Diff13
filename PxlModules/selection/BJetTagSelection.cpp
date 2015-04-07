#include "pxl/hep.hh"
#include "pxl/core.hh"
#include "pxl/core/macros.hh"
#include "pxl/core/PluginManager.hh"
#include "pxl/modules/Module.hh"
#include "pxl/modules/ModuleFactory.hh"

static pxl::Logger logger("BJetTagSelection");

class BJetTagSelection:
    public pxl::Module
{
    private:
        pxl::Source* _output1BJetTagsSource;
        pxl::Source* _output2BJetTagsSource;
        pxl::Source* _output3BJetTagsSource;
        pxl::Source* _output4BJetTagsSource;
        pxl::Source* _outputOtherBJetTagsSource;

        std::string _inputJetName;
        std::string _inputEventViewName;
        std::string _selectedJetName;
        
        bool _cleanEvent;
        
        std::string _bTagAlgo; //b-tag algorithm out of the ones implemented in CMSSW

        /* b tag algorithms related variables, more info on */
        /* https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideBTagging#Preliminary_working_or_operating */

        //"Combined Secondary Vertex", i.e. extracting the discriminator value from a given set of input variables
        double _etaMaxCSVv2IVFBJetTag; //Maximum pseudorapidity for b-tagging according to xCSVv2IVF algorithm
        double _CSVv2IVFBJetTagWP; // working (or operating) point for CombinedSecondaryVertexv2+IVF ?

    public:
        BJetTagSelection():
            Module(),
            _inputJetName("SelectedJet"),
            _inputEventViewName("Reconstructed"),
            _selectedJetName("SelectedBJetTag"),
            _cleanEvent(true),
	    _bTagAlgo("CSVv2IVF"),
	    _etaMaxCSVv2IVFBJetTag(2.4),
	    _CSVv2IVFBJetTagWP(0.941)

        {
            addSink("input", "input");
            _output1BJetTagsSource = addSource("1 B Jet Tag", "1 B Jet Tag");
            _output2BJetTagsSource = addSource("2 B Jets Tag", "2 B Jets Tag");
            _output3BJetTagsSource = addSource("3 B Jets Tag", "3 B Jets Tag");
            _output4BJetTagsSource = addSource("4 B Jets Tag", "4 B Jets Tag");
            _outputOtherBJetTagsSource = addSource("other", "other");
            
            addOption("event view","name of the event view where jets are selected",_inputEventViewName);
            addOption("input jet name","name of particles to consider for selection",_inputJetName);
            addOption("name of selected jets","",_selectedJetName);
            addOption("clean event","this option will clean the event of all jets falling cuts",_cleanEvent);

            addOption("b Tagging Algorithm","",_bTagAlgo);

            addOption("Jet Maximum Eta for CSVv2IVFB","",_etaMaxCSVv2IVFBJetTag);
            addOption("WP for CSVv2IVFB","",_CSVv2IVFBJetTagWP);

        }

        ~BJetTagSelection()
        {
        }

        // every Module needs a unique type
        static const std::string &getStaticType()
        {
            static std::string type ("BJetTagSelection");
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
            getOption("input jet name",_inputJetName);
            getOption("name of selected jets",_selectedJetName);
            getOption("clean event",_cleanEvent);

            getOption("b Tagging Algorithm",_bTagAlgo);

            getOption("Jet Maximum Eta for CSVv2IVFB",_etaMaxCSVv2IVFBJetTag);
            getOption("WP for CSVv2IVFB",_CSVv2IVFBJetTagWP);

            
        }

        bool passesCSVv2IVFBJetTagSelection(pxl::Particle* particle)
        {
            //TODO: need to be extended to recommendation?
	    if (not fabs(particle->getEta())<_etaMaxCSVv2IVFBJetTag) { 
	      return false;
	    }
	    if (not (particle->getUserRecord("combinedMVABJetTags").toFloat()<_CSVv2IVFBJetTagWP)) {
	      return false;
	    }
	    
	    return true;
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
                    
                    std::vector<pxl::Particle*> selectedJets;
                    
		    for (unsigned ieventView=0; ieventView<eventViews.size();++ieventView)
                    {

                        pxl::EventView* eventView = eventViews[ieventView];
                        if (eventView->getName()==_inputEventViewName)
                        {
			    std::vector<pxl::Particle*> particles;
                            eventView->getObjectsOfType(particles);

                            for (unsigned iparticle=0; iparticle<particles.size();++iparticle)
                            {
                                pxl::Particle* particle = particles[iparticle];

			        if (particle->getName()==_inputJetName && _bTagAlgo.compare("CSVv2IVF") == 0)
                                {
				    if (passesCSVv2IVFBJetTagSelection(particle))
				    {
				      particle->setName(_selectedJetName);
				      selectedJets.push_back(particle);
				    } else if (_cleanEvent) {
				      eventView->removeObject(particle);
				    }
                                }
                            }
                        }
                        			
                        switch (selectedJets.size())
                        {
			    case 1:
                                _output1BJetTagsSource->setTargets(event);
                                return _output1BJetTagsSource->processTargets();
                            case 2:
                                _output2BJetTagsSource->setTargets(event);
                                return _output2BJetTagsSource->processTargets();
                            case 3:
                                _output3BJetTagsSource->setTargets(event);
                                return _output3BJetTagsSource->processTargets();
                            case 4:
                                _output4BJetTagsSource->setTargets(event);
                                return _output4BJetTagsSource->processTargets();  
                            default:
                                _outputOtherBJetTagsSource->setTargets(event);
                                return _outputOtherBJetTagsSource->processTargets();
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

PXL_MODULE_INIT(BJetTagSelection)
PXL_PLUGIN_INIT
