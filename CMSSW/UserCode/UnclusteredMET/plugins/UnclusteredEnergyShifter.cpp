
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

#include "DataFormats/Math/interface/LorentzVector.h"

#include "DataFormats/Common/interface/View.h"


#include <string>
#include <vector>
#include <cmath>


class UnclusteredEnergyShifter:
    public edm::EDProducer
{
    private:
    
        typedef math::XYZTLorentzVector LorentzVector;
        
        edm::EDGetTokenT<edm::View<pat::Jet>> _jetToken;
        edm::EDGetTokenT<edm::View<pat::MET>> _metToken;
        
        int _shift;

         
    public:
        explicit UnclusteredEnergyShifter(const edm::ParameterSet& config):
            _jetToken(consumes<edm::View<pat::Jet>>(config.getParameter<edm::InputTag>("jetSrc"))),
            _metToken(consumes<edm::View<pat::MET>>(config.getParameter<edm::InputTag>("metSrc"))),
            _shift(config.getParameter<int>("shift"))
        {
            produces<std::vector<pat::MET>>();
        }
        
        ~UnclusteredEnergyShifter()
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

            edm::Handle<edm::View<pat::Jet>> jetCollection;
            edmEvent.getByToken(_jetToken,jetCollection);
           
            edm::Handle<edm::View<pat::MET>> metCollection;
            edmEvent.getByToken(_metToken,metCollection);
           
            std::unique_ptr<std::vector<pat::MET>> output(new std::vector<pat::MET>());
            
            const double metPx = metCollection->at(0).px();
            const double metPy = metCollection->at(0).py();
            
            double jetSumPx = 0.0;
            double jetSumPy = 0.0;
            for (unsigned int ijet = 0; ijet < jetCollection->size(); ++ijet)
            {
                const pat::Jet& jet = jetCollection->at(ijet);
                jetSumPx+=jet.px();
                jetSumPy+=jet.py();
            }
            //sum(jet)+unclusteredE+MET=0
            const double unclusteredPx = -(metPx+jetSumPx);
            const double unclusteredPy = -(metPy+jetSumPy);
            
            pat::MET* newMET = metCollection->at(0).clone();
            reco::Candidate::LorentzVector metP = newMET->p4();
            
            double newPx = 0.0;
            double newPy = 0.0;
            
            if (_shift==0)
            {
                newPx=metP.px();
                newPy=metP.py();
            }
            if (_shift>0)
            {
                newPx = metP.px() + unclusteredPx*0.1;
                newPy = metP.py() + unclusteredPy*0.1;
            }
            else if (_shift<0)
            {
                newPx = metP.px() - unclusteredPx*0.1;
                newPy = metP.py() - unclusteredPy*0.1;
            }
            metP.SetPxPyPzE(
                newPx,
                newPy,
                metP.Pz(),
                std::sqrt(newPx*newPx+newPy*newPy+metP.Pz()*metP.Pz())
            );
            newMET->setP4(metP);
            output->emplace_back(std::move(*newMET));
            edmEvent.put(std::move(output));
        }
        virtual void endJob() override
        {
        }
};


DEFINE_FWK_MODULE(UnclusteredEnergyShifter);

