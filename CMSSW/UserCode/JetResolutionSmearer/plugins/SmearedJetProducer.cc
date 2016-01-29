#include "UserCode/JetResolutionSmearer/interface/SmearedJetProducerT.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

typedef SmearedJetProducerT<reco::CaloJet> NewSmearedCaloJetProducer;
typedef SmearedJetProducerT<reco::PFJet>   NewSmearedPFJetProducer;
typedef SmearedJetProducerT<pat::Jet> NewSmearedPATJetProducer;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(NewSmearedCaloJetProducer);
DEFINE_FWK_MODULE(NewSmearedPFJetProducer);
DEFINE_FWK_MODULE(NewSmearedPATJetProducer);
