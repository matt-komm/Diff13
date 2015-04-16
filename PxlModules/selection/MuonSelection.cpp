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
        pxl::Source* _outputSource;
        pxl::Source* _outputVetoSource;
        
        std::string _inputMuonName;
        std::string _inputEventViewName;
        std::string _tightMuonName;
        std::string _looseMuonName;
        

        bool _cleanEvent;

        int64_t _numTightMuons; //Number of selected tight muons
        int64_t _numLooseMuons; //Number of selected loose muons
  
        /*Tight Muon Related Criteria*/
        double _pTMinTightMuon;  //Minimum transverse momentum
        double _etaMaxTightMuon; //Maximum pseudorapidity
        double _normChi2TightMuon; //(Maximum) Chi2/nDoFs of muon track
        int64_t _numberOfValidMuonHitsTightMuon; //(Minimum) number of Muon Hits
        int64_t _numberOfValidPixelHitsTightMuon; //(Minimum) number of Pixel Hits
        int64_t _numberOfMatchedStationsTightMuon; //(Minimum) number of Matched Muon Stations
        double _dxyMaxTightMuon; //Maximum transverse impact parameter 
        double _dzMaxTightMuon; //Maximum longitudinal impact parameter  
        int64_t _trackerLayersWithMeasurementTightMuon; //(Minimum) number of tracker layers recorded a hit
        double _pfRelIsoCorDbTightMuon; //Muon Isolation:Relative(Rel) Isolation Correction (Cor) Delta beta (Db)
        double _pfRelIsoCorDbBetaTightMuon; //Muon Isolation:Relative Isolation Correction (Cor) Delta beta (Db)- Beta parameter

        /*Loose Muon Related Criteria*/
        double _pTminLooseMuon;  //Minimum transverse momentum
        double _etamaxLooseMuon; //Maximum pseudorapidity
        std::string _idsLooseMuon; //Muon Conditions as provided by the user e.g. Tracker Muon 
        std::vector<std::string> _idLooseMuon; //Vector to store each Muon Condition sepately
        std::string _pfIsoMethodsLooseMuon; //Particle Flow (pf) Isolation (Iso) Method(s) as provided by the user 
        double _pfRelIsoCorDbLooseMuon; //Muon Isolation:Relative(Rel) Isolation Correction (Cor) Delta beta (Db) 
        double _pfRelIsoCorDbBetaLooseMuon; //Muon Isolation:Relative Isolation Correction (Cor) Delta beta (Db)- Beta parameter

      
    public:
        MuonSelection():

            Module(),
            _inputMuonName("Muon"),
            _inputEventViewName("Reconstructed"),
            _tightMuonName("TightMuon"),
            _looseMuonName("LooseMuon"),
            _cleanEvent(true),
            _numTightMuons(1),
            _numLooseMuons(0),

            _pTMinTightMuon(26.),
            _etaMaxTightMuon(2.1),
            _normChi2TightMuon(10.0),
            _numberOfValidMuonHitsTightMuon(0),
            _numberOfValidPixelHitsTightMuon(0),
            _numberOfMatchedStationsTightMuon(1),
            _dxyMaxTightMuon(0.2),
            _dzMaxTightMuon(0.5),
            _trackerLayersWithMeasurementTightMuon(5),
            _pfRelIsoCorDbTightMuon(0.12),
            _pfRelIsoCorDbBetaTightMuon(0.5),

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
            _outputSource = addSource("selected", "selected");
            _outputVetoSource = addSource("veto", "veto");

            addOption("Event view","name of the event view where muons are selected",_inputEventViewName);
            addOption("Input muon name","name of particles to consider for selection",_inputMuonName);
            addOption("Name of selected tight muons","",_tightMuonName);
            addOption("Name of selected loose muons","",_looseMuonName);
            addOption("Clean event","this option will clean the event of all muons falling tight or loose criteria",_cleanEvent);
            
            addOption("TightMuon Minimum pT","",_pTMinTightMuon);
            addOption("TightMuon Maximum Eta","",_etaMaxTightMuon);
            addOption("TightMuon Chi2/nDoFs", "",_normChi2TightMuon);
            addOption("TightMuon Number Of Valid Muon Hits", "",_numberOfValidMuonHitsTightMuon);
            addOption("TightMuon Number Of Valid Pixel Hits", "",_numberOfValidPixelHitsTightMuon);
            addOption("TightMuon Number Of Matched Stations","", _numberOfMatchedStationsTightMuon);
            addOption("TightMuon Transverse Impact Parameter","", _dxyMaxTightMuon);
            addOption("TightMuon Longitudinal Impact Parameter","", _dzMaxTightMuon);
            addOption("TightMuon Tracker Layers With Measurement","", _trackerLayersWithMeasurementTightMuon);
            addOption("TightMuon Minimum Relative Iso DeltaBeta","",_pfRelIsoCorDbTightMuon);
            addOption("TightMuon Relative Iso DeltaBeta; Beta Parameter","",_pfRelIsoCorDbBetaTightMuon);

            addOption("Number of TightMuons to Select","",_numTightMuons);

            addOption("LooseMuon Minimum pT","",_pTminLooseMuon);
            addOption("LooseMuon Maximum Eta","",_etamaxLooseMuon);
            addOption("LooseMuon ID","", _idsLooseMuon);
            addOption("LooseMuon PF Iso","",_pfIsoMethodsLooseMuon);
            addOption("LooseMuon Minimum Relative Iso DeltaBeta","",_pfRelIsoCorDbLooseMuon);
            addOption("LooseMuon Relative Iso DeltaBeta; Beta Parameter","",_pfRelIsoCorDbBetaLooseMuon);

            addOption("Number of LooseMuons to Select","",_numLooseMuons);
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
            getOption("Name of selected loose muons",_looseMuonName);
            getOption("Clean event",_cleanEvent);
            
            getOption("TightMuon Minimum pT",_pTMinTightMuon);
            getOption("TightMuon Maximum Eta",_etaMaxTightMuon);
            getOption("TightMuon Chi2/nDoFs", _normChi2TightMuon);
            getOption("TightMuon Number Of Valid Muon Hits", _numberOfValidMuonHitsTightMuon);
            getOption("TightMuon Number Of Valid Pixel Hits", _numberOfValidPixelHitsTightMuon);
            getOption("TightMuon Number Of Matched Stations", _numberOfMatchedStationsTightMuon);
            getOption("TightMuon Transverse Impact Parameter", _dxyMaxTightMuon);
            getOption("TightMuon Longitudinal Impact Parameter", _dzMaxTightMuon);
            getOption("TightMuon Tracker Layers With Measurement", _trackerLayersWithMeasurementTightMuon);
            getOption("TightMuon Maximum Relative Iso DeltaBeta",_pfRelIsoCorDbTightMuon);
            getOption("TightMuon Relative Iso DeltaBeta; Beta Parameter",_pfRelIsoCorDbBetaTightMuon);
            getOption("Number of TightMuons to Select",_numTightMuons);

            getOption("LooseMuon Minimum pT",_pTminLooseMuon);
            getOption("LooseMuon Maximum Eta",_etamaxLooseMuon);
            getOption("LooseMuon PF Iso",_pfIsoMethodsLooseMuon);
            getOption("LooseMuon Minimum Relative Iso DeltaBeta",_pfRelIsoCorDbLooseMuon);
            getOption("LooseMuon Relative Iso DeltaBeta; Beta Parameter",_pfRelIsoCorDbBetaLooseMuon);
            getOption("Number of LooseMuons to Select",_numLooseMuons);




        }

        bool passesTightCriteria(pxl::Particle* particle)
        {
            if (not (particle->hasUserRecord("chi2") and particle->hasUserRecord("isTightMuon")))
            {
              //check if combined track, tracker track & PV had been found
                return false;
            }
            if (not (particle->getPt()>_pTMinTightMuon)) {
                return false;
            }
            if (not (fabs(particle->getEta())<_etaMaxTightMuon)) {
                return false;
            }
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
            return true;
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
            if (not particle->getUserRecord("isPFMuon"))
            {
                return false;
            }
            if (not (particle->getUserRecord("isGlobalMuon") || particle->getUserRecord("isTrackerMuon")))
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
                    
                    std::vector<pxl::Particle*> tightMuons;
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
                                    if (passesTightCriteria(particle))
                                    {
                                        tightMuons.push_back(particle);
                                    }
                                    else if (passesLooseCriteria(particle))
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

                        if (tightMuons.size()==_numTightMuons && looseMuons.size()==_numLooseMuons)
                        {
                            for (unsigned int i=0; i < tightMuons.size(); ++i)
                            {
                                tightMuons[i]->setName(_tightMuonName);
                            }
                            for (unsigned int i=0; i < looseMuons.size(); ++i)
                            {
                                looseMuons[i]->setName(_looseMuonName);
                            }
                            for (unsigned int i=0; _cleanEvent && (i < otherMuons.size()); ++i)
                            {
                                eventView->removeObject(otherMuons[i]);
                            }
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
            return  (R04PFsumChargedHadronPt + std::max(R04sumNeutralHadronEt+ R04PFsumPhotonEt - _pfRelIsoCorDbBetaTightMuon*R04PFsumPUPt, 0.0)) / pT;
        }
  
};

PXL_MODULE_INIT(MuonSelection)
PXL_PLUGIN_INIT

