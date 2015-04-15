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
        std::string _tMuonName;
        std::string _lMuonName;
        

        bool _cleanEvent;
  
        /*Tight Muon Related Criteria*/
        double _pTMintMuon;  //Minimum transverse momentum
        double _etaMaxtMuon; //Maximum pseudorapidity
        std::string _idstMuon; //Muon Conditions as provided by the user e.g. PF Muon ID, Muon Reconstruction Alghoritm ID
        std::vector<std::string> _idtMuon; //Vector to store each Muon Condition sepately
        double _normChi2tMuon; //(Maximum) Chi2/nDoFs of muon track
        int64_t _numberOfValidPixelHitstMuon; //(Minimum) number of Pixel Hits
        int64_t _numberOfMatchedStationstMuon; //(Minimum) number of Matched Muon Stations
        double _dxyMaxtMuon; //Maximum transverse impact parameter 
        double _dzMaxtMuon; //Maximum longitudinal impact parameter  
        int64_t _trackerLayersWithMeasurementtMuon; //(Minimum) number of tracker layers recorded a hit
        std::string _pfIsoMethodstMuon; //Particle Flow (pf) Isolation (Iso) Method(s) as provided by the user 
        std::vector<std::string> _pfIsoMethodtMuon; //Vector to store each Muon Isolation method
        double _pfRelIsoCorDbtMuon; //Muon Isolation:Relative(Rel) Isolation Correction (Cor) Delta beta (Db)
        double _pfRelIsoCorDbBetatMuon; //Muon Isolation:Relative Isolation Correction (Cor) Delta beta (Db)- Beta parameter

        int64_t _numtMuons; //Number of selected tight muons

        /*Loose Muon Related Criteria*/
        double _pTminlMuon;  //Minimum transverse momentum
        double _etamaxlMuon; //Maximum pseudorapidity
        std::string _idslMuon; //Muon Conditions as provided by the user e.g. Tracker Muon 
        std::vector<std::string> _idlMuon; //Vector to store each Muon Condition sepately
        std::string _pfIsoMethodslMuon; //Particle Flow (pf) Isolation (Iso) Method(s) as provided by the user 
        std::vector<std::string> _pfIsoMethodlMuon; //Vector to store each Muon Isolation method
        double _pfRelIsoCorDblMuon; //Muon Isolation:Relative(Rel) Isolation Correction (Cor) Delta beta (Db) 
        double _pfRelIsoCorDbBetalMuon; //Muon Isolation:Relative Isolation Correction (Cor) Delta beta (Db)- Beta parameter

        int64_t _numlMuons; //Number of selected loose muons
      
    public:
        MuonSelection():

            Module(),
            _inputMuonName("Muon"),
            _inputEventViewName("Reconstructed"),
            _tMuonName("TightMuon"),
            _lMuonName("LooseMuon"),
            _cleanEvent(true),
            _pTMintMuon(26.),
            _etaMaxtMuon(2.1),
            _idstMuon("isPFMuon, isGlobalMuon"),
            _normChi2tMuon(10),
            _numberOfValidPixelHitstMuon(1),
            _numberOfMatchedStationstMuon(2),
            _dxyMaxtMuon(0.2),
            _dzMaxtMuon(0.5),
            _trackerLayersWithMeasurementtMuon(5),
            _pfIsoMethodstMuon("DeltaBeta"),
            _pfRelIsoCorDbtMuon(0.12),
            _pfRelIsoCorDbBetatMuon(0.5),
            _numtMuons(1),
            _pTminlMuon(10),
            _etamaxlMuon(2.5),
            _idslMuon("isGlobalMuon"),
            _pfIsoMethodslMuon("DeltaBeta"),
            _pfRelIsoCorDblMuon(0.2),
            _pfRelIsoCorDbBetalMuon(0.5),
            _numlMuons(0)
        /*Initial Values taken from single t-quark cross section at 8 TeV
        /*http://cms.cern.ch/iCMS/jsp/openfile.jsp?tp=draft&files=AN2013_032_v8.pdf */
        {
            addSink("input", "input");
            _outputSource = addSource("selected", "selected");
            _outputVetoSource = addSource("veto", "veto");

            addOption("Event view","name of the event view where muons are selected",_inputEventViewName);
            addOption("Input muon name","name of particles to consider for selection",_inputMuonName);
            addOption("Name of selected tight muons","",_tMuonName);
            addOption("Name of selected loose muons","",_lMuonName);
            addOption("Clean event","this option will clean the event of all muons falling tight or loose criteria",_cleanEvent);
            
            addOption("TightMuon Minimum pT","",_pTMintMuon);
            addOption("TightMuon Maximum Eta","",_etaMaxtMuon);
            addOption("TightMuon ID","",_idstMuon);
            addOption("TightMuon Chi2/nDoFs", "",_normChi2tMuon);
            addOption("TightMuon Number Of Valid Pixel Hits", "",_numberOfValidPixelHitstMuon);
            addOption("TightMuon Number Of Matched Stations","", _numberOfMatchedStationstMuon);
            addOption("TightMuon Transverse Impact Parameter","", _dxyMaxtMuon);
            addOption("TightMuon Longitudinal Impact Parameter","", _dzMaxtMuon);
            addOption("TightMuon Tracker Layers With Measurement","", _trackerLayersWithMeasurementtMuon);
            addOption("TightMuon PF Iso","",_pfIsoMethodstMuon);
            addOption("TightMuon Minimum Relative Iso DeltaBeta","",_pfRelIsoCorDbtMuon);
            addOption("TightMuon Relative Iso DeltaBeta; Beta Parameter","",_pfRelIsoCorDbBetatMuon);
            addOption("Number of TightMuons to Select","",_numtMuons);

            addOption("LooseMuon Minimum pT","",_pTminlMuon);
            addOption("LooseMuon Maximum Eta","",_etamaxlMuon);
            addOption("LooseMuon ID","", _idslMuon);
            addOption("LooseMuon PF Iso","",_pfIsoMethodslMuon);
            addOption("LooseMuon Minimum Relative Iso DeltaBeta","",_pfRelIsoCorDblMuon);
            addOption("LooseMuon Relative Iso DeltaBeta; Beta Parameter","",_pfRelIsoCorDbBetalMuon);
            addOption("Number of LooseMuons to Select","",_numlMuons);
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
            getOption("Name of selected tight muons",_tMuonName);
            getOption("Name of selected loose muons",_lMuonName);
            getOption("Clean event",_cleanEvent);
            
            getOption("TightMuon Minimum pT",_pTMintMuon);
            getOption("TightMuon Maximum Eta",_etaMaxtMuon);
            getOption("TightMuon ID",_idstMuon);
            getOption("TightMuon Chi2/nDoFs", _normChi2tMuon);
            getOption("TightMuon Number Of Valid Pixel Hits", _numberOfValidPixelHitstMuon);
            getOption("TightMuon Number Of Matched Stations", _numberOfMatchedStationstMuon);
            getOption("TightMuon Transverse Impact Parameter", _dxyMaxtMuon);
            getOption("TightMuon Longitudinal Impact Parameter", _dzMaxtMuon);
            getOption("TightMuon Tracker Layers With Measurement", _trackerLayersWithMeasurementtMuon);
            getOption("TightMuon PF Iso",_pfIsoMethodstMuon);
            getOption("TightMuon Minimum Relative Iso DeltaBeta",_pfRelIsoCorDbtMuon);
            getOption("TightMuon Relative Iso DeltaBeta; Beta Parameter",_pfRelIsoCorDbBetatMuon);
            getOption("Number of TightMuons to Select",_numtMuons);

            getOption("LooseMuon Minimum pT",_pTminlMuon);
            getOption("LooseMuon Maximum Eta",_etamaxlMuon);
            getOption("LooseMuon ID",_idslMuon);
            getOption("LooseMuon PF Iso",_pfIsoMethodslMuon);
            getOption("LooseMuon Minimum Relative Iso DeltaBeta",_pfRelIsoCorDblMuon);
            getOption("LooseMuon Relative Iso DeltaBeta; Beta Parameter",_pfRelIsoCorDbBetalMuon);
            getOption("Number of LooseMuons to Select",_numlMuons);

            try
            {
                std::set<char> token;
                identifyToken(token, _idstMuon);
                std::istringstream idstMuon(_idstMuon);
                std::string idtMuon;

                while (std::getline(idstMuon, idtMuon, (*token.begin())))
                {
                    removeWhiteSpaces(idtMuon);
                    _idtMuon.push_back(idtMuon);
                }

            }
            catch (const char* msg)
            {
                std::cerr << msg << std::endl;
            }

            try
            {
                std::set<char> token;
                identifyToken(token, _pfIsoMethodstMuon);
                std::istringstream pfIsoMethodstMuon(_pfIsoMethodstMuon);
                std::string pfIsoMethodtMuon;

                while (std::getline(pfIsoMethodstMuon, pfIsoMethodtMuon, (*token.begin())))
                {
                    removeWhiteSpaces(pfIsoMethodtMuon);
                    _pfIsoMethodtMuon.push_back(pfIsoMethodtMuon);
                }

            }
            catch (const char* msg)
            {
                std::cerr << msg << std::endl;
            }

            try
            {
                std::set<char> token;
                identifyToken(token, _idslMuon);
                std::istringstream idslMuon(_idslMuon);
                std::string idlMuon;

                while (std::getline(idslMuon, idlMuon, (*token.begin())))
                {
                    removeWhiteSpaces(idlMuon);
                    _idlMuon.push_back(idlMuon);
                }

            }
            catch (const char* msg)
            {
                std::cerr << msg << std::endl;
            }

            try
            {
                std::set<char> token;
                identifyToken(token, _pfIsoMethodslMuon);
                std::istringstream pfIsoMethodslMuon(_pfIsoMethodslMuon);
                std::string pfIsoMethodlMuon;

                while (std::getline(pfIsoMethodslMuon, pfIsoMethodlMuon, (*token.begin())))
                {
                    removeWhiteSpaces(pfIsoMethodlMuon);
                    _pfIsoMethodlMuon.push_back(pfIsoMethodlMuon);
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
            for (unsigned i=0; i<_idtMuon.size(); ++i)
            {
                  //check Muon ID(s)
                if (not particle->getUserRecord(_idtMuon[i]).toBool())
                {
                    return false;
                }
            }
            if (not (particle->getPt()>_pTMintMuon)) {
                return false;
            }
            if (not (fabs(particle->getEta())<_etaMaxtMuon)) {
                return false;
            }
            if (not ((particle->getUserRecord("chi2").toFloat()/particle->getUserRecord("ndof").toFloat())<_normChi2tMuon)) {
                return false;
            }
            if (not (particle->getUserRecord("numberOfValidPixelHits").toInt32()>_numberOfValidPixelHitstMuon)) {
                return false;
            }
            if (not (particle->getUserRecord("numberOfMatchedStations").toInt32()>_numberOfMatchedStationstMuon)) {
                return false;
            }
            if (not (fabs(particle->getUserRecord("dxy").toFloat())<_dxyMaxtMuon)) {
                return false;
            }
            if (not (fabs(particle->getUserRecord("dz").toFloat())<_dzMaxtMuon)) {
                return false;
            }
            if (not (particle->getUserRecord("trackerLayersWithMeasurement").toInt32()>_trackerLayersWithMeasurementtMuon)) {
                return false;
            }
            for (unsigned i=0; i<_pfIsoMethodtMuon.size(); ++i)
            {
                if (not (pfRelIsoCorDb (particle))>_pfRelIsoCorDbtMuon)
                {
                    return false;
                }
                return true;
            }
        }

        bool passesLooseCriteria(pxl::Particle* particle)
        {
            if (not (particle->getPt()>_pTminlMuon))
            {
                return false;
            }
            if (not (fabs(particle->getEta())<_etamaxlMuon))
            {
                return false;
            }
            for (unsigned i=0; i<_pfIsoMethodlMuon.size(); ++i)
            {
                if (not (pfRelIsoCorDb (particle))>_pfRelIsoCorDblMuon)
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
                    
                    std::vector<pxl::Particle*> tMuons;
                    std::vector<pxl::Particle*> lMuons;
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
                                        tMuons.push_back(particle);
                                    } else if (passesLooseCriteria(particle)) {
                                        lMuons.push_back(particle);
                                    } else {
                                        otherMuons.push_back(particle);
                                    }
                                }
                            }
                        }
                    
                        if (tMuons.size()==_numtMuons && lMuons.size()==_numlMuons)
                        {
                            for (unsigned int i=0; i < tMuons.size(); ++i)
                            {
                                tMuons[i]->setName(_tMuonName);
                            }
                            for (unsigned int i=0; i < lMuons.size(); ++i)
                            {
                                lMuons[i]->setName(_lMuonName);
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
            return  (R04PFsumChargedHadronPt + std::max(R04sumNeutralHadronEt+ R04PFsumPhotonEt - _pfRelIsoCorDbBetatMuon*R04PFsumPUPt, 0.0)) / pT;
        }
  
};

PXL_MODULE_INIT(MuonSelection)
PXL_PLUGIN_INIT

