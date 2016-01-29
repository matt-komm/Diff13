// system include files
#include <memory>
#include <vector>
// user include files

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TH1F.h"


class EventInfoCollector:
    public edm::EDAnalyzer
{
    private:    
        edm::Service<TFileService> fs;
        
        TH1F* _genweight;
        
        TH1F* _nInteractions0;
        TH1F* _nInteractions1;
        TH1F* _nInteractions2;
        
        TH1F* _nTrueInteractions0;
        TH1F* _nTrueInteractions1;
        TH1F* _nTrueInteractions2;
    
        edm::InputTag _genEventInfoProductInputTag;
        edm::EDGetTokenT<GenEventInfoProduct> _genEventInfoProductToken;
        
        edm::InputTag _pileupSummaryInfoInputTag;
        edm::EDGetTokenT<std::vector<PileupSummaryInfo>> _pileupSummaryInfoToken;
    
    
        virtual void beginJob() override;
        virtual void analyze(const edm::Event&, const edm::EventSetup&);
        virtual void endJob() override;

    public:
        explicit EventInfoCollector(const edm::ParameterSet&);
        ~EventInfoCollector();

        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

};


//
// constructors and destructor
//
EventInfoCollector::EventInfoCollector(const edm::ParameterSet& iConfig)
{
    if (iConfig.exists("GenEventInfo"))
    {
        _genEventInfoProductInputTag = iConfig.getParameter<edm::InputTag>("GenEventInfo");
        _genEventInfoProductToken = this->consumes<GenEventInfoProduct>(_genEventInfoProductInputTag);
        
    }
    
    if (iConfig.exists("PileupSummaryInfo"))
    {
        _pileupSummaryInfoInputTag = iConfig.getParameter<edm::InputTag>("PileupSummaryInfo");
        _pileupSummaryInfoToken = this->consumes<std::vector<PileupSummaryInfo>>(_pileupSummaryInfoInputTag);
    }
    
}


EventInfoCollector::~EventInfoCollector()
{
}

// ------------ method called once each job just before starting event loop  ------------
void 
EventInfoCollector::beginJob()
{
    if (_genEventInfoProductInputTag.label().size()>0)
    {
        _genweight = fs->make<TH1F>("genweight","genweight",2,-1.5,1.5);
    }
    if (_pileupSummaryInfoInputTag.label().size()>0)
    {
        _nInteractions0 = fs->make<TH1F>("nInteractions0","nInteractions0",101,-0.5,100.5);
        _nInteractions1 = fs->make<TH1F>("nInteractions1","nInteractions1",101,-0.5,100.5);
        _nInteractions2 = fs->make<TH1F>("nInteractions2","nInteractions2",101,-0.5,100.5);
        _nTrueInteractions0 = fs->make<TH1F>("nTrueInteractions0","nTrueInteractions0",101,-0.5,100.5);
        _nTrueInteractions1 = fs->make<TH1F>("nTrueInteractions1","nTrueInteractions1",101,-0.5,100.5);
        _nTrueInteractions2 = fs->make<TH1F>("nTrueInteractions2","nTrueInteractions2",101,-0.5,100.5);
    }
}

// ------------ method called to produce the data  ------------
void
EventInfoCollector::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    if (_genEventInfoProductInputTag.label().size()>0)
    {
        edm::Handle<GenEventInfoProduct> genEventInfoProduct;
        iEvent.getByToken(_genEventInfoProductToken, genEventInfoProduct);
        _genweight->Fill(genEventInfoProduct->weight()>0.0 ? 1.0 : -1.0,genEventInfoProduct->weight());
    }
    if (_pileupSummaryInfoInputTag.label().size()>0)
    {
        edm::Handle<std::vector<PileupSummaryInfo>> pileupSummaryInfoCollection;
        iEvent.getByToken(_pileupSummaryInfoToken, pileupSummaryInfoCollection);
        
        for (unsigned int i = 0; i < pileupSummaryInfoCollection->size(); ++i)
        {
            if ((*pileupSummaryInfoCollection)[i].getBunchCrossing()==-1)
            {
                _nInteractions0->Fill((*pileupSummaryInfoCollection)[i].getPU_NumInteractions());
                _nTrueInteractions0->Fill((*pileupSummaryInfoCollection)[i].getTrueNumInteractions());
            }
            else if ((*pileupSummaryInfoCollection)[i].getBunchCrossing()==0)
            {
                _nInteractions1->Fill((*pileupSummaryInfoCollection)[i].getPU_NumInteractions());
                _nTrueInteractions1->Fill((*pileupSummaryInfoCollection)[i].getTrueNumInteractions());
            }
            else if ((*pileupSummaryInfoCollection)[i].getBunchCrossing()==1)
            {
                _nInteractions2->Fill((*pileupSummaryInfoCollection)[i].getPU_NumInteractions());
                _nTrueInteractions2->Fill((*pileupSummaryInfoCollection)[i].getTrueNumInteractions());
            }
        }
    }
}



// ------------ method called once each job just after ending the event loop  ------------
void 
EventInfoCollector::endJob() {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
EventInfoCollector::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}



//define this as a plug-in
DEFINE_FWK_MODULE(EventInfoCollector);
