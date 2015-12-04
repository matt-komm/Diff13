
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Math/interface/Vector3D.h"

#include "DataFormats/Common/interface/View.h"

#include "UserCode/JetCorrectionFactorProducer/interface/JetCorrectionLevel.h"

#include <string>
#include <vector>


template<class HANDLE, class VALUE> void putValueMap(
    edm::Event& edmEvent, 
    edm::Handle<HANDLE>& handle,
    std::vector<VALUE>& valueList,
    const std::string& labelName=""
)
{
    typename std::auto_ptr<edm::ValueMap<VALUE> > out(new edm::ValueMap<VALUE>() );
    typename edm::ValueMap<VALUE>::Filler filler(*out);
    filler.insert(handle, valueList.begin(), valueList.end() );
    filler.fill();
    edmEvent.put(out,labelName);
};


class MetT1Producer:
    public edm::EDProducer
{
    private:
        edm::EDGetTokenT<edm::View<pat::MET>> _metToken;
        edm::EDGetTokenT<edm::View<pat::Jet>> _jetToken;
        
        edm::EDGetTokenT<JetCorrectionLevel> _jetCorrectionL1Token;
        edm::EDGetTokenT<JetCorrectionLevel> _jetCorrectionL123Token;
        
        double _jetPtThreshold;
        double _jetEMThreshold;
        bool _skipMuons;
        
    public:
        explicit MetT1Producer(const edm::ParameterSet& config):
            _metToken(consumes<edm::View<pat::MET>>(config.getParameter<edm::InputTag>("metSrc"))),
            _jetToken(consumes<edm::View<pat::Jet>>(config.getParameter<edm::InputTag>("patJetSrc"))),
            _jetCorrectionL1Token(consumes<JetCorrectionLevel>(config.getParameter<edm::InputTag>("jetCorrectionL1"))),
            _jetCorrectionL123Token(consumes<JetCorrectionLevel>(config.getParameter<edm::InputTag>("jetCorrectionL123"))),
            _jetPtThreshold(config.getParameter<double>("minJetPt")),
            _jetEMThreshold(config.getParameter<double>("maxJetEM")),
            _skipMuons(config.getParameter<bool>("skipMuons"))
        {
            produces<edm::ValueMap<reco::Candidate::LorentzVector>>("old");
            produces<edm::ValueMap<reco::Candidate::LorentzVector>>("new");
        }
        
        ~MetT1Producer()
        {
        }

        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions)
        {
        }

    private:
        virtual void beginJob() override
        {
        }
        virtual void produce(edm::Event& edmEvent, const edm::EventSetup& edmSetup) override
        {
            edm::Handle<edm::View<pat::MET>> metCollection;
            edmEvent.getByToken(_metToken,metCollection);
            
            edm::Handle<edm::View<pat::Jet>> jetCollection;
            edmEvent.getByToken(_jetToken,jetCollection);
            
            edm::Handle<JetCorrectionLevel> jetCorrectionL1;
            edmEvent.getByToken(_jetCorrectionL1Token,jetCorrectionL1);
            
            edm::Handle<JetCorrectionLevel> jetCorrectionL123;
            edmEvent.getByToken(_jetCorrectionL123Token,jetCorrectionL123);
            
            std::vector<reco::Candidate::LorentzVector> outputOld(metCollection->size());
            std::vector<reco::Candidate::LorentzVector> outputNew(metCollection->size());
            
            for (unsigned int imet = 0; imet < metCollection->size(); ++imet)
            {
                const pat::MET& met = metCollection->at(imet);
                
                reco::Candidate::LorentzVector metT1Correction(0,0,0,0);
                //https://twiki.cern.ch/twiki/bin/view/CMS/METType1Type2Formulae#3_The_Type_I_correction
                //https://github.com/cms-sw/cmssw/blob/CMSSW_7_4_X/JetMETCorrections/Type1MET/interface/PFJetMETcorrInputProducerT.h
                for (unsigned int ijet = 0; ijet < jetCollection->size(); ++ijet) 
                {
                    const pat::Jet& jet = jetCollection->at(ijet);
                    std::shared_ptr<pat::Jet> rawJet(jet.clone());
                    rawJet->setP4(rawJet->correctedP4(0));
                    
                    std::cout<<"jet pt="<<jet.pt()<<" raw="<<rawJet->pt()<<" L123="<<(rawJet->p4()*jetCorrectionL123->getCorrection(*rawJet)).pt()<<std::endl;
                    
                    double emEnergyFraction = jet.chargedEmEnergyFraction() + jet.neutralEmEnergyFraction();
                    if (emEnergyFraction > _jetEMThreshold)
                    {
                        continue;
                    }
                    if (_skipMuons)
                    {
	                    for (unsigned int idaughter = 0; idaughter < jet.numberOfDaughters(); ++idaughter)
	                    {
                            const reco::Candidate* daughter = jet.daughter(idaughter);
                            if (daughter->isMuon())
                            {
                                //TODO: subtract muon from raw jet p4
                                rawJet->setP4(rawJet->p4()-daughter->p4());
                            }
                        }
                    }
                    


                    
                    reco::Candidate::LorentzVector jetPL1 = rawJet->p4()*jetCorrectionL1->getCorrection(*rawJet);
                    reco::Candidate::LorentzVector jetPL123 = rawJet->p4()*jetCorrectionL123->getCorrection(*rawJet);
                    
                                        
                    if (jetPL123.pt()>_jetPtThreshold)
                    {
                        metT1Correction+=(jetPL123-jetPL1);
                    }
                    
                    /*
                    if ((not _jetSelector) or (*_jetSelector)(rawP4))
                    {
                        continue;
                    }*/
                    //TODO: add L1 correction to raw jet excl. muons (need the function here somehow ??)
                    
                    //reco::Candidate::LorentzVector rawJetP4 = jet.uncorrectedP4();
                    
                }
                //TODO: final correction = sum over jets (raw jet with L123 excl. muons - raw jet with L1 excl. muons)
            
                outputOld[imet]=met.p4()-met.corP4(pat::MET::Raw);
                outputNew[imet]=metT1Correction;
            }    
                

            putValueMap(edmEvent,metCollection,outputOld,"old");
            putValueMap(edmEvent,metCollection,outputNew,"new");
        }
        virtual void endJob() override
        {
        }
};


DEFINE_FWK_MODULE(MetT1Producer);
