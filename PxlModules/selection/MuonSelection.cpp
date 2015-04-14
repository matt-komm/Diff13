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
        
        std::string _inputightMuonName;
        std::string _inputEventViewName;
        std::string _tightMuonName;
        std::string _looseMuonName;
        

        bool _cleanEvent;
  
        /*Tight Muon Related Criteria*/
        double _pTMintightMuon;  //Minimum transverse momentum
        double _etaMaxtightMuon; //Maximum pseudorapidity
        std::string _idstightMuon; //Muon Conditions as provided by the user e.g. PF Muon ID, Muon Reconstruction Alghoritm ID
        std::vector<std::string> _idtightMuon; //Vector to store each Muon Condition sepately
        double _normChi2tightMuon; //(Maximum) Chi2/nDoFs of muon track
        int64_t _numberOfValidMuonHitstightMuon; //(Minimum) number of Muon Hits
        int64_t _numberOfValidPixelHitstightMuon; //(Minimum) number of Pixel Hits
        int64_t _numberOfMatchedStationstightMuon; //(Minimum) number of Matched Muon Stations
        double _dxyMaxtightMuon; //Maximum transverse impact parameter 
        double _dzMaxtightMuon; //Maximum longitudinal impact parameter  
        int64_t _trackerLayersWithMeasurementtightMuon; //(Minimum) number of tracker layers recorded a hit
        std::string _pfIsoMethodstightMuon; //Particle Flow (pf) Isolation (Iso) Method(s) as provided by the user 
        std::vector<std::string> _pfIsoMethodtightMuon; //Vector to store each Muon Isolation method
        double _pfRelIsoCorDbtightMuon; //Muon Isolation:Relative(Rel) Isolation Correction (Cor) Delta beta (Db)
        double _pfRelIsoCorDbBetatightMuon; //Muon Isolation:Relative Isolation Correction (Cor) Delta beta (Db)- Beta parameter

        int64_t _numtightMuons; //Number of selected tight muons

        /*Loose Muon Related Criteria*/
        double _pTminlooseMuon;  //Minimum transverse momentum
        double _etamaxlooseMuon; //Maximum pseudorapidity
        std::string _idslooseMuon; //Muon Conditions as provided by the user e.g. Tracker Muon 
        std::vector<std::string> _idlooseMuon; //Vector to store each Muon Condition sepately
        std::string _pfIsoMethodslooseMuon; //Particle Flow (pf) Isolation (Iso) Method(s) as provided by the user 
        std::vector<std::string> _pfIsoMethodlooseMuon; //Vector to store each Muon Isolation method
        double _pfRelIsoCorDblooseMuon; //Muon Isolation:Relative(Rel) Isolation Correction (Cor) Delta beta (Db) 
        double _pfRelIsoCorDbBetalooseMuon; //Muon Isolation:Relative Isolation Correction (Cor) Delta beta (Db)- Beta parameter

        int64_t _numlooseMuons; //Number of selected loose muons
      
    public:
        MuonSelection():

            Module(),
            _inputightMuonName("Muon"),
            _inputEventViewName("Reconstructed"),
            _tightMuonName("TightMuon"),
            _looseMuonName("LooseMuon"),
            _cleanEvent(true),
            _pTMintightMuon(26.),
            _etaMaxtightMuon(2.1),
            _idstightMuon("isPFMuon, isGlobalMuon"),
            _normChi2tightMuon(10),
	    _numberOfValidMuonHitstightMuon(0),
            _numberOfValidPixelHitstightMuon(1),
            _numberOfMatchedStationstightMuon(1),
            _dxyMaxtightMuon(0.2),
            _dzMaxtightMuon(0.5),
            _trackerLayersWithMeasurementtightMuon(5),
            _pfIsoMethodstightMuon("DeltaBeta"),
            _pfRelIsoCorDbtightMuon(0.12),
            _pfRelIsoCorDbBetatightMuon(0.5),
            _numtightMuons(1),
            _pTminlooseMuon(10),
            _etamaxlooseMuon(2.5),
            _idslooseMuon("isGlobalMuon"),
            _pfIsoMethodslooseMuon("DeltaBeta"),
            _pfRelIsoCorDblooseMuon(0.2),
            _pfRelIsoCorDbBetalooseMuon(0.5),
            _numlooseMuons(0)
	    /*Initial Values for tight Muons taken TOP Muon Information for Analysis (Run2) 
	      https://twiki.cern.ch/twiki/bin/view/CMS/TopMUO#Signal */

	    /*Initial Values for loose Muons taken from single t-quark cross section at 8 TeV 
	      http://cms.cern.ch/iCMS/jsp/openfile.jsp?tp=draft&files=AN2013_032_v8.pdf */

        {
            addSink("input", "input");
            _outputSource = addSource("selected", "selected");
            _outputVetoSource = addSource("veto", "veto");

            addOption("Event view","name of the event view where muons are selected",_inputEventViewName);
            addOption("Input muon name","name of particles to consider for selection",_inputightMuonName);
            addOption("Name of selected tight muons","",_tightMuonName);
            addOption("Name of selected loose muons","",_looseMuonName);
            addOption("Clean event","this option will clean the event of all muons falling tight or loose criteria",_cleanEvent);
            
            addOption("TightMuon Minimum pT","",_pTMintightMuon);
            addOption("TightMuon Maximum Eta","",_etaMaxtightMuon);
            addOption("TightMuon ID","",_idstightMuon);
            addOption("TightMuon Chi2/nDoFs", "",_normChi2tightMuon);
	    addOption("TightMuon Number Of Valid Muon Hits", "",_numberOfValidMuonHitstightMuon);
            addOption("TightMuon Number Of Valid Pixel Hits", "",_numberOfValidPixelHitstightMuon);
            addOption("TightMuon Number Of Matched Stations","", _numberOfMatchedStationstightMuon);
            addOption("TightMuon Transverse Impact Parameter","", _dxyMaxtightMuon);
            addOption("TightMuon Longitudinal Impact Parameter","", _dzMaxtightMuon);
            addOption("TightMuon Tracker Layers With Measurement","", _trackerLayersWithMeasurementtightMuon);
            addOption("TightMuon PF Iso","",_pfIsoMethodstightMuon);
            addOption("TightMuon Minimum Relative Iso DeltaBeta","",_pfRelIsoCorDbtightMuon);
            addOption("TightMuon Relative Iso DeltaBeta; Beta Parameter","",_pfRelIsoCorDbBetatightMuon);
            addOption("Number of TightMuons to Select","",_numtightMuons);

            addOption("LooseMuon Minimum pT","",_pTminlooseMuon);
            addOption("LooseMuon Maximum Eta","",_etamaxlooseMuon);
            addOption("LooseMuon ID","", _idslooseMuon);
            addOption("LooseMuon PF Iso","",_pfIsoMethodslooseMuon);
            addOption("LooseMuon Minimum Relative Iso DeltaBeta","",_pfRelIsoCorDblooseMuon);
            addOption("LooseMuon Relative Iso DeltaBeta; Beta Parameter","",_pfRelIsoCorDbBetalooseMuon);
            addOption("Number of LooseMuons to Select","",_numlooseMuons);
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
            getOption("Input muon name",_inputightMuonName);
            getOption("Name of selected tight muons",_tightMuonName);
            getOption("Name of selected loose muons",_looseMuonName);
            getOption("Clean event",_cleanEvent);
            
            getOption("TightMuon Minimum pT",_pTMintightMuon);
            getOption("TightMuon Maximum Eta",_etaMaxtightMuon);
            getOption("TightMuon ID",_idstightMuon);
            getOption("TightMuon Chi2/nDoFs", _normChi2tightMuon);
	    getOption("TightMuon Number Of Valid Muon Hits", _numberOfValidMuonHitstightMuon);
            getOption("TightMuon Number Of Valid Pixel Hits", _numberOfValidPixelHitstightMuon);
            getOption("TightMuon Number Of Matched Stations", _numberOfMatchedStationstightMuon);
            getOption("TightMuon Transverse Impact Parameter", _dxyMaxtightMuon);
            getOption("TightMuon Longitudinal Impact Parameter", _dzMaxtightMuon);
            getOption("TightMuon Tracker Layers With Measurement", _trackerLayersWithMeasurementtightMuon);
            getOption("TightMuon PF Iso",_pfIsoMethodstightMuon);
            getOption("TightMuon Minimum Relative Iso DeltaBeta",_pfRelIsoCorDbtightMuon);
            getOption("TightMuon Relative Iso DeltaBeta; Beta Parameter",_pfRelIsoCorDbBetatightMuon);
            getOption("Number of TightMuons to Select",_numtightMuons);

            getOption("LooseMuon Minimum pT",_pTminlooseMuon);
            getOption("LooseMuon Maximum Eta",_etamaxlooseMuon);
            getOption("LooseMuon ID",_idslooseMuon);
            getOption("LooseMuon PF Iso",_pfIsoMethodslooseMuon);
            getOption("LooseMuon Minimum Relative Iso DeltaBeta",_pfRelIsoCorDblooseMuon);
            getOption("LooseMuon Relative Iso DeltaBeta; Beta Parameter",_pfRelIsoCorDbBetalooseMuon);
            getOption("Number of LooseMuons to Select",_numlooseMuons);

            try
            {
                std::set<char> token;
                identifyToken(token, _idstightMuon);
                std::istringstream idstightMuon(_idstightMuon);
                std::string idtightMuon;

                while (std::getline(idstightMuon, idtightMuon, (*token.begin())))
                {
                    removeWhiteSpaces(idtightMuon);
                    _idtightMuon.push_back(idtightMuon);
                }

            }
            catch (const char* msg)
            {
                std::cerr << msg << std::endl;
            }

            try
            {
                std::set<char> token;
                identifyToken(token, _pfIsoMethodstightMuon);
                std::istringstream pfIsoMethodstightMuon(_pfIsoMethodstightMuon);
                std::string pfIsoMethodtightMuon;

                while (std::getline(pfIsoMethodstightMuon, pfIsoMethodtightMuon, (*token.begin())))
                {
                    removeWhiteSpaces(pfIsoMethodtightMuon);
                    _pfIsoMethodtightMuon.push_back(pfIsoMethodtightMuon);
                }

            }
            catch (const char* msg)
            {
                std::cerr << msg << std::endl;
            }

            try
            {
                std::set<char> token;
                identifyToken(token, _idslooseMuon);
                std::istringstream idslooseMuon(_idslooseMuon);
                std::string idlooseMuon;

                while (std::getline(idslooseMuon, idlooseMuon, (*token.begin())))
                {
                    removeWhiteSpaces(idlooseMuon);
                    _idlooseMuon.push_back(idlooseMuon);
                }

            }
            catch (const char* msg)
            {
                std::cerr << msg << std::endl;
            }

            try
            {
                std::set<char> token;
                identifyToken(token, _pfIsoMethodslooseMuon);
                std::istringstream pfIsoMethodslooseMuon(_pfIsoMethodslooseMuon);
                std::string pfIsoMethodlooseMuon;

                while (std::getline(pfIsoMethodslooseMuon, pfIsoMethodlooseMuon, (*token.begin())))
                {
                    removeWhiteSpaces(pfIsoMethodlooseMuon);
                    _pfIsoMethodlooseMuon.push_back(pfIsoMethodlooseMuon);
                }

            }
            catch (const char* msg)
            {
                std::cerr << msg << std::endl;
            }
        }

        bool passesTightCriteria(pxl::Particle* particle)
        {

            if (not (particle->hasUserRecord("chi2") and particle->hasUserRecord("isTightMuon")))
            {
              //check if combined track, tracker track & PV had been found
                return false;
            }
            for (unsigned i=0; i<_idtightMuon.size(); ++i)
            {
                  //check Muon ID(s)
                if (not particle->getUserRecord(_idtightMuon[i]).toBool())
                {
                    return false;
                }
            }
            if (not (particle->getPt()>_pTMintightMuon)) {
                return false;
            }
            if (not (fabs(particle->getEta())<_etaMaxtightMuon)) {
                return false;
            }
            if (not ((particle->getUserRecord("chi2").toFloat()/particle->getUserRecord("ndof").toFloat())<_normChi2tightMuon)) {
                return false;
            }
	    if (not (particle->getUserRecord("numberOfValidMuonHits").toInt32()>_numberOfValidMuonHitstightMuon)) {
                return false;
            }
            if (not (particle->getUserRecord("numberOfValidPixelHits").toInt32()>_numberOfValidPixelHitstightMuon)) {
                return false;
            }
            if (not (particle->getUserRecord("numberOfMatchedStations").toInt32()>_numberOfMatchedStationstightMuon)) {
                return false;
            }
            if (not (fabs(particle->getUserRecord("dxy").toFloat())<_dxyMaxtightMuon)) {
                return false;
            }
            if (not (fabs(particle->getUserRecord("dz").toFloat())<_dzMaxtightMuon)) {
                return false;
            }
            if (not (particle->getUserRecord("trackerLayersWithMeasurement").toInt32()>_trackerLayersWithMeasurementtightMuon)) {
                return false;
            }
            for (unsigned i=0; i<_pfIsoMethodtightMuon.size(); ++i)
            {
	        if (not (pfRelIsoCorDb (particle)<_pfRelIsoCorDbtightMuon))
		  
                {
                    return false;
                }
            }

	    	    
	    return true;

        }

        bool passesLooseCriteria(pxl::Particle* particle)
        {
            if (not (particle->getPt()>_pTminlooseMuon))
            {
                return false;
            }
            if (not (fabs(particle->getEta())<_etamaxlooseMuon))
            {
                return false;
            }
            for (unsigned i=0; i<_pfIsoMethodlooseMuon.size(); ++i)
            {
	        if (not (pfRelIsoCorDb (particle)<_pfRelIsoCorDblooseMuon))
                {
                    return false;
                }
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

                                if (particle->getName()==_inputightMuonName)
                                {
				    if (passesTightCriteria(particle))
                                    {
                                        tightMuons.push_back(particle);
                                    } else if (passesLooseCriteria(particle)) {
                                        looseMuons.push_back(particle);
                                    } else {
				        otherMuons.push_back(particle);
                                    }
                                }
                            }
                        }
			
                        if (tightMuons.size()==_numtightMuons && looseMuons.size()==_numlooseMuons)
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
  
       void identifyToken (std::set<char> & token, std::string tokenize )
       {

            //Token could be everything other than a letter, i.e. everything with ASCII code outside the [65,90] and [97,122] range
            //What about excluding also the simple identation (ASCII code 45) and underscore (ASCII code 95)?
            for(unsigned int count =0; count < tokenize.size(); count++)
            {
                if ( ( (int) tokenize[count]>=65 && (int) tokenize[count]<=90 ) || ( (int) tokenize[count]>=97 && (int) tokenize[count]<=122 ))
                {
                    continue;
                }
                else
                {
                    if ( (int) tokenize[count]!=32 )
                    {
                        token.insert(tokenize[count]);
                    }
                }
            }

            if (token.size()>1) throw "Please define one delimeter at most!";
        }
  
        void removeWhiteSpaces( std::string &idMuon ) 
        {  
            //Function to remove white spaces that accidentally have been added by the user in the GUI
            int i;
            i = idMuon.find("  ");

            if ( i > -1 )
            {
                removeWhiteSpaces (  idMuon.replace( i,2,"" ) ); //recursive calling of the function itself
            }
            else if ( i == -1 )
            {
                i = idMuon.find(" ");
                if ( i > -1 )
                {
                    idMuon.erase(0,1);
                }
            }
        }
        
        double pfRelIsoCorDb (pxl::Particle* particle)
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
            return  (R04PFsumChargedHadronPt + std::max(R04sumNeutralHadronEt+ R04PFsumPhotonEt - _pfRelIsoCorDbBetatightMuon*R04PFsumPUPt, 0.0)) / pT;
        }
  
};

PXL_MODULE_INIT(MuonSelection)
PXL_PLUGIN_INIT

