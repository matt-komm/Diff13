#!/usr/bin/python

import ROOT
import numpy
import random
import math
import os


ROOT.gROOT.Reset()
ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptFit(0)
ROOT.gROOT.Reset()
ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptFit(1111)
ROOT.gStyle.SetPadTopMargin(0.08)
ROOT.gStyle.SetPadLeftMargin(0.13)
ROOT.gStyle.SetPadRightMargin(0.26)
ROOT.gStyle.SetPadBottomMargin(0.15)
ROOT.gStyle.SetMarkerSize(0.16)
ROOT.gStyle.SetHistLineWidth(1)
ROOT.gStyle.SetStatFontSize(0.025)
ROOT.gStyle.SetLabelSize(0.055, "XYZ")

ROOT.gStyle.SetOptFit()
ROOT.gStyle.SetOptStat(0)

# For the canvas:
ROOT.gStyle.SetCanvasBorderMode(0)
ROOT.gStyle.SetCanvasColor(ROOT.kWhite)
ROOT.gStyle.SetCanvasDefH(600) #Height of canvas
ROOT.gStyle.SetCanvasDefW(600) #Width of canvas
ROOT.gStyle.SetCanvasDefX(0)   #POsition on screen
ROOT.gStyle.SetCanvasDefY(0)

# For the Pad:
ROOT.gStyle.SetPadBorderMode(0)
# ROOT.gStyle.SetPadBorderSize(Width_t size = 1)
ROOT.gStyle.SetPadColor(ROOT.kWhite)
#ROOT.gStyle.SetPadGridX(True)
#ROOT.gStyle.SetPadGridY(True)
ROOT.gStyle.SetGridColor(ROOT.kBlack)
ROOT.gStyle.SetGridStyle(2)
ROOT.gStyle.SetGridWidth(1)

# For the frame:

ROOT.gStyle.SetFrameBorderMode(0)
ROOT.gStyle.SetFrameBorderSize(0)
ROOT.gStyle.SetFrameFillColor(0)
ROOT.gStyle.SetFrameFillStyle(0)
ROOT.gStyle.SetFrameLineColor(1)
ROOT.gStyle.SetFrameLineStyle(1)
ROOT.gStyle.SetFrameLineWidth(0)

# For the histo:
# ROOT.gStyle.SetHistFillColor(1)
# ROOT.gStyle.SetHistFillStyle(0)
ROOT.gStyle.SetHistLineColor(1)
ROOT.gStyle.SetHistLineStyle(0)
ROOT.gStyle.SetHistLineWidth(1)
# ROOT.gStyle.SetLegoInnerR(Float_t rad = 0.5)
# ROOT.gStyle.SetNumberContours(Int_t number = 20)

ROOT.gStyle.SetEndErrorSize(2)
#ROOT.gStyle.SetErrorMarker(20)
ROOT.gStyle.SetErrorX(0.)

ROOT.gStyle.SetMarkerStyle(20)
#ROOT.gStyle.SetMarkerStyle(20)

#For the fit/function:
ROOT.gStyle.SetOptFit(1)
ROOT.gStyle.SetFitFormat("5.4g")
ROOT.gStyle.SetFuncColor(2)
ROOT.gStyle.SetFuncStyle(1)
ROOT.gStyle.SetFuncWidth(1)

#For the date:
ROOT.gStyle.SetOptDate(0)
# ROOT.gStyle.SetDateX(Float_t x = 0.01)
# ROOT.gStyle.SetDateY(Float_t y = 0.01)

# For the statistics box:
ROOT.gStyle.SetOptFile(0)
ROOT.gStyle.SetOptStat(0) # To display the mean and RMS:   SetOptStat("mr")
ROOT.gStyle.SetStatColor(ROOT.kWhite)
ROOT.gStyle.SetStatFont(42)
ROOT.gStyle.SetStatFontSize(0.025)
ROOT.gStyle.SetStatTextColor(1)
ROOT.gStyle.SetStatFormat("6.4g")
ROOT.gStyle.SetStatBorderSize(1)
ROOT.gStyle.SetStatH(0.1)
ROOT.gStyle.SetStatW(0.15)
# ROOT.gStyle.SetStaROOT.TStyle(Style_t style = 1001)
# ROOT.gStyle.SetStatX(Float_t x = 0)
# ROOT.gStyle.SetStatY(Float_t y = 0)


#ROOT.gROOT.ForceStyle(True)
#end modified

# For the Global title:

ROOT.gStyle.SetOptTitle(0)

# ROOT.gStyle.SetTitleH(0) # Set the height of the title box
# ROOT.gStyle.SetTitleW(0) # Set the width of the title box
#ROOT.gStyle.SetTitleX(0.35) # Set the position of the title box
#ROOT.gStyle.SetTitleY(0.986) # Set the position of the title box
# ROOT.gStyle.SetTitleStyle(Style_t style = 1001)
#ROOT.gStyle.SetTitleBorderSize(0)

# For the axis titles:
ROOT.gStyle.SetTitleColor(1, "XYZ")
ROOT.gStyle.SetTitleFont(43, "XYZ")
ROOT.gStyle.SetTitleSize(32, "XYZ")
# ROOT.gStyle.SetTitleXSize(Float_t size = 0.02) # Another way to set the size?
# ROOT.gStyle.SetTitleYSize(Float_t size = 0.02)
ROOT.gStyle.SetTitleXOffset(1.135)
#ROOT.gStyle.SetTitleYOffset(1.2)
ROOT.gStyle.SetTitleOffset(1.32, "YZ") # Another way to set the Offset

# For the axis labels:

ROOT.gStyle.SetLabelColor(1, "XYZ")
ROOT.gStyle.SetLabelFont(43, "XYZ")
ROOT.gStyle.SetLabelOffset(0.0077, "XYZ")
ROOT.gStyle.SetLabelSize(28, "XYZ")
#ROOT.gStyle.SetLabelSize(0.04, "XYZ")

# For the axis:

ROOT.gStyle.SetAxisColor(1, "XYZ")
ROOT.gStyle.SetAxisColor(1, "XYZ")
ROOT.gStyle.SetStripDecimals(True)
ROOT.gStyle.SetTickLength(0.03, "Y")
ROOT.gStyle.SetTickLength(0.05, "X")
ROOT.gStyle.SetNdivisions(505, "X")
ROOT.gStyle.SetNdivisions(512, "Y")

ROOT.gStyle.SetPadTickX(1)  # To get tick marks on the opposite side of the frame
ROOT.gStyle.SetPadTickY(1)

# Change for log plots:
ROOT.gStyle.SetOptLogx(0)
ROOT.gStyle.SetOptLogy(0)
ROOT.gStyle.SetOptLogz(0)

#ROOT.gStyle.SetPalette(1) #(1,0)

# another top group addition
ROOT.gStyle.SetHatchesSpacing(1.0)

# Postscript options:
#ROOT.gStyle.SetPaperSize(20., 20.)
#ROOT.gStyle.SetPaperSize(ROOT.TStyle.kA4)
#ROOT.gStyle.SetPaperSize(27., 29.7)
#ROOT.gStyle.SetPaperSize(27., 29.7)
ROOT.TGaxis.SetMaxDigits(3)
ROOT.gStyle.SetLineScalePS(2)

# ROOT.gStyle.SetLineStyleString(Int_t i, const char* text)
# ROOT.gStyle.SetHeaderPS(const char* header)
# ROOT.gStyle.SetTitlePS(const char* pstitle)
#ROOT.gStyle.SetColorModelPS(1)

# ROOT.gStyle.SetBarOffset(Float_t baroff = 0.5)
# ROOT.gStyle.SetBarWidth(Float_t barwidth = 0.5)
# ROOT.gStyle.SetPaintTextFormat(const char* format = "g")
# ROOT.gStyle.SetPalette(Int_t ncolors = 0, Int_t* colors = 0)
# ROOT.gStyle.SetTimeOffset(Double_t toffset)
# ROOT.gStyle.SetHistMinimumZero(kTRUE)
ROOT.gStyle.SetPalette(1)
ROOT.gStyle.SetPaintTextFormat("7.4f")

ROOT.gStyle.SetHatchesSpacing(0.5)
ROOT.gStyle.SetHatchesLineWidth(1)


colors=[]
def newColor(red,green,blue):
    newColor.colorindex+=1
    color=ROOT.TColor(newColor.colorindex,red,green,blue)
    colors.append(color)
    return color
    
newColor.colorindex=301

def getDarkerColor(color):
    darkerColor=newColor(color.GetRed()*0.6,color.GetGreen()*0.6,color.GetBlue()*0.6)
    return darkerColor

sampleDict = {

    "tChannel":
    {
        "processes":[
            "ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1",
        ],
        "color":ROOT.gROOT.GetColor(ROOT.kRed),
        "title":"t-channel",
        "weight":"((Generated_1__genweight<0)*(-1)+(Generated_1__genweight>0)*1)"
    },

    "tWChannel":
    {
        "processes":[
            "ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
            "ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1"
        ],
        "color":ROOT.gROOT.GetColor(ROOT.kOrange),
        "title":"tW-channel",
        "weight":"((Generated_1__genweight<0)*(-1)+(Generated_1__genweight>0)*1)"
    },

    "sChannel":
    {
        "processes":[
            "ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1",
        ],
        "color":ROOT.gROOT.GetColor(ROOT.kYellow),
        "title":"s-channel",
        "weight":"((Generated_1__genweight<0)*(-1)+(Generated_1__genweight>0)*1)"
    },

    "TTJets":
    {
        "processes":[
            "TT_TuneCUETP8M1_13TeV-powheg-pythia8"
        ],
        "color":ROOT.gROOT.GetColor(ROOT.kOrange-3),
        "title":"t#bar{t}",
        "weight":"((Generated_1__genweight<0)*(-1)+(Generated_1__genweight>0)*1)"
    },
    
    "WJets":
    {
        "processes":[
            "WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8"
        ],
        "color":ROOT.gROOT.GetColor(ROOT.kGreen-2),
        "title":"W+jets",
        #"addtitle":"(#times 1.8)",
        "weight":"((Generated_1__genweight<0)*(-1)+(Generated_1__genweight>0)*1)*(1+0.8*(Reconstructed_1__nSelectedJet==2)*(Reconstructed_1__nSelectedBJet==1))"
    },
    
    "DY":
    {
        "processes":[
            "DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8"
        ],
        "color":ROOT.gROOT.GetColor(ROOT.kBlue-1),
        "title":"Drell-Yan",
        #"addtitle":"(#times 1.8)",
        "weight":"((Generated_1__genweight<0)*(-1)+(Generated_1__genweight>0)*1)*(1+0.8*(Reconstructed_1__nSelectedJet==2)*(Reconstructed_1__nSelectedBJet==1))"
    },

    "QCD":
    {
        "processes":[
            "QCD_Pt-20toInf_MuEnrichedPt15_TuneCUETP8M1_13TeV_pythia8",
        ],
        "color":ROOT.gROOT.GetColor(ROOT.kGray),
        "title":"QCD",# #lower[-0.06]{#scale[0.85]{#times#frac{1}{5}}}",
        "weight":"((Generated_1__genweight<0)*(-1)+(Generated_1__genweight>0)*1)"
    },
    
    "data":
    {
        "processes":[
            "SingleMuon_ALL",
        ],
        "color":ROOT.gROOT.GetColor(ROOT.kBlack),
        "title":"data",
        "weight":"1"
    }
}

rootFiles=[]
for f in os.listdir(os.path.join(os.getcwd(),"plot")):
    if f.endswith(".root"):
        rootFiles.append(os.path.join(os.getcwd(),"plot",f))
for f in os.listdir(os.path.join(os.getcwd(),"plotDataDCSBUGRA")):
    if f.endswith(".root"):
        rootFiles.append(os.path.join(os.getcwd(),"plotDataDCSBUGRA",f))

def addUnderflowOverflow(hist):
    hist.SetBinContent(1,hist.GetBinContent(0)+hist.GetBinContent(1))
    N=hist.GetNbinsX()
    hist.SetBinContent(N,hist.GetBinContent(N)+hist.GetBinContent(N+1))
    hist.SetBinContent(0,0)
    hist.SetBinContent(N+1,0)
    hist.SetEntries(hist.GetEntries()-4)
        

for sample in sampleDict.keys():
    sampleDict[sample]["darkColor"]=getDarkerColor(sampleDict[sample]["color"])
'''
        ["fox_1_eta","SingleTop_1__fox_1_eta","H_{1}[#eta]","1",50,0,1],
        ["fox_2_eta","SingleTop_1__fox_2_eta","H_{2}[#eta]","1",50,0,1],
        ["fox_3_eta","SingleTop_1__fox_3_eta","H_{3}[#eta]","1",50,0,1],
        ["fox_4_eta","SingleTop_1__fox_4_eta","H_{4}[#eta]","1",50,0,1],
        ["fox_5_eta","SingleTop_1__fox_5_eta","H_{5}[#eta]","1",50,0,1],
        ["fox_6_eta","SingleTop_1__fox_6_eta","H_{6}[#eta]","1",50,0,1],
        ["fox_7_eta","SingleTop_1__fox_7_eta","H_{7}[#eta]","1",50,0,1],
        ["fox_8_eta","SingleTop_1__fox_8_eta","H_{8}[#eta]","1",50,0,1],
        ["fox_9_eta","SingleTop_1__fox_9_eta","H_{9}[#eta]","1",50,0,1],
        ["fox_10_eta","SingleTop_1__fox_10_eta","H_{10}[#eta]","1",50,0,1],
        
        ["fox_1_one","SingleTop_1__fox_1_one","H_{1}[1]","1",50,0,25],
        ["fox_2_one","SingleTop_1__fox_2_one","H_{2}[1]","1",50,0,25],
        ["fox_3_one","SingleTop_1__fox_3_one","H_{3}[1]","1",50,0,20],
        ["fox_4_one","SingleTop_1__fox_4_one","H_{4}[1]","1",50,0,20],
        ["fox_5_one","SingleTop_1__fox_5_one","H_{5}[1]","1",50,0,15],
        ["fox_6_one","SingleTop_1__fox_6_one","H_{6}[1]","1",50,0,15],
        ["fox_7_one","SingleTop_1__fox_7_one","H_{7}[1]","1",50,0,15],
        ["fox_8_one","SingleTop_1__fox_8_one","H_{8}[1]","1",50,0,15],
        ["fox_9_one","SingleTop_1__fox_9_one","H_{9}[1]","1",50,0,10],
        ["fox_10_one","SingleTop_1__fox_10_one","H_{10}[1]","1",50,0,10],
        
        ["fox_1_psum","SingleTop_1__fox_1_psum","H_{1}[#Sigma p]","1",50,0,1],
        ["fox_2_psum","SingleTop_1__fox_2_psum","H_{2}[#Sigma p]","1",50,0,1],
        ["fox_3_psum","SingleTop_1__fox_3_psum","H_{3}[#Sigma p]","1",50,0,1],
        ["fox_4_psum","SingleTop_1__fox_4_psum","H_{4}[#Sigma p]","1",50,0,1],
        ["fox_5_psum","SingleTop_1__fox_5_psum","H_{5}[#Sigma p]","1",50,0,1],
        ["fox_6_psum","SingleTop_1__fox_6_psum","H_{6}[#Sigma p]","1",50,0,1],
        ["fox_7_psum","SingleTop_1__fox_7_psum","H_{7}[#Sigma p]","1",50,0,1],
        ["fox_8_psum","SingleTop_1__fox_8_psum","H_{8}[#Sigma p]","1",50,0,1],
        ["fox_9_psum","SingleTop_1__fox_9_psum","H_{9}[#Sigma p]","1",50,0,1],
        ["fox_10_psum","SingleTop_1__fox_10_psum","H_{10}[#Sigma p]","1",50,0,1],
        
        ["fox_1_pt","SingleTop_1__fox_1_pt","H_{1}[#Sigma p_{T}]","1",50,0,1],
        ["fox_2_pt","SingleTop_1__fox_2_pt","H_{2}[#Sigma p_{T}]","1",50,0,1],
        ["fox_3_pt","SingleTop_1__fox_3_pt","H_{3}[#Sigma p_{T}]","1",50,0,1],
        ["fox_4_pt","SingleTop_1__fox_4_pt","H_{4}[#Sigma p_{T}]","1",50,0,1],
        ["fox_5_pt","SingleTop_1__fox_5_pt","H_{5}[#Sigma p_{T}]","1",50,0,1],
        ["fox_6_pt","SingleTop_1__fox_6_pt","H_{6}[#Sigma p_{T}]","1",50,0,1],
        ["fox_7_pt","SingleTop_1__fox_7_pt","H_{7}[#Sigma p_{T}]","1",50,0,1],
        ["fox_8_pt","SingleTop_1__fox_8_pt","H_{8}[#Sigma p_{T}]","1",50,0,1],
        ["fox_9_pt","SingleTop_1__fox_9_pt","H_{9}[#Sigma p_{T}]","1",50,0,1],
        ["fox_10_pt","SingleTop_1__fox_10_pt","H_{10}[#Sigma p_{T}]","1",50,0,1],
        
        ["fox_1_pz","TMath::Log(SingleTop_1__fox_1_pz)","ln(H_{1}[#Sigma p_{z}])","1",50,-3,9],
        ["fox_2_pz","TMath::Log(SingleTop_1__fox_2_pz)","ln(H_{2}[#Sigma p_{z}])","1",50,-3,9],
        ["fox_3_pz","TMath::Log(SingleTop_1__fox_3_pz)","ln(H_{3}[#Sigma p_{z}])","1",50,-3,9],
        ["fox_4_pz","TMath::Log(SingleTop_1__fox_4_pz)","ln(H_{4}[#Sigma p_{z}])","1",50,-3,9],
        ["fox_5_pz","TMath::Log(SingleTop_1__fox_5_pz)","ln(H_{5}[#Sigma p_{z}])","1",50,-3,9],
        ["fox_6_pz","TMath::Log(SingleTop_1__fox_6_pz)","ln(H_{6}[#Sigma p_{z}])","1",50,-3,9],
        ["fox_7_pz","TMath::Log(SingleTop_1__fox_7_pz)","ln(H_{7}[#Sigma p_{z}])","1",50,-3,9],
        ["fox_8_pz","TMath::Log(SingleTop_1__fox_8_pz)","ln(H_{8}[#Sigma p_{z}])","1",50,-3,9],
        ["fox_9_pz","TMath::Log(SingleTop_1__fox_9_pz)","ln(H_{9}[#Sigma p_{z}])","1",50,-3,9],
        ["fox_10_pz","TMath::Log(SingleTop_1__fox_10_pz)","ln(H_{10}[#Sigma p_{z}])","1",50,-3,9],

        ["fox_1_shat","TMath::Log(SingleTop_1__fox_1_shat)","ln(H_{1}[#hat{s}])","1",50,-3.5,6.5],
        ["fox_2_shat","TMath::Log(SingleTop_1__fox_2_shat)","ln(H_{2}[#hat{s}])","1",50,-3.5,6.5],
        ["fox_3_shat","TMath::Log(SingleTop_1__fox_3_shat)","ln(H_{3}[#hat{s}])","1",50,-3.5,6.5],
        ["fox_4_shat","TMath::Log(SingleTop_1__fox_4_shat)","ln(H_{4}[#hat{s}])","1",50,-3.5,6.5],
        ["fox_5_shat","TMath::Log(SingleTop_1__fox_5_shat)","ln(H_{5}[#hat{s}])","1",50,-3.5,6.5],
        ["fox_6_shat","TMath::Log(SingleTop_1__fox_6_shat)","ln(H_{6}[#hat{s}])","1",50,-3.5,6.5],
        ["fox_7_shat","TMath::Log(SingleTop_1__fox_7_shat)","ln(H_{7}[#hat{s}])","1",50,-3.5,6.5],
        ["fox_8_shat","TMath::Log(SingleTop_1__fox_8_shat)","ln(H_{8}[#hat{s}])","1",50,-3.5,6.5],
        ["fox_9_shat","TMath::Log(SingleTop_1__fox_9_shat)","ln(H_{9}[#hat{s}])","1",50,-3.5,6.5],
        ["fox_10_shat","TMath::Log(SingleTop_1__fox_10_shat)","ln(H_{10}[#hat{s}])","1",50,-3.5,6.5],

        ["light_jet_nhf","(SingleTop_1__LightJet_1__neutralHadronMultiplicity>0)*SingleTop_1__LightJet_1__neutralHadronEnergyFraction","neutral hadron frac.","","1",30,0,1],
        ["light_jet_hff","(SingleTop_1__LightJet_1__HFHadronMultiplicity>0)*SingleTop_1__LightJet_1__HFHadronEnergyFraction","neutral hadron HF frac.","","1",30,0,1],
        ["light_jet_nhf_hff","(SingleTop_1__LightJet_1__neutralHadronMultiplicity>0)*SingleTop_1__LightJet_1__neutralHadronEnergyFraction+(SingleTop_1__LightJet_1__HFHadronMultiplicity>0)*SingleTop_1__LightJet_1__HFHadronEnergyFraction","neutral hadron+HF frac.","","1",30,0,1],
        
        ["light_jet_nem","(SingleTop_1__LightJet_1__neutralMultiplicity>0)*SingleTop_1__LightJet_1__neutralEmEnergyFraction","neutral EM frac.","","1",30,0,1],
        
        ["light_jet_neutralHadronEnergy","(SingleTop_1__LightJet_1__neutralHadronMultiplicity>0)*SingleTop_1__LightJet_1__neutralHadronEnergy","neutral hadron energy","GeV","1",30,0,500],
        ["light_jet_HFHadronEnergy","(SingleTop_1__LightJet_1__HFHadronMultiplicity>0)*SingleTop_1__LightJet_1__HFHadronEnergy","neutral hadron HF energy","GeV","1",30,0,3500],
        ["light_jet_neutralEmEnergy","(SingleTop_1__LightJet_1__neutralMultiplicity>0)*SingleTop_1__LightJet_1__neutralEmEnergy","EM energy","GeV","1",30,0,500],
        
        ["light_jet_E","SingleTop_1__LightJet_1__E","forward jet energy","GeV","1",30,0,2500],




'''


for category in [
    ["2j0t","(Reconstructed_1__nSelectedJet==2)*(Reconstructed_1__nSelectedBJet==0)"],
    #["2j1t","(Reconstructed_1__nSelectedJet==2)*(Reconstructed_1__nSelectedBJet==1)"],
    ["3j0t","(Reconstructed_1__nSelectedJet==3)*(Reconstructed_1__nSelectedBJet==0)"],
    ["3j1t","(Reconstructed_1__nSelectedJet==3)*(Reconstructed_1__nSelectedBJet==1)"],
    ["3j2t","(Reconstructed_1__nSelectedJet==3)*(Reconstructed_1__nSelectedBJet==2)"]
]:
    for var in [    

        ["nVertices","Reconstructed_1__PU_1__nVertices","#vertices","","1",51,-0.5,50.5],
        ["chi2ndof","(Reconstructed_1__PU_1__PVndof>0)*Reconstructed_1__PU_1__PVchi/Reconstructed_1__PU_1__PVndof","PV #chi^{2}/ndof","","1",50,0,2.0],
        ["PVz","Reconstructed_1__PU_1__PVz","PV z","cm","1",50,-25.0,25.0],
        ["muon_pt","SingleTop_1__TightMuon_1__Pt","muon p_{T}","GeV","1",50,0,200],
        ["muon_eta","SingleTop_1__TightMuon_1__Eta","muon #eta","","1",50,-2.6,2.6],
        ["muon_abseta","fabs(SingleTop_1__TightMuon_1__Eta)","muon |#eta|","","1",50,0,2.6],
        ["ljet_pt","SingleTop_1__LightJet_1__Pt","light jet p_{T}","GeV","1",50,20,350],
        ["ljet_eta","SingleTop_1__LightJet_1__Eta","light jet #eta","","1",50,-5.0,5.0],
        ["ljet_abseta","fabs(SingleTop_1__LightJet_1__Eta)","light jet |#eta|","","1",50,0,5.0],
        ["ljet_mass","SingleTop_1__LightJet_1__Mass","light jet mass","GeV","1",50,0.0,50.0],
        ["bjet_pt","SingleTop_1__BJet_1__Pt","b-jet p_{T}","GeV","1",50,0,250],
        ["bjet_eta","SingleTop_1__BJet_1__Eta","b-jet #eta","","1",50,-2.5,2.5],
        ["bjet_abseta","fabs(SingleTop_1__BJet_1__Eta)","b-jet |#eta|","","1",50,0,2.5],
        ["bjet_mass","SingleTop_1__BJet_1__Mass","b-jet mass","GeV","1",50,0.0,50.0],
        
        ["met","Reconstructed_1__MET_1__Pt","MET","GeV","1",50,0,200],
        #["met","Reconstructed_1__PuppiMET_1__Pt","MET","GeV","1",50,0,200],
        ["met_phi","Reconstructed_1__MET_1__Phi","MET #phi","","1",50,-3.2,3.2],
        #["met_phi","Reconstructed_1__PuppiMET_1__Phi","MET #phi","","1",50,-3.2,3.2],
        ["mtw","SingleTop_1__mtw_beforePz","MTW","GeV","1",50,0,200],
        
        ["neutrino_pt","SingleTop_1__Neutrino_1__Pt","neutrino p_{T}","GeV","1",50,0,250],
        ["neutrino_eta","SingleTop_1__Neutrino_1__Eta","neutrino #eta","","1",50,-5.0,5.0],
        ["neutrino_abseta","fabs(SingleTop_1__Neutrino_1__Eta)","neutrino |#eta|","","1",50,0,5.0],
        ["neutrino_mass","SingleTop_1__Neutrino_1__Mass","neutrino mass","","1",50,0.0,0.03],
        
        ["w_pt","SingleTop_1__W_1__Pt","W boson p_{T}","GeV","1",50,0,300],
        ["w_eta","SingleTop_1__W_1__Eta","W boson #eta","","1",50,-5,5],
        ["w_abseta","fabs(SingleTop_1__W_1__Eta)","W boson |#eta|","","1",50,0,5],
        
        ["top_pt","SingleTop_1__Top_1__Pt","top p_{T}","GeV","1",50,0,350],
        ["top_eta","SingleTop_1__Top_1__Eta","top #eta","","1",50,-6,6],
        ["top_abseta","fabs(SingleTop_1__Top_1__Eta)","top |#eta|","","1",50,0,6],
        ["top_mass","SingleTop_1__Top_1__Mass","top mass","GeV","1",50,0,800],
        
        #["hadronic_mass","SingleTop_1__Hadronic_1__Mass","hadronic mass","","1",50,0,1000],
        #["hadronic_eta","SingleTop_1__Hadronic_1__Eta","hadronic #eta","","1",50,-7,7],
        #["hadronic_abseta","fabs(SingleTop_1__Hadronic_1__Eta)","hadronic |#eta|","","1",50,0,7],
        #["hadronic_pt","SingleTop_1__Hadronic_1__Pt","hadronic p_{T}","","1",50,0,350],
        #["hadronic_minCosTheta","SingleTop_1__Hadronic_1__minCosTheta","cos#theta_{min}(hadronic)","","1",50,-1,1],
        #["hadronic_minDY","SingleTop_1__Hadronic_1__minDY","#Delta y_{min}(hadronic)","","1",50,0,8],
        #["hadronic_minDR","SingleTop_1__Hadronic_1__minDR","#Delta R_{min}(hadronic)","","1",50,0,7],
        #["hadronic_minDPhi","SingleTop_1__Hadronic_1__minDPhi","#Delta #phi_{min}(hadronic)","","1",50,0,3.2],
        #["hadronic_maxCosTheta","SingleTop_1__Hadronic_1__maxCosTheta","cos#theta_{max}(hadronic)","","1",50,-1,1],
        #["hadronic_maxDY","SingleTop_1__Hadronic_1__maxDY","#Delta y_{max}(hadronic)","","1",50,0,8],
        #["hadronic_maxDR","SingleTop_1__Hadronic_1__maxDR","#Delta R_{max}(hadronic)","","1",50,0,7],
        #["hadronic_maxDPhi","SingleTop_1__Hadronic_1__maxDPhi","#Delta #phi_{max}(hadronic)","","1",50,0,3.2],  
        
        ["dijet_mass","SingleTop_1__Dijet_1__Mass","dijet mass","GeV","1",50,0,800],
        ["dijet_eta","SingleTop_1__Dijet_1__Eta","dijet #eta","","1",50,-6.5,6.5],
        ["dijet_abseta","fabs(SingleTop_1__Dijet_1__Eta)","dijet |#eta|","","1",50,0,6.5],
        ["dijet_pt","SingleTop_1__Dijet_1__Pt","dijet p_{T}","GeV","1",50,0,350],
        ["dijet_cosTheta","SingleTop_1__Dijet_1__CosTheta","cos#theta(lj,bj)","","1",50,-1,1],
        ["dijet_deltaY","SingleTop_1__Dijet_1__DY","#Delta y(lj,bj)","","1",50,0,8],
        ["dijet_deltaEta","SingleTop_1__Dijet_1__DEta","#Delta #eta(lj,bj)","","1",50,0,8],
        ["dijet_deltaR","SingleTop_1__Dijet_1__DR","#Delta R(lj,bj)","","1",50,0,7],
        ["dijet_deltaPhi","SingleTop_1__Dijet_1__DPhi","#Delta #phi(lj,bj)","","1",50,0,3.2],
        
        ["shat_mass","SingleTop_1__Shat_1__Mass","#hat{s}","GeV","1",50,0,1500],
        ["shat_eta","SingleTop_1__Shat_1__Eta","#hat{s} #eta","","1",50,-7,7],
        ["shat_abseta","fabs(SingleTop_1__Shat_1__Eta)","#hat{s} |#eta|","","1",50,0,7],
        ["shat_pt","SingleTop_1__Shat_1__Pt","#hat{s} p_{T}","GeV","1",50,0,300],
        ["shat_minCosTheta","SingleTop_1__Shat_1__minCosTheta","cos#theta_{min}(#hat{s})","","1",50,-1,1],
        ["shat_minDY","SingleTop_1__Shat_1__minDY","#Delta y_{min}(#hat{s})","","1",50,0,5],
        ["shat_minDR","SingleTop_1__Shat_1__minDR","#Delta R_{min}(#hat{s})","","1",50,0,7],
        ["shat_minDPhi","SingleTop_1__Shat_1__minDPhi","#Delta #phi_{min}(#hat{s})","","1",50,0,3.2],
        ["shat_maxCosTheta","SingleTop_1__Shat_1__maxCosTheta","cos#theta_{max}(#hat{s})","","1",50,-1,1],
        ["shat_maxDY","SingleTop_1__Shat_1__maxDY","#Delta y_{max}(#hat{s})","","1",50,0,8],
        ["shat_maxDR","SingleTop_1__Shat_1__maxDR","#Delta R_{max}(#hat{s})","","1",50,0,7],
        ["shat_maxDPhi","SingleTop_1__Shat_1__maxDPhi","#Delta #phi_{max}(#hat{s})","","1",50,0,3.2],
        
        #["aplanarity","SingleTop_1__aplanarity","aplanarity","","1",50,0,0.4],
        #["circularity","SingleTop_1__circularity","circularity","","1",50,0,1],
        #["C","SingleTop_1__C","C","","1",50,0,1],
        #["isotropy","SingleTop_1__isotropy","isotropy","","1",50,0,1],
        #["sphericity","SingleTop_1__sphericity","sphericity","","1",50,0,1],
        
        ["cosTheta_tPL","SingleTop_1__cosTheta_tPL","cos#theta_{l,q}^{top}","","1",25,-1,1],
        ["cosTheta_tPB","SingleTop_1__cosTheta_tPB","cos#theta_{l,b}^{top}","","1",25,-1,1],
        ["cosTheta_tPN","SingleTop_1__cosTheta_tPN","cos#theta_{l,#nu}^{top}","","1",25,-1,1],
        
        ["cosTheta_wH","SingleTop_1__cosTheta_wH","cos#theta_{helicity}^{W}","","1",25,-1,1],
        ["cosTheta_wT","SingleTop_1__cosTheta_wT","cos#theta_{transverse}^{W}","","1",25,-1,1],
        ["cosTheta_wN","SingleTop_1__cosTheta_wN","cos#theta_{normal}^{W}","","1",25,-1,1], 
        
                        
    ]:
        for qcd in [
            ["qcdnone","1",""],
            #["qcdnone_central","(fabs(SingleTop_1__LightJet_1__Eta)<3.0)","|#eta|<3"],
            #["qcdnone_forward","(fabs(SingleTop_1__LightJet_1__Eta)>3.0)","|#eta|>3"],
            ["qcdmtw","(SingleTop_1__mtw_beforePz>50.0)","MTW>50 GeV"],
            #["qcdmtw_central","(SingleTop_1__mtw_beforePz>50.0)*(fabs(SingleTop_1__LightJet_1__Eta)<3.0)","MTW>50 GeV, |#eta|<3"],
            #["qcdmtw_forward","(SingleTop_1__mtw_beforePz>50.0)*(fabs(SingleTop_1__LightJet_1__Eta)>3.0)","MTW>50 GeV, |#eta|>3"],
        ]:
            print category[0],var[0],qcd[0]
            
            outputName=category[0]+"_"+var[0]+"_"+qcd[0]
            variableName=var[1]
            variableTitle=var[2]
            unit=var[3]
            weight=var[4]+"*"+category[1]+"*"+qcd[1]
            nbins=var[5]
            xmin=var[6]
            xmax=var[7]
            
            stackMC = ROOT.THStack()
            
            
            legendEntries=[]
            
            sumHistMC = None
            
            for sampleName in ["tChannel","tWChannel","TTJets","WJets","DY","QCD"]:
                sample=sampleDict[sampleName]
                sampleHist=ROOT.TH1F("sampleHist"+sampleName+str(random.random()),"",nbins,xmin,xmax)
                sampleHist.Sumw2()
                sampleHist.SetFillColor(sample["color"].GetNumber())
                sampleHist.SetFillStyle(1001)
                sampleHist.SetLineStyle(1)
                sampleHist.SetLineWidth(2)
                sampleHist.SetLineColor(sample["darkColor"].GetNumber())
                sampleHist.SetDirectory(0)
                for process in sample["processes"]:
                    for f in rootFiles:
                        rootFile = ROOT.TFile(f)
                        tree = rootFile.Get(process)
                        if (tree):
                            tempHist=sampleHist.Clone()
                            tempHist.SetName(sampleHist.GetName()+process+str(random.random()))
                            tree.Project(tempHist.GetName(),variableName,"41.0*mc_weight*"+sample["weight"]+"*"+weight)
                            tempHist.SetDirectory(0)
                            addUnderflowOverflow(tempHist)
                            sampleHist.Add(tempHist)
                        rootFile.Close()
                        
                if sumHistMC==None:
                    sumHistMC=sampleHist.Clone()
                else:
                    sumHistMC.Add(sampleHist)
                
                print "\t",sampleName,sampleHist.GetEntries(),sampleHist.Integral()
                stackMC.Add(sampleHist,"HIST F")
                if sample.has_key("addtitle"):
                    legendEntries.append(["",sample["addtitle"],""])
                legendEntries.append([sampleHist,sample["title"],"F"])
                
                
                
            sumHistData = None
                
            stackData = ROOT.THStack()
            for sampleName in ["data"]:
                sample=sampleDict[sampleName]
                sampleHist=ROOT.TH1F("sampleHist"+sampleName+str(random.random()),"",nbins,xmin,xmax)
                sampleHist.Sumw2()
                sampleHist.SetMarkerColor(sample["color"].GetNumber())
                sampleHist.SetMarkerStyle(20)
                sampleHist.SetMarkerSize(1)
                sampleHist.SetFillStyle(0)
                sampleHist.SetLineStyle(1)
                sampleHist.SetLineWidth(1)
                sampleHist.SetDirectory(0)
                for process in sample["processes"]:
                    for f in rootFiles:
                        rootFile = ROOT.TFile(f)
                        tree = rootFile.Get(process)
                        if (tree):
                            tempHist=sampleHist.Clone()
                            tempHist.SetName(sampleHist.GetName()+process+str(random.random()))
                            tree.Project(tempHist.GetName(),variableName,sample["weight"]+"*"+weight)
                            tempHist.SetDirectory(0)
                            addUnderflowOverflow(tempHist)
                            sampleHist.Add(tempHist)
                        rootFile.Close()
                        
                if sumHistData==None:
                    sumHistData=sampleHist.Clone()
                else:
                    sumHistData.Add(sampleHist)
                
                print "\t",sampleName,sampleHist.GetEntries(),sampleHist.Integral()
                stackData.Add(sampleHist,"PE")
                legendEntries.append([sampleHist,sample["title"],"PE"])
                
            print "\tMC=",sumHistMC.Integral(),", data=",sumHistData.Integral(),", L=",sumHistData.Integral()/sumHistMC.Integral()
            
            
            cv = ROOT.TCanvas("cv"+outputName+str(random.random()),"",800,600)
            cv.Divide(1,2,0,0)
            cv.GetPad(1).SetPad(0.0, 0.0, 1.0, 1.0)
            cv.GetPad(1).SetFillStyle(4000)
            cv.GetPad(2).SetPad(0.0, 0.00, 1.0,1.0)
            cv.GetPad(2).SetFillStyle(4000)
            
            cvxmin=0.14
            cvxmax=0.74
            cvymin=0.14
            cvymax=0.92
            resHeight=0.35
            
            for i in range(1,3):
                #for the canvas:
                cv.GetPad(i).SetBorderMode(0)
                cv.GetPad(i).SetGridx(False)
                cv.GetPad(i).SetGridy(False)


                #For the frame:
                cv.GetPad(i).SetFrameBorderMode(0)
                cv.GetPad(i).SetFrameBorderSize(1)
                cv.GetPad(i).SetFrameFillColor(0)
                cv.GetPad(i).SetFrameFillStyle(0)
                cv.GetPad(i).SetFrameLineColor(1)
                cv.GetPad(i).SetFrameLineStyle(1)
                cv.GetPad(i).SetFrameLineWidth(1)

                # Margins:
                cv.GetPad(i).SetLeftMargin(cvxmin)
                cv.GetPad(i).SetRightMargin(1-cvxmax)
                
                # For the Global title:
                cv.GetPad(i).SetTitle("")
                
                # For the axis:
                cv.GetPad(i).SetTickx(1)  # To get tick marks on the opposite side of the frame
                cv.GetPad(i).SetTicky(1)

                # Change for log plots:
                cv.GetPad(i).SetLogx(0)
                cv.GetPad(i).SetLogy(0)
                cv.GetPad(i).SetLogz(0)
            
            
            
            cv.GetPad(2).SetTopMargin(1-cvymax)
            cv.GetPad(2).SetBottomMargin(resHeight)
            cv.GetPad(1).SetTopMargin(1-resHeight)
            cv.GetPad(1).SetBottomMargin(cvymin)
            
            cv.cd(2)
            
            if unit!="":
                axis=ROOT.TH2F("axis"+str(random.random()),";"+variableTitle+" ("+unit+");events",50,xmin,xmax,50,0.0,1.1*max([sumHistMC.GetMaximum(),sumHistData.GetMaximum(),1.0]))
            else:
                axis=ROOT.TH2F("axis"+str(random.random()),";"+variableTitle+";events",50,xmin,xmax,50,0.0,1.1*max([sumHistMC.GetMaximum(),sumHistData.GetMaximum(),1.0]))
            axis.GetYaxis().SetNdivisions(506)
            axis.GetXaxis().SetNdivisions(504)
            axis.GetXaxis().SetLabelSize(0)
            axis.GetXaxis().SetTitle("")
            axis.GetXaxis().SetTickLength(0.025/(1-cv.GetPad(2).GetLeftMargin()-cv.GetPad(2).GetRightMargin()))
            axis.GetYaxis().SetTickLength(0.015/(1-cv.GetPad(2).GetTopMargin()-cv.GetPad(2).GetBottomMargin()))
            axis.GetYaxis().SetNoExponent(True)
            axis.Draw("AXIS")
            stackMC.Draw("Same")
            stackMC.GetXaxis().SetTitle(variableTitle)
            stackMC.GetYaxis().SetTitle("events")
            
            stackData.Draw("Same")
            #cv.GetPad(2).SetLogy(1)
            ROOT.gPad.RedrawAxis()
            
            legend = ROOT.TLegend(0.745,0.9,0.99,0.745-0.052*len(legendEntries))
            legend.SetFillColor(ROOT.kWhite)
            legend.SetBorderSize(0)
            legend.SetTextFont(43)
            legend.SetTextSize(30)
            for entry in reversed(legendEntries):
                legend.AddEntry(entry[0],entry[1],entry[2])
                
            pText=ROOT.TPaveText(0.13,0.94,0.13,0.94,"NDC")
            pText.SetFillColor(ROOT.kWhite)
            pText.SetBorderSize(0)
            pText.SetTextFont(63)
            pText.SetTextSize(26)
            pText.SetTextAlign(11)
            pText.AddText(qcd[2])
            pText.Draw("Same")
            
            
            pCat=ROOT.TPaveText(0.47,0.94,0.47,0.94,"NDC")
            pCat.SetFillColor(ROOT.kWhite)
            pCat.SetBorderSize(0)
            pCat.SetTextFont(63)
            pCat.SetTextSize(30)
            pCat.SetTextAlign(31)
            pCat.AddText(category[0])
            pCat.Draw("Same")
            
            pLumi=ROOT.TPaveText(0.74,0.94,0.74,0.94,"NDC")
            pLumi.SetFillColor(ROOT.kWhite)
            pLumi.SetBorderSize(0)
            pLumi.SetTextFont(43)
            pLumi.SetTextSize(30)
            pLumi.SetTextAlign(31)
            pLumi.AddText("41pb#lower[-0.7]{#scale[0.7]{-1}} (13TeV)")
            pLumi.Draw("Same")
            
            
            
            cv.cd(1)
            axisRes=None
            if unit!="":
                axisRes=ROOT.TH2F("axisRes"+str(random.random()),";"+variableTitle+" ("+unit+");data/MC",50,xmin,xmax,50,0.2,1.8)
            else:
                axisRes=ROOT.TH2F("axisRes"+str(random.random()),";"+variableTitle+";data/MC",50,xmin,xmax,50,0.2,1.8)
            axisRes.GetYaxis().SetNdivisions(406)
            axisRes.GetXaxis().SetNdivisions(504)
            axisRes.GetXaxis().SetTickLength(0.025/(1-cv.GetPad(1).GetLeftMargin()-cv.GetPad(1).GetRightMargin()))
            axisRes.GetYaxis().SetTickLength(0.015/(1-cv.GetPad(1).GetTopMargin()-cv.GetPad(1).GetBottomMargin()))

            axisRes.Draw("AXIS")
            
            
            
            rootObj=[]
            sumHistRes=sumHistData.Clone()
            sumHistRes.Divide(sumHistMC)
            for ibin in range(sumHistData.GetNbinsX()):
                c = sumHistMC.GetBinCenter(ibin+1)
                w = sumHistMC.GetBinWidth(ibin+1)
                if sumHistData.GetBinContent(ibin+1)>0:
                    h = min(sumHistMC.GetBinError(ibin+1)/sumHistData.GetBinContent(ibin+1),0.8)
                    box = ROOT.TBox(c-0.5*w,1-h,c+0.5*w,1+h)
                    box.SetFillStyle(3244)
                    box.SetLineColor(ROOT.kGray+1)
                    box.SetFillColor(ROOT.kGray)
                    rootObj.append(box)
                    box.Draw("SameF")
                    box2 = ROOT.TBox(c-0.5*w,1-h,c+0.5*w,1+h)
                    box2.SetFillStyle(0)
                    box2.SetLineColor(ROOT.kGray+1)
                    box2.SetFillColor(ROOT.kGray)
                    rootObj.append(box2)
                    box2.Draw("SameL")

            sumHistRes.Draw("Same")
            if len(rootObj)>0:
                legend.AddEntry(rootObj[0],"MC stat.","F")
            legend.Draw("Same")
            
            axisLine = ROOT.TF1("axisLine"+str(random.random()),"1",xmin,xmax)
            axisLine.SetLineColor(ROOT.kBlack)
            axisLine.SetLineWidth(1)
            axisLine.Draw("SameL")
            ROOT.gPad.RedrawAxis()
            
            hidePave=ROOT.TPaveText(0.07,resHeight-0.04,cvxmin-0.005,resHeight+0.03,"NDC")
            hidePave.SetFillColor(ROOT.kGray)
            hidePave.SetFillStyle(1001)
            #hidePave.Draw("Same")
            
            cv.Update()
            cv.Print("/home/mkomm/Analysis/ST13/plots/2015_07_22_DCSBUGRA/"+outputName+".pdf")
            cv.Print("/home/mkomm/Analysis/ST13/plots/2015_07_22_DCSBUGRA/"+outputName+".png")
            cv.Print("/home/mkomm/Analysis/ST13/plots/2015_07_22_DCSBUGRA/"+outputName+".C")
            cv.WaitPrimitive()
            #break
        #break
    #break
   




