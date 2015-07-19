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
        pxl::Source* _outputIsoLessSource;
        pxl::Source* _outputIsoMoreSource;
        pxl::Source* _outputAntiIsoSource;
        pxl::Source* _outputOtherSource;

        std::string _inputEventViewName;
        std::string _inputMuonName;
        std::string _tightMuonName;
  
        /*Tight Muon Related Criteria*/
        double _pTMinTightMuon;  //Minimum transverse momentum
        double _etaMaxTightMuon; //Maximum pseudorapidity
        double _pfRelIsoLessCorDbTightMuon; //Muon Isolation:Relative(Rel) Isolation Correction (Cor) Delta beta (Db)
        double _pfRelIsoMoreCorDbTightMuon; //Muon Isolation:Relative(Rel) Isolation Correction (Cor) Delta beta (Db)
        double _pfRelIsoCorDbBetaTightMuon; //Muon Isolation:Relative Isolation Correction (Cor) Delta beta (Db)- Beta parameter

      
    public:
        MuonSelection():

            Module(),
            _inputEventViewName("Reconstructed"),
            _inputMuonName("Muon"),
            _tightMuonName("TightMuon"),

            _pTMinTightMuon(22),
            _etaMaxTightMuon(2.1),
            _pfRelIsoLessCorDbTightMuon(0.06),
            _pfRelIsoMoreCorDbTightMuon(0.12),
            _pfRelIsoCorDbBetaTightMuon(0.5)


        /*Initial Values for tight Muons taken TOP Muon Information for Analysis (Run2)
          https://twiki.cern.ch/twiki/bin/view/CMS/TopMUO#Signal */

        /*Initial Values for loose Muons taken from single t-quark cross section at 8 TeV
          http://cms.cern.ch/iCMS/jsp/openfile.jsp?tp=draft&files=AN2013_032_v8.pdf */

        {
            addSink("input", "input");
            _outputIsoLessSource = addSource("1 iso muon", "iso");
            _outputIsoMoreSource = addSource("1 iso-more muon", "iso-more");
            _outputAntiIsoSource = addSource("1 anti-iso muon", "anti-iso");
            _outputOtherSource = addSource("other", "other");

            addOption("Event view","name of the event view where muons are selected",_inputEventViewName);
            addOption("Input muon name","name of particles to consider for selection",_inputMuonName);
            addOption("Name of selected tight muons","",_tightMuonName);

            addOption("TightMuon Minimum pT","",_pTMinTightMuon);
            addOption("TightMuon Maximum Eta","",_etaMaxTightMuon);
            addOption("TightMuon Minimum Relative IsoLess DeltaBeta","",_pfRelIsoLessCorDbTightMuon);
            addOption("TightMuon Minimum Relative IsoMore DeltaBeta","",_pfRelIsoMoreCorDbTightMuon);
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
            getOption("TightMuon Minimum Relative IsoLess DeltaBeta",_pfRelIsoLessCorDbTightMuon);
            getOption("TightMuon Minimum Relative IsoMore DeltaBeta",_pfRelIsoMoreCorDbTightMuon);
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
            if (not (particle->getUserRecord("trackerLayersWithMeasurement").toFloat()>_trackerLayersWithMeasurementTightMuon))
            {
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
                    
                    std::vector<pxl::Particle*> tightIsoLessMuons;
                    std::vector<pxl::Particle*> tightIsoMoreMuons;
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
                                        if (pfRelIsoCorDb(particle)<_pfRelIsoLessCorDbTightMuon)
                                        {
                                            //highly isolated muons
                                            tightIsoLessMuons.push_back(particle);
                                        }
                                        else if (pfRelIsoCorDb(particle)>_pfRelIsoLessCorDbTightMuon && pfRelIsoCorDb(particle)<_pfRelIsoMoreCorDbTightMuon)
                                        {
                                            //intermediate isolated muons
                                            tightIsoMoreMuons.push_back(particle);
                                        }
                                        else
                                        {
                                            //non-isolated muons
                                            tightAntiIsoMuons.push_back(particle);
                                        }
                                    }
                                    break;
                                }
                            }
                        }
                    }

                    //1 highly iso muon, 0 intermediate iso muons
                    if (tightIsoLessMuons.size()==1 && tightIsoMoreMuons.size()==0)
                    {
                        pxl::Particle* tightMuon = tightIsoLessMuons.front();
                        tightMuon->setName(_tightMuonName);
                        _outputIsoLessSource->setTargets(event);
                        return _outputIsoLessSource->processTargets();
                    }
                    //0 highly iso muon, 1 intermediate iso muons
                    else if (tightIsoLessMuons.size()==0 && tightIsoMoreMuons.size()==1)
                    {
                        pxl::Particle* tightMuon = tightIsoMoreMuons.front();
                        tightMuon->setName(_tightMuonName);
                        _outputIsoMoreSource->setTargets(event);
                        return _outputIsoMoreSource->processTargets();
                    }
                    //0 highly iso muon, 0 intermediate iso muons, 1 non-iso muon
                    else if (tightIsoLessMuons.size()==0 && tightIsoMoreMuons.size()==0 && tightAntiIsoMuons.size()==1)
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


        double pfRelIsoPUPPI (pxl::Particle* particle)
        {
            float pT =  particle->getPt();
            if( pT < std::numeric_limits<float>::epsilon())
            {
                throw "Division by zero pT!";
            }
            float relIso = particle->getUserRecord("puppiIsoMuonR40").toFloat() / pT;

            return relIso;
        }
  
};

PXL_MODULE_INIT(MuonSelection)
PXL_PLUGIN_INIT

