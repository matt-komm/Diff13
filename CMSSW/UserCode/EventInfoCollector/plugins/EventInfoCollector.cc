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

#include "TTree.h"


class EventInfoCollector:
    public edm::EDAnalyzer
{
    private:    
        edm::Service<TFileService> fs;
        TTree* _tree;
        
        float _genweight;
        
        unsigned short _nInteractions0;
        unsigned short _nInteractions1;
        unsigned short _nInteractions2;
        
        float _nTrueInteractions0;
        float _nTrueInteractions1;
        float _nTrueInteractions2;
    
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
    _tree = fs->make<TTree>("info","info");
    if (_genEventInfoProductInputTag.label().size()>0)
    {
        _tree->Branch("genweight",&_genweight);
    }
    if (_pileupSummaryInfoInputTag.label().size()>0)
    {
        _tree->Branch("nInteractions0",&_nInteractions0);
        _tree->Branch("nInteractions1",&_nInteractions1);
        _tree->Branch("nInteractions2",&_nInteractions2);
        
        _tree->Branch("nTrueInteractions0",&_nTrueInteractions0);
        _tree->Branch("nTrueInteractions1",&_nTrueInteractions1);
        _tree->Branch("nTrueInteractions2",&_nTrueInteractions2);
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
        _genweight = genEventInfoProduct->weight();
    }
    if (_pileupSummaryInfoInputTag.label().size()>0)
    {
        edm::Handle<std::vector<PileupSummaryInfo>> pileupSummaryInfoCollection;
        iEvent.getByToken(_pileupSummaryInfoToken, pileupSummaryInfoCollection);
        
        for (unsigned int i = 0; i < pileupSummaryInfoCollection->size(); ++i)
        {
            if ((*pileupSummaryInfoCollection)[i].getBunchCrossing()==-1)
            {
                _nInteractions0=(*pileupSummaryInfoCollection)[i].getPU_NumInteractions();
                _nTrueInteractions0=(*pileupSummaryInfoCollection)[i].getTrueNumInteractions();
            }
            else if ((*pileupSummaryInfoCollection)[i].getBunchCrossing()==0)
            {
                _nInteractions1=(*pileupSummaryInfoCollection)[i].getPU_NumInteractions();
                _nTrueInteractions1=(*pileupSummaryInfoCollection)[i].getTrueNumInteractions();
            }
            else if ((*pileupSummaryInfoCollection)[i].getBunchCrossing()==1)
            {
                _nInteractions2=(*pileupSummaryInfoCollection)[i].getPU_NumInteractions();
                _nTrueInteractions2=(*pileupSummaryInfoCollection)[i].getTrueNumInteractions();
            }
        }
    }
    _tree->Fill();

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
