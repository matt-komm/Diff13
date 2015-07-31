#include <TString.h>
#include <Tools.h>
#include <Factory.h>
#include <MethodBDT.h>
#include <MethodBase.h>
#include <TTree.h>
#include <TFile.h>
#include <TRandom.h>

#include <TNtuple.h>

#include <string>
#include <unordered_map>

int main()
{
    TMVA::Tools::Instance();
    std::string jobName = "TMVA";
    TFile f("test.root","RECREATE");
    TMVA::Factory factory("test",&f,"!V:AnalysisType=Classification");
    
    TTree signal;
    TTree background;
    double var1 = 0;
    double var2 = 0;
    signal.Branch("var1",&var1);
    signal.Branch("var2",&var2);
    background.Branch("var1",&var1);
    background.Branch("var2",&var2);
    for (unsigned int i=0; i<1000; ++i)
    {
        var1=gRandom->Uniform(-1,1);
        var2=gRandom->Gaus(0.0,0.2)*var1+gRandom->Uniform(-0.1,0.1);
        signal.Fill();
        var2=gRandom->Gaus(0.0,1.0)*var1+gRandom->Uniform(-0.2,0.2);
        background.Fill();
    }
    factory.AddSignalTree(&signal,1.0);
    factory.AddBackgroundTree(&background,1.0);
    
    factory.SetSignalWeightExpression("1.0");
    factory.SetBackgroundWeightExpression("1.0");
    
    factory.AddVariable("var1");
    factory.AddVariable("var2");
    
    factory.BookMethod(TMVA::Types::kBDT,"BDT","BoostType=AdaBoost:AdaBoostBeta=0.3");
    factory.TrainAllMethods();
    factory.TestAllMethods();
    factory.EvaluateAllMethods();

}
