
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
#include "DataFormats/Math/interface/LorentzVector.h"

#include "DataFormats/Common/interface/View.h"

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

class JetCorrectionFactorProducer:
    public edm::EDProducer
{
    private:
    
        struct JecCorrection
        {
            std::string level;
            edm::FileInPath file;
        };
        
        
        typedef math::XYZTLorentzVector LorentzVector;
        
        edm::EDGetTokenT<edm::View<pat::Jet>> _jetToken;
        edm::EDGetTokenT<std::vector<reco::Vertex>> _pvToken;
        edm::EDGetTokenT<double> _rhoToken;
        std::string _moduleLabel;
        
        std::vector<JecCorrection> _jecCorrections;
        
        std::vector<JetCorrectorParameters> _jetCorrectionParameters;

        
    public:
        explicit JetCorrectionFactorProducer(const edm::ParameterSet& config):
            _jetToken(consumes<edm::View<pat::Jet>>(config.getParameter<edm::InputTag>("jetSrc"))),
            _pvToken(consumes<std::vector<reco::Vertex>>(config.getParameter<edm::InputTag>("primaryVertices"))),
            _rhoToken(consumes<double>(config.getParameter<edm::InputTag>("rho"))),
            _moduleLabel(config.getParameter<std::string>("@module_label"))
        {
            const std::vector<edm::ParameterSet>& jecCorrectionCfg = config.getParameter<std::vector<edm::ParameterSet>>("jec");
            for (unsigned int ilevel = 0; ilevel <jecCorrectionCfg.size(); ++ilevel)
            {
                JecCorrection jecCorrection;
                jecCorrection.level=jecCorrectionCfg[ilevel].getParameter<std::string>("level");
                jecCorrection.file=edm::FileInPath(jecCorrectionCfg[ilevel].getParameter<std::string>("file"));
                _jecCorrections.push_back(jecCorrection);
            }
            //produces<edm::ValueMap<LorentzVector>> ("jets");
            //produces<edm::ValueMap<LorentzVector>> ("mets");

            for (unsigned int ilevel = 0; ilevel <_jecCorrections.size(); ++ilevel)
            {
                _jetCorrectionParameters.emplace_back(_jecCorrections[ilevel].file.fullPath());
            }
            
            
            produces<edm::ValueMap<pat::JetCorrFactors>>();
        }
        
        ~JetCorrectionFactorProducer()
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
            
            edm::Handle<std::vector<reco::Vertex>> primaryVerticesCollection;
            edmEvent.getByToken(_pvToken,primaryVerticesCollection);
            
            edm::Handle<double> rhoCollection;
            edmEvent.getByToken(_rhoToken,rhoCollection);
            
            FactorizedJetCorrector factorizedJetCorrector(_jetCorrectionParameters);
            
            std::vector<pat::JetCorrFactors> output;
            for (unsigned int ijet = 0; ijet < jetCollection->size(); ++ijet)
            {
                const pat::Jet& jet = jetCollection->at(ijet);
                factorizedJetCorrector.setJetEta(jet.correctedP4(0).eta());
                factorizedJetCorrector.setJetPt(jet.correctedP4(0).pt());
                factorizedJetCorrector.setJetE(jet.correctedP4(0).energy());
                factorizedJetCorrector.setNPV(primaryVerticesCollection->size());
                
                factorizedJetCorrector.setJetPhi(jet.phi());
                factorizedJetCorrector.setJetA(jet.jetArea());
                factorizedJetCorrector.setRho(*rhoCollection);
                
                std::vector<std::pair<std::string, std::vector<float> >> corrections;
                //GLUON, UDS, CHARM, BOTTOM
                corrections.push_back(std::make_pair<std::string, std::vector<float>>("Uncorrected",{1}));
                
                std::vector<float> factors = factorizedJetCorrector.getSubCorrections();
                for (unsigned int ilevel = 0; ilevel < _jecCorrections.size(); ++ilevel)
                {
                    corrections.push_back(std::pair<std::string, std::vector<float>>(_jecCorrections[ilevel].level,{factors[ilevel]}));
                }
                
                output.emplace_back(_moduleLabel,corrections);

            }
            
            putValueMap(edmEvent,jetCollection,output,"");
        }
        virtual void endJob() override
        {
        }
};


DEFINE_FWK_MODULE(JetCorrectionFactorProducer);
