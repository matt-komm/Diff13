
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "CondFormats/PhysicsToolsObjects/interface/PerformanceWorkingPoint.h"
#include "CondFormats/DataRecord/interface/PerformanceWPRecord.h"
#include "CondFormats/PhysicsToolsObjects/interface/PerformancePayload.h"
#include "CondFormats/DataRecord/interface/PerformancePayloadRecord.h"
#include "CondFormats/PhysicsToolsObjects/interface/BinningPointByMap.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "RecoBTag/PerformanceDB/interface/BtagPerformance.h"

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

class BtagUncertainty:
    public edm::EDProducer
{
    private:
        
        typedef math::XYZTLorentzVector LorentzVector;
        
        const std::string _workingPointES;
        const std::string _scaleFactorES;
        edm::EDGetTokenT<std::vector<pat::Jet>> _jetToken;
        
    public:
        explicit BtagUncertainty(const edm::ParameterSet& config):
            _workingPointES(config.getParameter<std::string>("workingPointES")),
            _scaleFactorES(config.getParameter<std::string>("scaleFactorES")),
            _jetToken(consumes<std::vector<pat::Jet>>(config.getParameter<edm::InputTag>("jetSrc")))
        {
            produces<edm::ValueMap<float>> ("SFB");
            produces<edm::ValueMap<float>> ("SFC");
            produces<edm::ValueMap<float>> ("SFL");
            
            produces<edm::ValueMap<float>> ("SFBerr");
            produces<edm::ValueMap<float>> ("SFCerr");
            produces<edm::ValueMap<float>> ("SFLerr");
        }
        
        ~BtagUncertainty()
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
            edm::ESHandle<PerformanceWorkingPoint> btagWP;
            edmSetup.get<PerformanceWPRecord>().get(_workingPointES,btagWP);
            
            edm::ESHandle<PerformancePayload> btagSF;
            edmSetup.get<PerformancePayloadRecord>().get(_scaleFactorES,btagSF);
            
            BtagPerformance btagPerformance(*btagSF.product(),*btagWP.product());
            
            /*
            BinningPointByMap mapPoint;
            mapPoint.insert(BinningVariables::JetAbsEta, 1.0);
            mapPoint.insert(BinningVariables::JetEt, 30.0);
            
            std::cout<<"eff="<<btagPerformance.getResult(PerformanceResult::BTAGBEFF,mapPoint);
            std::cout<<" +- "<<btagPerformance.getResult(PerformanceResult::BTAGBERR,mapPoint);
            std::cout<<std::endl;
            
            std::cout<<"SF="<<btagPerformance.getResult(PerformanceResult::BTAGBEFFCORR,mapPoint);
            std::cout<<" +- "<<btagPerformance.getResult(PerformanceResult::BTAGBERRCORR,mapPoint);
            std::cout<<std::endl;
            */
            
            edm::Handle<std::vector<pat::Jet>> jetCollection;
            edmEvent.getByToken(_jetToken,jetCollection);
            
            std::vector<float> sf_B(jetCollection->size());
            std::vector<float> sf_C(jetCollection->size());
            std::vector<float> sf_L(jetCollection->size());
            
            std::vector<float> sf_Berr(jetCollection->size());
            std::vector<float> sf_Cerr(jetCollection->size());
            std::vector<float> sf_Lerr(jetCollection->size());
            
            for (unsigned int ijet = 0; ijet < jetCollection->size(); ++ijet)
            {
                const pat::Jet& jet = jetCollection->at(ijet);
                BinningPointByMap mapPoint;
                mapPoint.insert(BinningVariables::JetAbsEta, fabs(jet.eta()));
                mapPoint.insert(BinningVariables::JetEt, jet.pt());
                
                sf_B[ijet]=btagPerformance.getResult(PerformanceResult::BTAGBEFFCORR,mapPoint);
                sf_Berr[ijet]=btagPerformance.getResult(PerformanceResult::BTAGBERRCORR,mapPoint);
                sf_C[ijet]=btagPerformance.getResult(PerformanceResult::BTAGBEFFCORR,mapPoint);
                sf_Cerr[ijet]=btagPerformance.getResult(PerformanceResult::BTAGBERRCORR,mapPoint);
                sf_L[ijet]=btagPerformance.getResult(PerformanceResult::BTAGBEFFCORR,mapPoint);
                sf_Lerr[ijet]=btagPerformance.getResult(PerformanceResult::BTAGBERRCORR,mapPoint);
                
            }
            
            putValueMap(edmEvent,jetCollection,sf_B,"SFB");
            putValueMap(edmEvent,jetCollection,sf_C,"SFC");
            putValueMap(edmEvent,jetCollection,sf_L,"SFL");
            
            putValueMap(edmEvent,jetCollection,sf_Berr,"SFBerr");
            putValueMap(edmEvent,jetCollection,sf_Cerr,"SFCerr");
            putValueMap(edmEvent,jetCollection,sf_Lerr,"SFLerr");
            
        }
        virtual void endJob() override
        {
        }
};


DEFINE_FWK_MODULE(BtagUncertainty);
