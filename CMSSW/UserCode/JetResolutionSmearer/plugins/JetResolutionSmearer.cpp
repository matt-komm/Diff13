
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "DataFormats/PatCandidates/interface/Jet.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/Math/interface/LorentzVector.h"

#include "DataFormats/Common/interface/View.h"

#include "PhysicsTools/PatUtils/interface/StringParserTools.h"


#include <string>
#include <vector>


class JetResolutionSmearer:
    public edm::EDProducer
{
    private:
    

        typedef math::XYZTLorentzVector LorentzVector;
        
        edm::EDGetTokenT<edm::View<pat::Jet>> _jetToken;

        struct JERFactor
        {
            double factor;
            StringCutObjectSelector<reco::Candidate> selector;
            
            JERFactor(
                const double& factor,
                StringCutObjectSelector<reco::Candidate>&& selector
            ):
                factor(factor),
                selector(selector)
            {
            }
        };
        
        std::vector<JERFactor> _jerFactors;
        
        double getSF(const reco::Candidate& jet)
        {
            for (const JERFactor& jer: _jerFactors)
            {
                if (jer.selector(jet))
                {
                    return jer.factor;
                }
            }
            return 1.0;
        }
         
    public:
        explicit JetResolutionSmearer(const edm::ParameterSet& config):
            _jetToken(consumes<edm::View<pat::Jet>>(config.getParameter<edm::InputTag>("src")))
        {
            const std::vector<edm::ParameterSet>& smearingFactorCfg = config.getParameter<std::vector<edm::ParameterSet>>("SFs");
            for (const edm::ParameterSet& cfg: smearingFactorCfg)
            {

                _jerFactors.emplace_back(
                    cfg.getParameter<double>("factor"),
                    cfg.getParameter<std::string>("select")
                );
            }
            
            produces<std::vector<pat::Jet>>();
        }
        
        ~JetResolutionSmearer()
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
           
            std::unique_ptr<std::vector<pat::Jet>> output(new std::vector<pat::Jet>(jetCollection->size()));
            for (unsigned int ijet = 0; ijet < jetCollection->size(); ++ijet)
            {
                const pat::Jet* originalJet = &jetCollection->at(ijet);
                pat::Jet* smearedJet = jetCollection->at(ijet).clone();
                if (not edmEvent.isRealData())
                {
                    const reco::GenJet* genJet = originalJet->genJet();
                    if (genJet)
                    {
                        const double SF = getSF(*genJet);
                        const double new_pt = std::max(0.0, genJet->pt() + SF*(originalJet->pt()-genJet->pt()));
                        //using relations: |p|=pT*cosh(eta), pz=pT*sinh(eta), px=pT*cos(phi), py=pT*sin(phi)
                        //with pz^2=|p|^2-pT^2
                        //const double p = new_pt*std::cosh(originalJet->eta());
                        
                        const double phi = originalJet->phi();
                        const double px = new_pt*std::cos(phi);
                        const double py = new_pt*std::sin(phi);
                        const double pz = new_pt*std::sinh(originalJet->eta());
                        
                        const double mass = originalJet->mass();
                        const double energy = std::sqrt(px*px+py*py+pz*pz+mass*mass);
                        
                        reco::Candidate::LorentzVector vec = smearedJet->p4();
                        vec.SetPxPyPzE(px,py,pz,energy);
                        
                        smearedJet->setP4(vec);
                    }
                }
                
                (*output)[ijet]=std::move(*smearedJet);
            }
            edmEvent.put(std::move(output));
        }
        virtual void endJob() override
        {
        }
};


DEFINE_FWK_MODULE(JetResolutionSmearer);

