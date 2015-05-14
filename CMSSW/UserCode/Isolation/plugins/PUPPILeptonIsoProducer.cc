// -*- C++ -*-
//
// Package:    test/PUPPILeptonIsoProducer
// Class:      PUPPILeptonIsoProducer
// 
/**\class PUPPILeptonIsoProducer PUPPILeptonIsoProducer.cc test/PUPPILeptonIsoProducer/plugins/PUPPILeptonIsoProducer.cc

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

class PUPPILeptonIsoProducer : public edm::EDProducer {
   public:
      explicit PUPPILeptonIsoProducer(const edm::ParameterSet&);
      ~PUPPILeptonIsoProducer();

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
   
      edm::EDGetTokenT<edm::ValueMap<float> > puppiToken_;

      float dRConeSize_;
};


//
// constructors and destructor
//
PUPPILeptonIsoProducer::PUPPILeptonIsoProducer(const edm::ParameterSet& iConfig):
    electronToken_(consumes<pat::ElectronCollection>(iConfig.getParameter<edm::InputTag>("electrons"))),
    muonToken_(consumes<pat::MuonCollection>(iConfig.getParameter<edm::InputTag>("muons"))),
    pFCandidatesToken_(consumes<candidateView_>(iConfig.getParameter<edm::InputTag>("pfCands"))),
    puppiToken_(consumes<edm::ValueMap<float> >(iConfig.getParameter<edm::InputTag>("puppi")))

{

  dRConeSize_  = iConfig.getUntrackedParameter<double>("dRConeSize");

  produces<edm::ValueMap<double> > ("MuonPuppiIso");
  produces<edm::ValueMap<double> > ("ElectronPuppiIso");

  
}


PUPPILeptonIsoProducer::~PUPPILeptonIsoProducer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
PUPPILeptonIsoProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    //Handle Particle Collections for PUPPI isolation
    edm::Handle<pat::MuonCollection> muons;
    iEvent.getByToken(muonToken_, muons);
    assert(muons.isValid());
    edm::Handle<pat::ElectronCollection> electrons;
    iEvent.getByToken(electronToken_, electrons);
    assert(electrons.isValid());
    edm::Handle<candidateView_> pfs;
    iEvent.getByToken(pFCandidatesToken_,pfs);
    assert(pfs.isValid());

    //Handle Value Map for PUPPI weights
    edm::Handle<edm::ValueMap<float> > weights;
    iEvent.getByToken(puppiToken_, weights); 
    assert(weights.isValid());

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

	// now loop on pf candidates
	// one possible Way 
    	//
	for (unsigned int i = 0; i <  pfs->size(); ++i) {
	  const pat::PackedCandidate *pf =  dynamic_cast<const pat::PackedCandidate*>(&pfs->at(i)); //check if const pat::PackedCandidate!=0 ?

	  edm::RefToBase<reco::Candidate>  pf_base_ref;
	  pf_base_ref = pfs->refAt(i);
	  float weight = (*weights)[pf_base_ref];

	  if (deltaR(*pf,*lep) < dRConeSize_) { 
	    // pfcandidate-based footprint removal
	    if (std::find(footprint.begin(), footprint.end(), reco::CandidatePtr(pfs,i)) != footprint.end()) {
	      continue;
	    }

	    if (pf->charge() == 0) {
	      if (pf->pdgId() == 22 && pf->pt() > 0.5) photons += weight*pf->pt();
	      else
		if (pf->pt() > 0.5) neutral += weight*pf->pt();
	    } else {
	      if (weight==1) charged += weight*pf->pt();
	      
	    }
	  }	
	}
  
	double rel_iso = (charged + neutral + photons)/lep->pt();
	if (abs(lep->pdgId())==13) muon_isolation.push_back(rel_iso);
	else if (abs(lep->pdgId())==11) electron_isolation.push_back(rel_iso);

	
    }

    if(!muon_isolation.empty())
      storeLeptonIso(iEvent, muons, muon_isolation,  "MuonPuppiIso");
    if(!electron_isolation.empty())
      storeLeptonIso(iEvent, electrons, electron_isolation, "ElectronPuppiIso");

}

// ------------ method called once each job just before starting event loop  ------------
void 
PUPPILeptonIsoProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
PUPPILeptonIsoProducer::endJob() {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
PUPPILeptonIsoProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}


template<class Hand, typename T>
void
PUPPILeptonIsoProducer:: storeLeptonIso(edm::Event &iEvent,
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
DEFINE_FWK_MODULE(PUPPILeptonIsoProducer);
