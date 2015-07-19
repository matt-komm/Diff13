#include "pxl/hep.hh"
#include "pxl/core.hh"
#include "pxl/core/macros.hh"
#include "pxl/core/PluginManager.hh"
#include "pxl/modules/Module.hh"
#include "pxl/modules/ModuleFactory.hh"

static pxl::Logger logger("BTagSelection");

class BTagSelection:
    public pxl::Module
{
    private:
        //Exact N B Tagged Jet(s) 
        pxl::Source* _output0BTagsSource;
        pxl::Source* _output1BTagsSource;
        pxl::Source* _output2BTagsSource;
        pxl::Source* _outputOtherBTagsSource;
  
        std::string _inputJetName;
        std::string _inputEventViewName;
        std::string _bTaggedJetName;

        std::string _bTaggingAlgorithmName; //b-tag algorithm out of the ones implemented in CMSSW

        /* b tag algorithms related variables, more info on */
        /* https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideBTagging#Preliminary_working_or_operating */

        //"Combined Secondary Vertex", i.e. extracting the discriminator value from a given set of input variables
        double _maxEtaBJet; //Maximum pseudorapidity for b-tagging according to b-tagging algorithm
        double _bTaggingWorkingPoint; // working point of b-tagging algorithm above jets are tagged

    public:
        BTagSelection():
            Module(),
            _inputJetName("SelectedJet"),
            _inputEventViewName("Reconstructed"),
            _bTaggedJetName("SelectedBJet"),
            _bTaggingAlgorithmName("pfCombinedInclusiveSecondaryVertexV2BJetTags"),
            _maxEtaBJet(2.4),
            _bTaggingWorkingPoint(0.97)

        {
            addSink("input", "input");

            _outputOtherBTagsSource = addSource(">2 b-Tags", ">2 b-Tags");
            _output2BTagsSource = addSource("2 b-Tags", "2 b-Tags");
            _output1BTagsSource = addSource("1 b-Tags", "1 b-Tags");
            _output0BTagsSource = addSource("0 b-Tags", "0 b-Tags");


            addOption("event view","name of the event view where jets are selected",_inputEventViewName);
            addOption("input jet name","name of particles to consider for b-tagging",_inputJetName);
            addOption("name of selected b-jets","",_bTaggedJetName);

            addOption("b Tagging Algorithm","",_bTaggingAlgorithmName);

            addOption("maximum b-jet eta","",_maxEtaBJet);
            addOption("working point","",_bTaggingWorkingPoint);

        }

        ~BTagSelection()
        {
        }

        // every Module needs a unique type
        static const std::string &getStaticType()
        {
            static std::string type ("BTagSelection");
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
            getOption("name of selected b-jets",_bTaggedJetName);
            getOption("b Tagging Algorithm",_bTaggingAlgorithmName);

            getOption("maximum b-jet eta",_maxEtaBJet);
            getOption("working point",_bTaggingWorkingPoint);
        }

        bool isBtagged(pxl::Particle* particle)
        {
            if (not (fabs(particle->getEta())<_maxEtaBJet))
            {
                return false;
            }
            if (not (particle->getUserRecord(_bTaggingAlgorithmName).toFloat()>_bTaggingWorkingPoint))
            {
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
                    
                    std::vector<pxl::Particle*> selectedBJets;
                    
                    pxl::EventView* inputEventView = nullptr;
                    
                    for (unsigned ieventView=0; ieventView<eventViews.size();++ieventView)
                    {

                        pxl::EventView* eventView = eventViews[ieventView];
                        if (eventView->getName()==_inputEventViewName)
                        {
                            inputEventView=eventView;
                            std::vector<pxl::Particle*> particles;
                            eventView->getObjectsOfType(particles);

                            for (unsigned iparticle=0; iparticle<particles.size();++iparticle)
                            {
                                pxl::Particle* particle = particles[iparticle];

                                if (particle->getName()==_inputJetName)
                                {
                                    if (isBtagged(particle))
                                    {
                                        particle->setName(_bTaggedJetName);
                                        selectedBJets.push_back(particle);
                                    }
                                }
                            }
                        }
                    }
                    if (inputEventView)
                    {
                        inputEventView->setUserRecord("n"+_bTaggedJetName,selectedBJets.size());
                    }
                    else
                    {
                        _outputOtherBTagsSource->setTargets(event);
                        return _outputOtherBTagsSource->processTargets();
                    }
                    
                    switch (selectedBJets.size())
                    {
                        case 0:
                            _output0BTagsSource->setTargets(event);
                            return _output0BTagsSource->processTargets();
                        case 1:
                            _output1BTagsSource->setTargets(event);
                            return _output1BTagsSource->processTargets();
                        case 2:
                            _output2BTagsSource->setTargets(event);
                            return _output2BTagsSource->processTargets();
                        default:
                            _outputOtherBTagsSource->setTargets(event);
                            return _outputOtherBTagsSource->processTargets();
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

PXL_MODULE_INIT(BTagSelection)
PXL_PLUGIN_INIT
