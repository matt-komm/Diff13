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
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "DataFormats/Common/interface/ValueMap.h"

#include <functional>

//
// class declaration
//

class PUPPILeptonIsoProducer : public edm::EDProducer
{
    public:
        explicit PUPPILeptonIsoProducer(const edm::ParameterSet&);
        ~PUPPILeptonIsoProducer();

        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

    private:
        virtual void beginJob() override;
        virtual void produce(edm::Event&, const edm::EventSetup&) override;
        virtual void endJob() override;
        
        edm::EDGetTokenT<edm::View<reco::RecoCandidate>> _leptonToken;
        edm::EDGetTokenT<edm::View<reco::Candidate>> _pfCandidatesToken;

        edm::EDGetTokenT<edm::ValueMap<float> > _puppiToken;

        float _dRConeSize;
};


//
// constructors and destructor
//
PUPPILeptonIsoProducer::PUPPILeptonIsoProducer(const edm::ParameterSet& iConfig):
    _leptonToken(consumes<edm::View<reco::RecoCandidate>>(iConfig.getParameter<edm::InputTag>("leptons"))),
    _pfCandidatesToken(consumes<edm::View<reco::Candidate>>(iConfig.getParameter<edm::InputTag>("pfCands"))),
    _puppiToken(consumes<edm::ValueMap<float> >(iConfig.getParameter<edm::InputTag>("puppi")))

{
    _dRConeSize  = iConfig.getUntrackedParameter<double>("dRConeSize");

    produces<edm::ValueMap<double> > ();
}


PUPPILeptonIsoProducer::~PUPPILeptonIsoProducer()
{
}


void
PUPPILeptonIsoProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    edm::Handle<edm::View<reco::RecoCandidate>> leptons;
    iEvent.getByToken(_leptonToken, leptons);
    
    edm::Handle<edm::View<reco::Candidate>> pfCandidates;
    iEvent.getByToken(_pfCandidatesToken,pfCandidates);

    edm::Handle<edm::ValueMap<float> > weights;
    iEvent.getByToken(_puppiToken, weights);


    std::vector<double>  leptonIsolation(leptons->size());
    for (unsigned int ilepton = 0; ilepton < leptons->size(); ++ilepton)
    {
        const reco::RecoCandidate& lepton = (leptons->at(ilepton));
        
        std::vector<reco::CandidatePtr> footprint;
	    for (unsigned int i = 0, n = lepton.numberOfSourceCandidatePtrs(); i < n; ++i)
	    {
            footprint.push_back(lepton.sourceCandidatePtr(i));
        }
	    double charged = 0, neutral = 0, photons  = 0;
        
        for (unsigned int ipf = 0; ipf <  pfCandidates->size(); ++ipf)
        {
            const reco::Candidate& pf = pfCandidates->at(ipf);
            float weight = (*weights)[pfCandidates->refAt(ipf)];

            if (deltaR(pf,lepton) < _dRConeSize)
            { 
                if (std::find(footprint.begin(), footprint.end(), reco::CandidatePtr(pfCandidates,ipf)) != footprint.end())
                {
                    continue;
                }

                if (pf.charge() == 0)
                {
                    if (pf.pdgId() == 22 && pf.pt() > 0.5)
                    {
                        photons += weight*pf.pt();
                    }
                }
                else
                {
                    if (pf.pt() > 0.5)
                    {
                        neutral += weight*pf.pt();
                    }
                    else
                    {
                        if (weight==1)
                        {
                            charged += weight*pf.pt();
                        }
                    }
                }
            }	
        }
        leptonIsolation[ilepton]=(charged + neutral + photons)/lepton.pt();
    }
    std::auto_ptr<edm::ValueMap<double> > out(new edm::ValueMap<double>() );
    edm::ValueMap<double>::Filler filler(*out);
    filler.insert(leptons, leptonIsolation.begin(), leptonIsolation.end() );
	filler.fill();

	iEvent.put(out);
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


//define this as a plug-in
DEFINE_FWK_MODULE(PUPPILeptonIsoProducer);
