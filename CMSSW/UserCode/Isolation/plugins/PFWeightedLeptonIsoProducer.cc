// -*- C++ -*-
//
// Package:    test/PFWeightedLeptonIsoProducer
// Class:      PFWeightedLeptonIsoProducer
// 
/**\class PFWeightedLeptonIsoProducer PFWeightedLeptonIsoProducer.cc test/PFWeightedLeptonIsoProducer/plugins/PFWeightedLeptonIsoProducer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Georgios Krintiras
//         Created:  Mon, 27 Apr 2015 23:04:18 GMT
//
//


// system include files
#include <memory>
#include <vector>
// user include files

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "DataFormats/Common/interface/ValueMap.h"

//
// class declaration
//

class PFWeightedLeptonIsoProducer : public edm::EDProducer {
   public:
      explicit PFWeightedLeptonIsoProducer(const edm::ParameterSet&);
      ~PFWeightedLeptonIsoProducer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual void produce(edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
  
      template<class Hand, typename T>
      void storeLeptonIso(edm::Event &iEvent,
			const edm::Handle<Hand > & handle, 
			const std::vector<T> & values,
			const std::string    & label) const ;


      // ----------member data ---------------------------
      edm::EDGetTokenT<pat::ElectronCollection> electronToken_;
      edm::EDGetTokenT<pat::MuonCollection> muonToken_;
      typedef edm::View<reco::Candidate> candidateView_;
      edm::EDGetTokenT< candidateView_ > pFCandidatesToken_;
      edm::EDGetTokenT< candidateView_ > pfWeightedNeutralHadronsToken_;
      edm::EDGetTokenT< candidateView_ > pfWeightedPhotonsToken_;
   
      float dRConeSize_;
  

};


//
// constructors and destructor
//
PFWeightedLeptonIsoProducer::PFWeightedLeptonIsoProducer(const edm::ParameterSet& iConfig):
    electronToken_(consumes<pat::ElectronCollection>(iConfig.getParameter<edm::InputTag>("electrons"))),
    muonToken_(consumes<pat::MuonCollection>(iConfig.getParameter<edm::InputTag>("muons"))),
    pFCandidatesToken_(consumes<candidateView_>(iConfig.getParameter<edm::InputTag>("pfCands"))),
    pfWeightedNeutralHadronsToken_(consumes<candidateView_>(iConfig.getParameter<edm::InputTag>("pfWeightedHadrons"))),
    pfWeightedPhotonsToken_(consumes<candidateView_>(iConfig.getParameter<edm::InputTag>("pfWeightedPhotons")))

{

  dRConeSize_  = iConfig.getUntrackedParameter<double>("dRConeSize");

  produces<edm::ValueMap<double> > ("MuonPFWeightedIso");
  produces<edm::ValueMap<double> > ("ElectronPFWeightedIso");

  
}


PFWeightedLeptonIsoProducer::~PFWeightedLeptonIsoProducer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
PFWeightedLeptonIsoProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    //Handle Particle Collections for PUPPI isolation
    edm::Handle<pat::MuonCollection> muons;
    iEvent.getByToken(muonToken_, muons);
    assert(muons.isValid());
    edm::Handle<pat::ElectronCollection> electrons;
    iEvent.getByToken(electronToken_, electrons);
    assert(electrons.isValid());
    edm::Handle<candidateView_> pfCharged;
    iEvent.getByToken(pFCandidatesToken_,pfCharged);
    assert(pfCharged.isValid());
    edm::Handle<candidateView_> pfNU;
    iEvent.getByToken(pfWeightedNeutralHadronsToken_,pfNU);
    edm::Handle<candidateView_> pfPH;
    iEvent.getByToken(pfWeightedPhotonsToken_,pfPH);

    std::vector<const reco::Candidate *> leptons;

    for (const pat::Muon &mu : *muons) leptons.push_back(&mu);
    for (const pat::Electron &el : *electrons) leptons.push_back(&el);

    std::vector<double>  muon_isolation;
    std::vector<double>  electron_isolation;

    for (const reco::Candidate *lep : leptons) {
        // initialize sums
        float charged = 0, neutral = 0, photons  = 0;

        // now get a list of the PF candidates used to build this lepton, so to exclude them
	std::vector<reco::CandidatePtr> footprint;
	for (unsigned int i = 0, n = lep->numberOfSourceCandidatePtrs(); i < n; ++i) {
	  footprint.push_back(lep->sourceCandidatePtr(i));
	}

	// now loop on pf charged candidates

	for (unsigned int i = 0; i <  pfCharged->size(); ++i) {
	  const pat::PackedCandidate *pf =  dynamic_cast<const pat::PackedCandidate*>(&pfCharged->at(i)); //check if const pat::PackedCandidate!=0 ?

	  if (deltaR(*pf,*lep) < dRConeSize_) { 

	    // pfcandidate-based footprint removal
	    if (std::find(footprint.begin(), footprint.end(), reco::CandidatePtr(pfCharged,i)) != footprint.end()) {
	      continue;
	    }

	    if (pf->charge() != 0 && pf->fromPV()>2) charged += pf->pt();
	  }
	}
	
	// now loop on PF-weighted neutral candidates
	for (unsigned int i = 0; i <  pfNU->size(); ++i) {
	  const reco::PFCandidate *pf =  dynamic_cast<const reco::PFCandidate*>(&pfNU->at(i)); //check if const pat::PackedCandidate!=0 ?

	  if (deltaR(*pf,*lep) < dRConeSize_) { 

	    // pfcandidate-based footprint removal
	    if (std::find(footprint.begin(), footprint.end(), reco::CandidatePtr(pfNU,i)) != footprint.end()) {
	      continue;
	    }
	    neutral += pf->pt();
	  }
	}

	// now loop on PF-weighted photon candidates
	for (unsigned int i = 0; i <  pfPH->size(); ++i) {
	  const reco::PFCandidate *pf =  dynamic_cast<const reco::PFCandidate*>(&pfPH->at(i)); //check if const pat::PackedCandidate!=0 ?

	  if (deltaR(*pf,*lep) < dRConeSize_) { 
	    // pfcandidate-based footprint removal
	    if (std::find(footprint.begin(), footprint.end(), reco::CandidatePtr(pfPH,i)) != footprint.end()) {
	      continue;
	    }
	    photons += pf->pt();
	  }
	}

	
	double rel_iso = (charged + neutral + photons)/lep->pt();
	if (abs(lep->pdgId())==13) muon_isolation.push_back(rel_iso);
	else if (abs(lep->pdgId())==11) electron_isolation.push_back(rel_iso);

	
    }

    if(!muon_isolation.empty())
      storeLeptonIso(iEvent, muons, muon_isolation,  "MuonPFWeightedIso");
    if(!electron_isolation.empty())
      storeLeptonIso(iEvent, electrons, electron_isolation, "ElectronPFWeightedIso");

}

// ------------ method called once each job just before starting event loop  ------------
void 
PFWeightedLeptonIsoProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
PFWeightedLeptonIsoProducer::endJob() {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
PFWeightedLeptonIsoProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}


template<class Hand, typename T>
void
PFWeightedLeptonIsoProducer:: storeLeptonIso(edm::Event &iEvent,
			    const edm::Handle<Hand > & handle,
			    const std::vector<T> & values,
			    const std::string    & label) const {

  
    std::auto_ptr<edm::ValueMap<T> > valMap(new edm::ValueMap<T>());
    typename edm::ValueMap<T>::Filler filler(*valMap);

    filler.insert(handle, values.begin(), values.end());
    filler.fill();
    iEvent.put(valMap, label);

}

//define this as a plug-in
DEFINE_FWK_MODULE(PFWeightedLeptonIsoProducer);
