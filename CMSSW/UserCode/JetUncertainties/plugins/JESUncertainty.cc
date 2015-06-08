
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Math/interface/LorentzVector.h"

#include <string>
#include <vector>

template<class HANDLE, class VALUE> void putValueMap(
    edm::Event& edmEvent, 
    edm::Handle<HANDLE>& handle,
    std::vector<VALUE>& valueList,
    const std::string& labelName
)
{
    typename std::auto_ptr<edm::ValueMap<VALUE> > out(new edm::ValueMap<VALUE>() );
    typename edm::ValueMap<VALUE>::Filler filler(*out);
    filler.insert(handle, valueList.begin(), valueList.end() );
    filler.fill();
    edmEvent.put(out,labelName);
};

class JESUncertainty:
    public edm::EDProducer
{
    private:
        
        typedef math::XYZTLorentzVector LorentzVector;
        
        const std::string _jecES;
        edm::EDGetTokenT<std::vector<pat::Jet>> _jetToken;
        edm::EDGetTokenT<std::vector<pat::MET>> _metToken;
        const double _delta;
        
    public:
        explicit JESUncertainty(const edm::ParameterSet& config):
            _jecES(config.getParameter<std::string>("jecES")),
            _jetToken(consumes<std::vector<pat::Jet>>(config.getParameter<edm::InputTag>("jetSrc"))),
            _metToken(consumes<std::vector<pat::MET>>(config.getParameter<edm::InputTag>("metSrc"))),
            _delta(config.getParameter<double>("delta"))
        {
            produces<edm::ValueMap<LorentzVector>> ("jets");
            produces<edm::ValueMap<LorentzVector>> ("mets");
        }
        
        ~JESUncertainty()
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
            edm::ESHandle<JetCorrectorParametersCollection> JetCorParColl;
            edmSetup.get<JetCorrectionsRecord>().get(_jecES,JetCorParColl);
            const JetCorrectorParameters& JetCorPar = (*JetCorParColl)["Uncertainty"];
            JetCorrectionUncertainty* jecUnc = new JetCorrectionUncertainty(JetCorPar);
            (void)jecUnc;
            
            /*
            1: L2Relative
            2: L3Absolute
            8: L2L3Residual
            9: Uncertainty
            10: L1FastJet
            
            //TODO: FactorizedJetCorrector to make default corrections?
            
            std::vector<int> keys;
            JetCorParColl->validKeys(keys);
            for (int k: keys)
            {
                std::cout<<k<<": "<<JetCorParColl->findLabel(k)<<std::endl;
            }
            std::cout<<std::endl;
            */
            
            edm::Handle<std::vector<pat::Jet>> jetCollection;
            edmEvent.getByToken(_jetToken,jetCollection);

            edm::Handle<std::vector<pat::MET>> metCollection;
            edmEvent.getByToken(_metToken,metCollection);

            std::vector<LorentzVector> shiftedJets(jetCollection->size());
            
            std::vector<LorentzVector> shiftedMETs(metCollection->size());
            
            for (unsigned int imet = 0; imet < metCollection->size(); ++imet)
            {
                shiftedMETs[imet]=metCollection->at(imet).p4();
            }
            
            for (unsigned int ijet = 0; ijet < jetCollection->size(); ++ijet)
            {
                const pat::Jet& jet = jetCollection->at(ijet);
                jecUnc->setJetEta(jet.eta());
                jecUnc->setJetPt(jet.pt());
                LorentzVector shiftedMomentum = jet.p4()*(1.0+jecUnc->getUncertainty(true)*_delta);

                shiftedJets[ijet]=std::move(shiftedMomentum);

                //TODO: MET variation is somewhat improvised and may not be correct at all!!!
                for (unsigned int imet = 0; imet < metCollection->size(); ++imet)
                {
                    shiftedMETs[imet]-=(shiftedMomentum-jet.p4());
                }
            }
            for (unsigned int imet = 0; imet < metCollection->size(); ++imet)
            {
                shiftedMETs[imet].SetPz(0.0);
            }
            
            putValueMap(edmEvent,jetCollection,shiftedJets,"jets");
            putValueMap(edmEvent,metCollection,shiftedMETs,"mets");
	        
	        
        }
        virtual void endJob() override
        {
        }
};


DEFINE_FWK_MODULE(JESUncertainty);
