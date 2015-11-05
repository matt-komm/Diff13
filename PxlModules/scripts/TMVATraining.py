import os
import ROOT

basePath = os.path.join("/home/fynu/mkomm/Diff13/analysis/split25ns")

rootFilesTrain=[]
rootFilesTest=[]
for f in os.listdir(basePath):
    if f.endswith(".root"):
        if f.find("train")!=-1:
            rootFilesTrain.append(os.path.join(basePath,f))
        elif f.find("test")!=-1:
            rootFilesTest.append(os.path.join(basePath,f))
       
print "found: ",len(rootFilesTrain)," (train) & ",len(rootFilesTest)," (test) files in ",basePath

ROOT.gSystem.Load("/home/fynu/mkomm/Diff13/PxlModules/build/internal/TMVA/libTMVA.so")

ROOT.TMVA.Tools.Instance()
f = ROOT.TFile("test.root","RECREATE")
factory = ROOT.TMVA.Factory("test",f,"!V:Color=False:AnalysisType=Classification")

backgroundTrainChains=[]
backgroundTestChains=[]

signalTrainChains=[]
signalTestChains=[]

for background in [
    "TT_TuneCUETP8M1_13TeV-powheg-pythia8",
    #"ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
    #"ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
    "WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",
    #"DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8"
]:
    chainTrain = ROOT.TChain(background,"")
    backgroundTrainChains.append(chainTrain)
    for rootFile in rootFilesTrain:
        chainTrain.AddFile(rootFile)
    factory.AddBackgroundTree(chainTrain,1.0/0.3,ROOT.TMVA.Types.kTraining)
    
    chainTest = ROOT.TChain(background,"")
    backgroundTestChains.append(chainTest)
    for rootFile in rootFilesTest:
        chainTest.AddFile(rootFile)
    factory.AddBackgroundTree(chainTest,1.0/0.7,ROOT.TMVA.Types.kTesting)
        
for signal in [
    "ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1"
]:
    chainTrain = ROOT.TChain(signal)
    signalTrainChains.append(chainTrain)
    for rootFile in rootFilesTrain:
        chainTrain.AddFile(rootFile)
    factory.AddSignalTree(chainTrain,1.0/0.3,ROOT.TMVA.Types.kTraining)
    
    chainTest = ROOT.TChain(signal)
    signalTestChains.append(chainTest)
    for rootFile in rootFilesTest:
        chainTest.AddFile(rootFile)
    factory.AddSignalTree(chainTest,1.0/0.7,ROOT.TMVA.Types.kTesting)

factory.SetSignalWeightExpression("100.0*mc_weight*(Reconstructed_1__nSelectedJet==2)*(Reconstructed_1__nSelectedBJet==1)*((Generated_1__genweight<0)*(-1)+(Generated_1__genweight>0)*1)")
factory.SetBackgroundWeightExpression("100.0*mc_weight*(Reconstructed_1__nSelectedJet==2)*(Reconstructed_1__nSelectedBJet==1)*((Generated_1__genweight<0)*(-1)+(Generated_1__genweight>0)*1)")
#factory.AddVariable("Reconstructed_1__MET_1__Pt")
factory.AddVariable("SingleTop_1__sumPt_BJet_TightMuon")
factory.AddVariable("SingleTop_1__mtw_beforePz")
factory.AddVariable("SingleTop_1__Top_1__Mass")
#factory.AddVariable("fabs(SingleTop_1__Top_1__Eta)")
#factory.AddVariable("SingleTop_1__Top_1__Pt")
#factory.AddVariable("fabs(SingleTop_1__W_1__Eta)")
#factory.AddVariable("SingleTop_1__W_1__Pt")
factory.AddVariable("SingleTop_1__Dijet_1__DEta")
#factory.AddVariable("SingleTop_1__Dijet_1__Pt")
#factory.AddVariable("SingleTop_1__Dijet_1__Mass")
#factory.AddVariable("SingleTop_1__TightMuon_1__Pt")
factory.AddVariable("SingleTop_1__TightMuon_MET_dPhi")
factory.AddVariable("SingleTop_1__LightJet_1__Pt")
factory.AddVariable("SingleTop_1__absLEta")
factory.AddVariable("SingleTop_1__Shat_1__Mass")

#factory.AddVariable("SingleTop_1__BJet_1__Mass")

#factory.AddVariable("SingleTop_1__BJet_1__user_vtxMass")
#factory.AddVariable("SingleTop_1__LightJet_BJet_dEta")
#factory.AddVariable("SingleTop_1__LightJet_BJet_dPhi")
#factory.AddVariable("SingleTop_1__TightMuon_Neutrino_dEta")
#factory.AddVariable("SingleTop_1__TightMuon_Neutrino_dPhi")

#factory.AddVariable("Reconstructed_1__fox_1_psum")
#factory.AddVariable("Reconstructed_1__fox_2_psum")
#factory.AddVariable("Reconstructed_1__fox_3_psum")
#factory.AddVariable("Reconstructed_1__fox_4_psum")
#factory.AddVariable("Reconstructed_1__wmet_fox_1_psum")
#factory.AddVariable("Reconstructed_1__wmet_fox_2_psum")
#factory.AddVariable("Reconstructed_1__wmet_fox_3_psum")
#factory.AddVariable("Reconstructed_1__wmet_fox_4_psum")

#factory.AddVariable("Reconstructed_1__fox_1_pt")
#factory.AddVariable("Reconstructed_1__fox_2_pt")
#factory.AddVariable("Reconstructed_1__fox_3_pt")
#factory.AddVariable("Reconstructed_1__fox_4_pt")
#factory.AddVariable("Reconstructed_1__wmet_fox_1_pt")
#factory.AddVariable("Reconstructed_1__wmet_fox_2_pt")
#factory.AddVariable("Reconstructed_1__wmet_fox_3_pt")
#factory.AddVariable("Reconstructed_1__wmet_fox_4_pt")

factory.AddVariable("Reconstructed_1__fox_1_eta")
#factory.AddVariable("Reconstructed_1__fox_2_eta")
#factory.AddVariable("Reconstructed_1__fox_3_eta")
#factory.AddVariable("Reconstructed_1__fox_4_eta")
#factory.AddVariable("Reconstructed_1__wmet_fox_1_eta")
#factory.AddVariable("Reconstructed_1__wmet_fox_2_eta")
#factory.AddVariable("Reconstructed_1__wmet_fox_3_eta")
#factory.AddVariable("Reconstructed_1__wmet_fox_4_eta")

#factory.AddVariable("Reconstructed_1__fox_1_shat")
#factory.AddVariable("Reconstructed_1__fox_2_shat")
#factory.AddVariable("Reconstructed_1__fox_3_shat")
#factory.AddVariable("Reconstructed_1__fox_4_shat")
#factory.AddVariable("Reconstructed_1__wmet_fox_1_shat")
#factory.AddVariable("Reconstructed_1__wmet_fox_2_shat")
#factory.AddVariable("Reconstructed_1__wmet_fox_3_shat")
#factory.AddVariable("Reconstructed_1__wmet_fox_4_shat")

factory.AddVariable("Reconstructed_1__fox_1_logpz")
#factory.AddVariable("TMath::Log(Reconstructed_1__fox_2_pz)")
#factory.AddVariable("TMath::Log(Reconstructed_1__fox_3_pz)")
#factory.AddVariable("TMath::Log(Reconstructed_1__fox_4_pz)")
#factory.AddVariable("TMath::Log(Reconstructed_1__wmet_fox_1_pz)")
#factory.AddVariable("TMath::Log(Reconstructed_1__wmet_fox_2_pz)")
#factory.AddVariable("TMath::Log(Reconstructed_1__wmet_fox_3_pz)")
#factory.AddVariable("TMath::Log(Reconstructed_1__wmet_fox_4_pz)")

#factory.AddVariable("Reconstructed_1__sphericity")
factory.AddVariable("Reconstructed_1__isotropy")
#factory.AddVariable("Reconstructed_1__C")

#factory.AddVariable("Reconstructed_1__wmet_sphericity")
#factory.AddVariable("Reconstructed_1__wmet_isotropy")
#factory.AddVariable("Reconstructed_1__wmet_C")

cut = ROOT.TCut("(Reconstructed_1__nSelectedJet==2)*(Reconstructed_1__nSelectedBJet==1)")
factory.PrepareTrainingAndTestTree(cut,"")

factory.BookMethod(ROOT.TMVA.Types.kBDT,"BDT_boost02_minnode001_maxvar2_ntree1000_invboost","BoostType=AdaBoost:AdaBoostBeta=0.2:PruneMethod=CostComplexity:PruneStrength=7:SeparationType=CrossEntropy:MaxDepth=2:nCuts=40:NodePurityLimit=0.5:NTrees=1000:MinNodeSize=1%:NegWeightTreatment=InverseBoostNegWeights")
factory.BookMethod(ROOT.TMVA.Types.kBDT,"BDT_boost03_minnode001_maxvar2_ntree1000_invboost","BoostType=AdaBoost:AdaBoostBeta=0.3:PruneMethod=CostComplexity:PruneStrength=7:SeparationType=CrossEntropy:MaxDepth=2:nCuts=40:NodePurityLimit=0.5:NTrees=1000:MinNodeSize=1%:NegWeightTreatment=InverseBoostNegWeights")
factory.BookMethod(ROOT.TMVA.Types.kBDT,"BDT_boost04_minnode001_maxvar2_ntree1000_invboost","BoostType=AdaBoost:AdaBoostBeta=0.4:PruneMethod=CostComplexity:PruneStrength=7:SeparationType=CrossEntropy:MaxDepth=2:nCuts=40:NodePurityLimit=0.5:NTrees=1000:MinNodeSize=1%:NegWeightTreatment=InverseBoostNegWeights")

factory.BookMethod(ROOT.TMVA.Types.kBDT,"BDT_boost02_minnode005_maxvar2_ntree1000_invboost","BoostType=AdaBoost:AdaBoostBeta=0.2:PruneMethod=CostComplexity:PruneStrength=7:SeparationType=CrossEntropy:MaxDepth=2:nCuts=40:NodePurityLimit=0.5:NTrees=1000:MinNodeSize=5%:NegWeightTreatment=InverseBoostNegWeights")
factory.BookMethod(ROOT.TMVA.Types.kBDT,"BDT_boost03_minnode005_maxvar2_ntree1000_invboost","BoostType=AdaBoost:AdaBoostBeta=0.3:PruneMethod=CostComplexity:PruneStrength=7:SeparationType=CrossEntropy:MaxDepth=2:nCuts=40:NodePurityLimit=0.5:NTrees=1000:MinNodeSize=5%:NegWeightTreatment=InverseBoostNegWeights")
factory.BookMethod(ROOT.TMVA.Types.kBDT,"BDT_boost04_minnode005_maxvar2_ntree1000_invboost","BoostType=AdaBoost:AdaBoostBeta=0.4:PruneMethod=CostComplexity:PruneStrength=7:SeparationType=CrossEntropy:MaxDepth=2:nCuts=40:NodePurityLimit=0.5:NTrees=1000:MinNodeSize=5%:NegWeightTreatment=InverseBoostNegWeights")

factory.BookMethod(ROOT.TMVA.Types.kBDT,"BDT_boost02_minnode010_maxvar2_ntree1000_invboost","BoostType=AdaBoost:AdaBoostBeta=0.2:PruneMethod=CostComplexity:PruneStrength=7:SeparationType=CrossEntropy:MaxDepth=2:nCuts=40:NodePurityLimit=0.5:NTrees=1000:MinNodeSize=10%:NegWeightTreatment=InverseBoostNegWeights")
factory.BookMethod(ROOT.TMVA.Types.kBDT,"BDT_boost03_minnode010_maxvar2_ntree1000_invboost","BoostType=AdaBoost:AdaBoostBeta=0.3:PruneMethod=CostComplexity:PruneStrength=7:SeparationType=CrossEntropy:MaxDepth=2:nCuts=40:NodePurityLimit=0.5:NTrees=1000:MinNodeSize=10%:NegWeightTreatment=InverseBoostNegWeights")
factory.BookMethod(ROOT.TMVA.Types.kBDT,"BDT_boost04_minnode010_maxvar2_ntree1000_invboost","BoostType=AdaBoost:AdaBoostBeta=0.4:PruneMethod=CostComplexity:PruneStrength=7:SeparationType=CrossEntropy:MaxDepth=2:nCuts=40:NodePurityLimit=0.5:NTrees=1000:MinNodeSize=10%:NegWeightTreatment=InverseBoostNegWeights")

factory.TrainAllMethods()
factory.TestAllMethods()
factory.EvaluateAllMethods()



