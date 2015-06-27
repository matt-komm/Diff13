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

static pxl::Logger logger("MuonSelection");

class MuonSelection:
    public pxl::Module
{
    private:
        pxl::Source* _outputIsoSource;
        pxl::Source* _outputAntiIsoSource;
        pxl::Source* _outputOtherSource;
        
        std::string _inputEventViewName;
        std::string _inputMuonName;
        std::string _tightMuonName;
  
        /*Tight Muon Related Criteria*/
        double _pTMinTightMuon;  //Minimum transverse momentum
        double _etaMaxTightMuon; //Maximum pseudorapidity
        double _pfRelIsoCorDbTightMuon; //Muon Isolation:Relative(Rel) Isolation Correction (Cor) Delta beta (Db)
        double _pfRelIsoCorDbBetaTightMuon; //Muon Isolation:Relative Isolation Correction (Cor) Delta beta (Db)- Beta parameter

      
    public:
        MuonSelection():

            Module(),
            _inputEventViewName("Reconstructed"),
            _inputMuonName("Muon"),
            _tightMuonName("TightMuon"),

            _pTMinTightMuon(30),
            _etaMaxTightMuon(2.5),
            _pfRelIsoCorDbTightMuon(0.12),
            _pfRelIsoCorDbBetaTightMuon(0.5)


	    /*Initial Values for tight Muons taken TOP Muon Information for Analysis (Run2) 
	      https://twiki.cern.ch/twiki/bin/view/CMS/TopMUO#Signal */

	    /*Initial Values for loose Muons taken from single t-quark cross section at 8 TeV 
	      http://cms.cern.ch/iCMS/jsp/openfile.jsp?tp=draft&files=AN2013_032_v8.pdf */

        {
            addSink("input", "input");
            _outputIsoSource = addSource("1 iso muon", "iso");
            _outputAntiIsoSource = addSource("1 anti-iso muon", "anti-iso");
            _outputOtherSource = addSource("other", "other");

            addOption("Event view","name of the event view where muons are selected",_inputEventViewName);
            addOption("Input muon name","name of particles to consider for selection",_inputMuonName);
            addOption("Name of selected tight muons","",_tightMuonName);

            addOption("TightMuon Minimum pT","",_pTMinTightMuon);
            addOption("TightMuon Maximum Eta","",_etaMaxTightMuon);
            addOption("TightMuon Minimum Relative Iso DeltaBeta","",_pfRelIsoCorDbTightMuon);
            addOption("TightMuon Relative Iso DeltaBeta; Beta Parameter","",_pfRelIsoCorDbBetaTightMuon);
        }

        ~MuonSelection()
        {
        }

        // every Module needs a unique type
        static const std::string &getStaticType()
        {
            static std::string type ("MuonSelection");
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
            getOption("Name of selected tight muons",_tightMuonName);

            getOption("TightMuon Minimum pT",_pTMinTightMuon);
            getOption("TightMuon Maximum Eta",_etaMaxTightMuon);
            getOption("TightMuon Minimum Relative Iso DeltaBeta",_pfRelIsoCorDbTightMuon);
            getOption("TightMuon Relative Iso DeltaBeta; Beta Parameter",_pfRelIsoCorDbBetaTightMuon);
        }

        bool passesTightCriteria(pxl::Particle* particle)
        {
            if (not (particle->getPt()>_pTMinTightMuon)) {
                return false;
            }
            if (not (fabs(particle->getEta())<_etaMaxTightMuon)) {
                return false;
            }
            if (not particle->hasUserRecord("isTightMuon") || not particle->getUserRecord("isTightMuon"))
            {
                return false;
            }
            /*
            if (not particle->getUserRecord("isGlobalMuon"))
            {
                return false;
            }
            if (not particle->getUserRecord("isPFMuon"))
            {
                return false;
            }
            if (not particle->getUserRecord("isTightMuon"))
            {
                return false;
            }
            if (not ((particle->getUserRecord("chi2").toFloat()/particle->getUserRecord("ndof").toInt32())<_normChi2TightMuon)) {
                return false;
            }
            if (not (particle->getUserRecord("numberOfValidMuonHits").toInt32()>_numberOfValidMuonHitsTightMuon)) {
                return false;
            }
            if (not (particle->getUserRecord("numberOfValidPixelHits").toInt32()>_numberOfValidPixelHitsTightMuon)) {
                return false;
            }
            if (not (particle->getUserRecord("numberOfMatchedStations").toInt32()>_numberOfMatchedStationsTightMuon)) {
                return false;
            }
            if (not (fabs(particle->getUserRecord("dxy").toFloat())<_dxyMaxTightMuon)) {
                return false;
            }
            if (not (fabs(particle->getUserRecord("dz").toFloat())<_dzMaxTightMuon)) {
                return false;
            }
            if (not (particle->getUserRecord("trackerLayersWithMeasurement").toInt32()>_trackerLayersWithMeasurementTightMuon)) {
                return false;
            }
            if (not (pfRelIsoCorDb(particle)<_pfRelIsoCorDbTightMuon))
            {
                return false;
            }
            */
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
                    
                    std::vector<pxl::Particle*> tightIsoMuons;
                    std::vector<pxl::Particle*> tightAntiIsoMuons;
                    
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
                                    if (passesTightCriteria(particle))
                                    {
                                        if (pfRelIsoCorDb(particle)<_pfRelIsoCorDbTightMuon)
                                        {
                                            tightIsoMuons.push_back(particle);
                                        }
                                        else
                                        {
                                            tightAntiIsoMuons.push_back(particle);
                                        }
                                        
                                    }
                                }
                            }
                        }

                        if (tightIsoMuons.size()==1)
                        {
                            pxl::Particle* tightMuon = tightIsoMuons.front();   
                            tightMuon->setName(_tightMuonName);
                            _outputIsoSource->setTargets(event);
                            return _outputIsoSource->processTargets();
                            
                        }
                        else if (tightIsoMuons.size()==0 && tightAntiIsoMuons.size()==1)
                        {
                            pxl::Particle* tightMuon = tightAntiIsoMuons.front();   
                            tightMuon->setName(_tightMuonName);
                            _outputAntiIsoSource->setTargets(event);
                            return _outputAntiIsoSource->processTargets();
                        }
                        else
                        {
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
        
        double pfRelIsoCorDb (pxl::Particle* particle)
        {
            float R04PFsumChargedHadronPt = particle->getUserRecord("R04PFsumChargedHadronPt").toFloat();
            float R04sumNeutralHadronEt = particle->getUserRecord("R04PFsumNeutralHadronEt").toFloat();
            float R04PFsumPhotonEt = particle->getUserRecord("R04PFsumPhotonEt").toFloat();
            float R04PFsumPUPt = particle->getUserRecord("R04PFsumPUPt").toFloat();
            float pT =  particle->getPt();
            if( pT < std::numeric_limits<float>::epsilon())
            {
                throw "Division by zero pT!";
            }
            float relIso = (R04PFsumChargedHadronPt + std::max(R04sumNeutralHadronEt+ R04PFsumPhotonEt - _pfRelIsoCorDbBetaTightMuon*R04PFsumPUPt, 0.0)) / pT;
            particle->setUserRecord("relIso_deltaBeta",relIso);
            return relIso;
        }
  
};

PXL_MODULE_INIT(MuonSelection)
PXL_PLUGIN_INIT

