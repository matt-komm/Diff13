#include "pxl/hep.hh"
#include "pxl/core.hh"
#include "pxl/core/macros.hh"
#include "pxl/core/PluginManager.hh"
#include "pxl/modules/Module.hh"
#include "pxl/modules/ModuleFactory.hh"

#include "EventShapeVariables.hpp"

static pxl::Logger logger("EventVariables");

class EventVariables:
    public pxl::Module
{
    private:
        pxl::Source* _outputSource;
        
        std::string _inputEventViewName;
        std::set<std::string> _particlesForEventShape;
        std::string _prefix;
        
    public:
        EventVariables():
            Module(),
            _inputEventViewName("Reconstructed"),
            _prefix("")
        {
            addSink("input", "input");
            _outputSource = addSource("output","output");

            addOption("event view","name of the event view",_inputEventViewName);
            addOption("particles","name of the event view",std::vector<std::string>{{"TightMuon","TightElectron","SelectedJet","SelectedBJet","Neutrino"}});
            addOption("prefix","user record prefix",_prefix);
        }

        ~EventVariables()
        {
        }
        /*
        std::vector<double> getEigenValues(const TMatrixD& matrix)
        {
            TMatrixDEigen eigen(matrix);
            unsigned int N = matrix.GetNrows();
            const TMatrixD& u = eigen.GetEigenValues();
            for (int i=0; i<N;++i)
            {
                for (int j=0; j<N;++j)
                {
                    printf("%4.2f ",u[i][j]);
                }
                printf("\n");
            }
            return std::vector<double>();
        }
        */
        // every Module needs a unique type
        static const std::string &getStaticType()
        {
            static std::string type ("EventVariables");
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
            std::vector<std::string> particleNameVector;
            getOption("particles",particleNameVector);
            for (const std::string& s: particleNameVector)
            {
                _particlesForEventShape.insert(s);
            }
            getOption("prefix",_prefix);
        }
        /*
        float angle(const pxl::Particle* p1, const pxl::Basic3Vector& boost1, const pxl::Particle* p2, const pxl::Basic3Vector& boost2)
        {
            pxl::LorentzVector boostP1 = p1->getVector();
            boostP1.boost(-boost1);
            pxl::LorentzVector boostP2 = p2->getVector();
            boostP2.boost(-boost2);

            return (boostP1.getPx()*boostP2.getPx()+boostP1.getPy()*boostP2.getPy()+boostP1.getPz()*boostP2.getPz())/(boostP1.getMag()*boostP2.getMag());
        }
        */
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
                            
                            std::vector<pxl::LorentzVector> eventShapeVectors;

                            for (unsigned int iparticle = 0; iparticle<particles.size(); ++iparticle)
                            {
                                pxl::Particle* particle = particles[iparticle];
                                if (_particlesForEventShape.find(particle->getName())!=_particlesForEventShape.end())
                                {
                                    eventShapeVectors.push_back(particle->getVector());
                                }
                            }
                            EventShapeVariables esv(eventShapeVectors);
                            eventView->setUserRecord(_prefix+"isotropy",esv.isotropy());
                            eventView->setUserRecord(_prefix+"circularity",esv.circularity());
                            eventView->setUserRecord(_prefix+"sphericity",esv.sphericity());
                            eventView->setUserRecord(_prefix+"aplanarity",esv.aplanarity());
                            eventView->setUserRecord(_prefix+"C",esv.C());
                            eventView->setUserRecord(_prefix+"D",esv.D());
                            //TODO: add thrust, fox wolfram & many more crazy variables
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

        void shutdown() throw(std::runtime_error)
        {
        }

        void destroy() throw (std::runtime_error)
        {
            delete this;
        }
};

PXL_MODULE_INIT(EventVariables)
PXL_PLUGIN_INIT
