#include <algorithm>
#include <fstream>
#include <set>
#include <sstream>
#include <streambuf>


#include "pxl/hep.hh"
#include "pxl/core.hh"
#include "pxl/core/macros.hh"
#include "pxl/core/PluginManager.hh"
#include "pxl/modules/Module.hh"
#include "pxl/modules/ModuleFactory.hh"

static pxl::Logger logger("MuonVeto");

class MuonVeto:
    public pxl::Module
{
    private:
        pxl::Source* _outputVetoSource;
        pxl::Source* _outputOtherSource;
        
        std::string _inputEventViewName;
        std::string _inputMuonName;
        std::string _looseMuonName;
        
        bool _cleanEvent;
  
        /*Loose Muon Related Criteria*/
        double _pTminLooseMuon;  //Minimum transverse momentum
        double _etamaxLooseMuon; //Maximum pseudorapidity
        double _pfRelIsoCorDbLooseMuon; //Muon Isolation:Relative(Rel) Isolation Correction (Cor) Delta beta (Db) 
        double _pfRelIsoCorDbBetaLooseMuon; //Muon Isolation:Relative Isolation Correction (Cor) Delta beta (Db)- Beta parameter

      
    public:
        MuonVeto():
            Module(),
            _inputEventViewName("Reconstructed"),
            _inputMuonName("Muon"),
            _looseMuonName("LooseMuon"),
            
            _cleanEvent(true),

            _pTminLooseMuon(10),
            _etamaxLooseMuon(2.5),
            _pfRelIsoCorDbLooseMuon(0.2),
            _pfRelIsoCorDbBetaLooseMuon(0.5)

	    /*Initial Values for tight Muons taken TOP Muon Information for Analysis (Run2) 
	      https://twiki.cern.ch/twiki/bin/view/CMS/TopMUO#Signal */

	    /*Initial Values for loose Muons taken from single t-quark cross section at 8 TeV 
	      http://cms.cern.ch/iCMS/jsp/openfile.jsp?tp=draft&files=AN2013_032_v8.pdf */

        {
            addSink("input", "input");
            _outputVetoSource = addSource("veto loose muons","veto");
            _outputOtherSource = addSource("other", "other");

            addOption("Event view","name of the event view where muons are selected",_inputEventViewName);
            addOption("Input muon name","name of particles to consider for selection",_inputMuonName);
            addOption("Name of selected loose muons","",_looseMuonName);
            
            addOption("Clean event","this option will clean the event of all muons falling loose criteria",_cleanEvent);

            addOption("LooseMuon Minimum pT","",_pTminLooseMuon);
            addOption("LooseMuon Maximum Eta","",_etamaxLooseMuon);
            addOption("LooseMuon Minimum Relative Iso DeltaBeta","",_pfRelIsoCorDbLooseMuon);
            addOption("LooseMuon Relative Iso DeltaBeta; Beta Parameter","",_pfRelIsoCorDbBetaLooseMuon);

        }

        ~MuonVeto()
        {
        }

        // every Module needs a unique type
        static const std::string &getStaticType()
        {
            static std::string type ("MuonVeto");
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
            getOption("Input muon name",_inputMuonName);
            getOption("Name of selected loose muons",_looseMuonName);
            
            getOption("Clean event",_cleanEvent);

            getOption("LooseMuon Minimum pT",_pTminLooseMuon);
            getOption("LooseMuon Maximum Eta",_etamaxLooseMuon);
            getOption("LooseMuon Minimum Relative Iso DeltaBeta",_pfRelIsoCorDbLooseMuon);
            getOption("LooseMuon Relative Iso DeltaBeta; Beta Parameter",_pfRelIsoCorDbBetaLooseMuon);
        }

        bool passesLooseCriteria(pxl::Particle* particle)
        {
            if (not (particle->getPt()>_pTminLooseMuon))
            {
                return false;
            }
            if (not (fabs(particle->getEta())<_etamaxLooseMuon))
            {
                return false;
            }
            if (not particle->getUserRecord("isLooseMuon"))
            {
                return false;
            }
            if (not (pfRelIsoCorDb (particle)<_pfRelIsoCorDbLooseMuon))
            {
                return false;
            }
            return true;
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
                    
                    std::vector<pxl::Particle*> looseMuons;
                    std::vector<pxl::Particle*> otherMuons;
                    
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

                                if (particle->getName()==_inputMuonName)
                                {
                                    if (passesLooseCriteria(particle))
                                    {
                                        looseMuons.push_back(particle);
                                    }
                                    else
                                    {
                                        otherMuons.push_back(particle);
                                    }
                                }
                            }
                        }
                        if (_cleanEvent)
                        {
                            for (unsigned int iparticle = 0; iparticle < otherMuons.size(); ++iparticle)
                            {
                                eventView->removeObject(otherMuons[iparticle]);
                            }
                        }

                        if (looseMuons.size()==0)
                        {
                            _outputVetoSource->setTargets(event);
                            return _outputVetoSource->processTargets();
                        }
                        else
                        {
                            for (unsigned int i=0; i < looseMuons.size(); ++i)
                            {
                                looseMuons[i]->setName(_looseMuonName);
                            }
                            _outputOtherSource->setTargets(event);
                            return _outputOtherSource->processTargets();
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
        
        double pfRelIsoCorDb (const pxl::Particle* particle)
        {
            float R04PFsumChargedHadronPt = particle->getUserRecord("R04PFsumChargedHadronPt").toFloat();
            float R04sumNeutralHadronEt = particle->getUserRecord("R04PFsumNeutralHadronEt").toFloat(); //Correct it!
            float R04PFsumPhotonEt = particle->getUserRecord("R04PFsumPhotonEt").toFloat(); //Correct it!
            float R04PFsumPUPt = particle->getUserRecord("R04PFsumPUPt").toFloat();
            float pT =  particle->getPt();
            if( pT < std::numeric_limits<float>::epsilon())
            {
                throw "Division by zero pT!";
            }
            return  (R04PFsumChargedHadronPt + std::max(R04sumNeutralHadronEt+ R04PFsumPhotonEt - _pfRelIsoCorDbBetaLooseMuon*R04PFsumPUPt, 0.0)) / pT;
        }
  
};

PXL_MODULE_INIT(MuonVeto)
PXL_PLUGIN_INIT

