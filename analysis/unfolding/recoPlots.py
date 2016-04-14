#!/usr/bin/python

import ROOT
import numpy
import random
import math
import os
import re
from optparse import OptionParser

ROOT.gROOT.Reset()
ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptFit(0)
ROOT.gROOT.Reset()
ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptFit(1111)
'''
ROOT.gStyle.SetPadTopMargin(0.08)
ROOT.gStyle.SetPadLeftMargin(0.13)
ROOT.gStyle.SetPadRightMargin(0.225)
ROOT.gStyle.SetPadBottomMargin(0.15)
'''
ROOT.gStyle.SetPadTopMargin(0.08)
ROOT.gStyle.SetPadLeftMargin(0.15)
ROOT.gStyle.SetPadRightMargin(0.04)
ROOT.gStyle.SetPadBottomMargin(0.12)


ROOT.gStyle.SetMarkerSize(0.16)
ROOT.gStyle.SetHistLineWidth(1)
ROOT.gStyle.SetStatFontSize(0.025)



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

ROOT.gStyle.SetHatchesSpacing(0.6)
ROOT.gStyle.SetHatchesLineWidth(1)

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
ROOT.gStyle.SetTitleSize(40, "XYZ")
# ROOT.gStyle.SetTitleXSize(Float_t size = 0.02) # Another way to set the size?
# ROOT.gStyle.SetTitleYSize(Float_t size = 0.02)
ROOT.gStyle.SetTitleXOffset(0.99)
ROOT.gStyle.SetTitleYOffset(1.45)

# For the axis labels:

ROOT.gStyle.SetLabelColor(1, "XYZ")
ROOT.gStyle.SetLabelFont(43, "XYZ")
ROOT.gStyle.SetLabelOffset(0.008, "XYZ")
ROOT.gStyle.SetLabelSize(34, "XYZ")
#ROOT.gStyle.SetLabelSize(0.04, "XYZ")

# For the axis:

ROOT.gStyle.SetAxisColor(1, "XYZ")
ROOT.gStyle.SetAxisColor(1, "XYZ")
ROOT.gStyle.SetAxisColor(1, "XYZ")
ROOT.gStyle.SetAxisColor(1, "XYZ")
ROOT.gStyle.SetStripDecimals(True)
ROOT.gStyle.SetTickLength(0.025, "Y")
ROOT.gStyle.SetTickLength(0.025, "X")
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

# Postscript options:
ROOT.gStyle.SetPaperSize(8.0*1.3,7.5*1.3)
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

sets = {
    "tChannel": {
        "hists": ["tChannel__tChannel"],
        "color":newColor(0.83,0.05,0.0),
        "title":"#it{t}-channel",
    },
    "TopBkg": {
        "hists":["TopBkg__tWChannel","TopBkg__TTJets"],
        "color":ROOT.gROOT.GetColor(ROOT.kOrange-3),
        "title":"tt#lower[-0.87]{#kern[-0.89]{-}}/tW",
    },
    "WZjets": {
        "hists": ["WZjets__WJetsAMC","WZjets__DY"],
        "color":newColor(0.2,0.8,0.2),
        "title":"W/Z+jets",
    },
    "QCD": {
        "hists": ["QCD_2j1t__data76_antiiso","QCD_2j1t__MC_antiiso"],
        "color":ROOT.gROOT.GetColor(ROOT.kGray),
        "title":"Multijet",
    },
    "data": {
        "hists": ["data__data"],
        "color":ROOT.gROOT.GetColor(ROOT.kBlack),
        "title":"Data",
    },
}


parser = OptionParser()
(options, args) = parser.parse_args()

rootFiles=[]

uncertainties = [
    ["DY","\\zjets yield"],
    ["TW","\\tw yield"],
    ["Btag","b-tagging efficiency"],
    ["Ltag","mis-tagging efficiency"],
    ["PU","pileup reweighting"],
    ["Iso","multijet isolation region"],
    ["En","jet energy scale"],
    ["Res","jet resolution"],
    ["Muon","muon selection"],
    ["PDF","PDF"],
    #["UnclEn","unclustered energy"],
    ["QScaleTChannel","\\tch Q scale"], 
    ["QScaleTTbar","\\ttbar Q scale"],
    ["QScaleTW","\\tw Q scale"],
    ["QScaleWjets","\\wjets Q scale"],
    ["TopMass","top quark mass"]
]

uncertaintiesSpecial = [
    ["NoTopPt","top quark \\pt reweighting"],
    ["Powheg","signal modeling"],
    ["Herwig","hadronization modeling"]
]


    

def addUnderflowOverflow(hist):
    hist.SetBinContent(1,hist.GetBinContent(0)+hist.GetBinContent(1))
    N=hist.GetNbinsX()
    hist.SetBinContent(N,hist.GetBinContent(N)+hist.GetBinContent(N+1))
    hist.SetBinContent(0,0)
    hist.SetBinContent(N+1,0)
    hist.SetEntries(hist.GetEntries()-4)
    
def normalize(hist):
    for ibin in range(hist.GetNbinsX()):
        hist.SetBinError(
            ibin+1,
            hist.GetBinError(ibin+1)/hist.GetBinWidth(ibin+1)
        )
        hist.SetBinContent(
            ibin+1,
            hist.GetBinContent(ibin+1)/hist.GetBinWidth(ibin+1)
        )

        

for setName in sets.keys():
    sets[setName]["darkColor"]=getDarkerColor(sets[setName]["color"])

plotSetups = [
    {
        "category": "2#kern[-0.5]{ }jets 1#kern[-0.5]{ }b-tag",
        "cut": "",
        "unit": "",
        "resUp": 1.4,
        "resDown": 0.6,
        "normalizeByBin": False,
        "scale": 0,
        "variableTitle": "|#eta(j#it{'})|",
        "inputFileName": "reco_etaj"
    },
    {
        "category": "2#kern[-0.5]{ }jets 1#kern[-0.5]{ }b-tag",
        "cut": "",
        "unit": "GeV",
        "resUp": 1.4,
        "resDown": 0.6,
        "normalizeByBin": False,
        "scale": 0,
        "variableTitle": "#it{m}#lower[0.4]{#scale[0.7]{#mu#nub}}",
        "inputFileName": "reco_topmass"
    },
    {
        "category": "2#kern[-0.5]{ }jets 1#kern[-0.5]{ }b-tag",
        "cut": "",
        "unit": "",
        "resUp": 1.4,
        "resDown": 0.6,
        "normalizeByBin": False,
        "scale": 0,
        "variableTitle": "#it{m}#lower[0.4]{#scale[0.7]{T}}(W)",
        "inputFileName": "reco_mtw"
    },

    {
        "category": "2#kern[-0.5]{ }jets 1#kern[-0.5]{ }b-tag",
        "cut": "#it{m}#lower[0.4]{#scale[0.7]{T}}(W)#kern[-0.3]{ }>#kern[-0.3]{ }50#kern[-0.5]{ }GeV",
        "unit": "",
        "resUp": 1.4,
        "resDown": 0.6,
        "normalizeByBin": False,
        "scale": 0,
        "variableTitle": "BDT discriminant",
        "inputFileName": "reco_BDT"
    },

    {
        "category": "2#kern[-0.5]{ }jets 1#kern[-0.5]{ }b-tag",
        "cut": "#it{m}#lower[0.4]{#scale[0.7]{T}}(W)#kern[-0.3]{ }>#kern[-0.3]{ }50#kern[-0.5]{ }GeV & BDT#kern[-0.7]{ }<#kern[-0.3]{ }0",
        "unit": "GeV",
        "resUp": 1.4,
        "resDown": 0.6,
        "normalizeByBin": True,
        "scale": 0,
        "variableTitle": "top quark p#lower[0.4]{#scale[0.7]{T}}",
        "inputFileName": "reco_toppt_bdtinv"
    },
    {
        "category": "2#kern[-0.5]{ }jets 1#kern[-0.5]{ }b-tag",
        "cut": "#it{m}#lower[0.4]{#scale[0.7]{T}}(W)#kern[-0.3]{ }>#kern[-0.3]{ }50#kern[-0.5]{ }GeV & BDT#kern[-0.7]{ }>#kern[-0.3]{ }0.6",
        "unit": "GeV",
        "resUp": 1.4,
        "resDown": 0.6,
        "normalizeByBin": True,
        "scale": 0,
        "variableTitle": "top quark |y|",
        "inputFileName": "reco_toppt_bdt"
    },    

    {
        "category": "2#kern[-0.5]{ }jets 1#kern[-0.5]{ }b-tag",
        "cut": "#it{m}#lower[0.4]{#scale[0.7]{T}}(W)#kern[-0.3]{ }>#kern[-0.3]{ }50#kern[-0.5]{ }GeV & BDT#kern[-0.7]{ }<#kern[-0.3]{ }0",
        "unit": "",
        "resUp": 1.4,
        "resDown": 0.6,
        "normalizeByBin": True,
        "scale": 3,
        "variableTitle": "top quark |y|",
        "inputFileName": "reco_topy_bdtinv"
    },
    {
        "category": "2#kern[-0.5]{ }jets 1#kern[-0.5]{ }b-tag",
        "cut": "#it{m}#lower[0.4]{#scale[0.7]{T}}(W)#kern[-0.3]{ }>#kern[-0.3]{ }50#kern[-0.5]{ }GeV & BDT#kern[-0.7]{ }>#kern[-0.3]{ }0.6",
        "unit": "",
        "resUp": 1.4,
        "resDown": 0.6,
        "normalizeByBin": True,
        "scale": 3,
        "variableTitle": "top quark |y|",
        "inputFileName": "reco_topy_bdt"
    }
    
    
]

for plotSetup in plotSetups:
    category=plotSetup["category"]
    cut=plotSetup["cut"]
    unit=plotSetup["unit"]
    resUp=plotSetup["resUp"]
    resDown=plotSetup["resDown"]
    normalizeByBin = plotSetup["normalizeByBin"]
    scale = plotSetup["scale"]
    variableTitle= plotSetup["variableTitle"]
    inputFileName= plotSetup["inputFileName"]
    outputFileName=inputFileName       
       
       
    stackMC = ROOT.THStack()
    stackMCHists=[]

    legendEntries=[]

    totalMC = 0.0
    totalMCError2 = 0.0

    sysHistograms = {}
    for unc in uncertainties+uncertaintiesSpecial:
        sysHistograms[unc[0]]={"Up":None,"Down":None}

    sumHistMC = None
    for setName in (["QCD","WZjets","TopBkg","tChannel"]):
        setHist = None
        for histName in sets[setName]["hists"]:
            rootFile = ROOT.TFile("result/nominal/"+inputFileName+".root")
            hNominal = rootFile.Get(histName).Clone(histName+str(random.random()))
            hNominal.SetDirectory(0)
            if normalizeByBin:
                normalize(hNominal)
            if scale!=0:
                hNominal.Scale(10**-scale)
            rootFile.Close()
            if setHist==None:
                setHist = hNominal.Clone()
                setHist.SetFillColor(sets[setName]["color"].GetNumber())
                setHist.SetFillStyle(1001)
                setHist.SetLineStyle(1)
                setHist.SetLineWidth(2)
                setHist.SetLineColor(sets[setName]["darkColor"].GetNumber())
                setHist.SetDirectory(0)
            else:
                setHist.Add(hNominal)
            
            for uncertainty in uncertainties:
                for shift in ["Up","Down"]:
                    rootFile = ROOT.TFile("result/"+uncertainty[0]+shift+"/"+inputFileName+".root")
                    hSys = rootFile.Get(histName).Clone(histName+str(random.random()))
                    hSys.SetDirectory(0)
                    if normalizeByBin:
                        normalize(hSys)
                    if scale!=0:
                        hSys.Scale(10**-scale)
                    rootFile.Close()
                    if sysHistograms[uncertainty[0]][shift]==None:
                        sysHistograms[uncertainty[0]][shift]=hSys.Clone()
                    else:
                        sysHistograms[uncertainty[0]][shift].Add(hSys)
                        
            for uncertainty in uncertaintiesSpecial:
                rootFile = ROOT.TFile("result/"+uncertainty[0]+"/"+inputFileName+".root")
                hSys = rootFile.Get(histName).Clone(histName+str(random.random()))
                hSys.SetDirectory(0)
                if normalizeByBin:
                    normalize(hSys)
                if scale!=0:
                    hSys.Scale(10**-scale)
                rootFile.Close()
                if sysHistograms[uncertainty[0]]["Up"]==None:
                    sysHistograms[uncertainty[0]]["Up"]=hSys.Clone(histName+str(random.random()))
                else:
                    sysHistograms[uncertainty[0]]["Up"].Add(hSys)
                if sysHistograms[uncertainty[0]]["Down"]==None:
                    sysHistograms[uncertainty[0]]["Down"]=hNominal.Clone(histName+str(random.random()))
                else:
                    sysHistograms[uncertainty[0]]["Down"].Add(hNominal)
                
        if sumHistMC==None:
            sumHistMC=setHist.Clone()
        else:
            sumHistMC.Add(setHist)
        totalMC+=setHist.Integral()
        error = 0.0
        if setHist.GetEntries()>0:
            error = setHist.Integral()/setHist.GetEntries()*math.sqrt(setHist.GetEntries())
        totalMCError2+=error**2
        print "\t",setName,round(setHist.GetEntries(),1),round(setHist.Integral(),1),"+-",round(error,1)
        stackMC.Add(setHist,"HIST F")
        stackMCHists.append(setHist)
        if sets[setName].has_key("addtitle"):
            legendEntries.append(["",sets[setName]["addtitle"],""])
        legendEntries.append([setHist,sets[setName]["title"],"F"])
        
    print "\ttotal MC",round(sumHistMC.GetEntries(),1),round(totalMC,1),"+-",round(math.sqrt(totalMCError2),1)
     
    sumHistData = None
        
    for setName in ["data"]:
        setHist = None
        
        for histName in sets[setName]["hists"]:
            rootFile = ROOT.TFile("result/nominal/"+inputFileName+".root")
            h = rootFile.Get(histName).Clone(histName+str(random.random()))
            h.SetDirectory(0)
            if normalizeByBin:
                normalize(h)
            if scale!=0:
                h.Scale(10**-scale)
            rootFile.Close()
            if setHist==None:
                setHist = h.Clone()
                setHist.SetMarkerColor(sets[setName]["color"].GetNumber())
                setHist.SetMarkerStyle(20)
                setHist.SetMarkerSize(1.4)
                setHist.SetFillStyle(0)
                setHist.SetLineStyle(1)
                setHist.SetLineWidth(1)
                setHist.SetDirectory(0)
            else:
                setHist.Add(h)
                    
        if sumHistData==None:
            sumHistData=setHist.Clone()
        else:
            sumHistData.Add(setHist)
        print "\t",setName,round(setHist.GetEntries(),1),round(setHist.Integral(),1),"+-",round(setHist.Integral()/setHist.GetEntries()*math.sqrt(setHist.GetEntries()),1)
        
    legendEntries.append([sumHistData,"Data","P"])
        

    print "\tMC=",sumHistMC.Integral(),", data=",sumHistData.Integral(),", L=",sumHistData.Integral()/sumHistMC.Integral()



    NTOYS=2000
    uncertaintyToys = numpy.zeros((sumHistMC.GetNbinsX(),NTOYS))
    uncertaintyBand = numpy.zeros((sumHistMC.GetNbinsX(),3))
    uncertaintyBandHistUp = sumHistMC.Clone()
    uncertaintyBandHistUp.SetLineColor(ROOT.kGray+1)
    uncertaintyBandHistUp.SetLineStyle(2)
    uncertaintyBandHistUp.SetFillStyle(0)
    uncertaintyBandHistUp.SetLineWidth(2)

    uncertaintyBandHistDown = sumHistMC.Clone()
    uncertaintyBandHistDown.SetLineColor(ROOT.kGray+1)
    uncertaintyBandHistDown.SetLineStyle(2)
    uncertaintyBandHistDown.SetFillStyle(0)
    uncertaintyBandHistDown.SetLineWidth(2)

    for ibin in range(sumHistMC.GetNbinsX()):
        for itoy in range(NTOYS):
            nominal = sumHistMC.GetBinContent(ibin+1)
            uncertaintyToys[ibin][itoy]=numpy.random.normal()*sumHistMC.GetBinError(ibin+1)
            for unc in uncertainties+uncertaintiesSpecial:
                up = sysHistograms[unc[0]]["Up"].GetBinContent(ibin+1)-nominal
                down = sysHistograms[unc[0]]["Down"].GetBinContent(ibin+1)-nominal
                dice = numpy.random.normal()
                if dice<0:
                    uncertaintyToys[ibin][itoy]+=down*math.fabs(dice)
                else:
                    uncertaintyToys[ibin][itoy]+=up*math.fabs(dice)

        uncertaintyBand[ibin] = numpy.percentile(uncertaintyToys[ibin],[15.865,50.0,84.135])
        uncertaintyBandHistDown.SetBinContent(ibin+1,uncertaintyBand[ibin][0]-uncertaintyBand[ibin][1]+nominal)
        uncertaintyBandHistUp.SetBinContent(ibin+1,uncertaintyBand[ibin][2]-uncertaintyBand[ibin][1]+nominal)



               
    cvxmin=ROOT.gStyle.GetPadLeftMargin()
    cvxmax=1-ROOT.gStyle.GetPadRightMargin()
    cvymin=ROOT.gStyle.GetPadBottomMargin()
    cvymax=1-ROOT.gStyle.GetPadTopMargin()
    resHeight=0.35


    cv = ROOT.TCanvas("cv"+str(random.random()),"",800,750)
    cv.Divide(1,2,0,0)
    cv.GetPad(1).SetPad(0.0, 0.0, 1.0, 1.0)
    cv.GetPad(1).SetFillStyle(4000)
    cv.GetPad(2).SetPad(0.0, 0.00, 1.0,1.0)
    cv.GetPad(2).SetFillStyle(4000)



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


    axis=ROOT.TH2F(
        "axis"+str(random.random()),
        ";"+variableTitle+";",
        50,
        sumHistData.GetXaxis().GetXmin(),
        sumHistData.GetXaxis().GetXmax(),
        50,
        0.0,
        1.4*max([sumHistData.GetMaximum(),sumHistMC.GetMaximum(),uncertaintyBandHistUp.GetMaximum()])
    )

    axis.GetYaxis().SetNdivisions(506)
    axis.GetXaxis().SetNdivisions(504)
    axis.GetXaxis().SetLabelSize(0)
    axis.GetXaxis().SetTitle("")
    axis.GetXaxis().SetTickLength(0.015/(1-cv.GetPad(2).GetLeftMargin()-cv.GetPad(2).GetRightMargin()))
    axis.GetYaxis().SetTickLength(0.015/(1-cv.GetPad(2).GetTopMargin()-cv.GetPad(2).GetBottomMargin()))
    axis.GetYaxis().SetNoExponent(True)
    axis.Draw("AXIS")

    stackMC.Draw("Same")
    '''
    sysHistograms["TopMass"]["Up"].SetFillStyle(0)
    sysHistograms["TopMass"]["Up"].Draw("SAMEHIST")
    sysHistograms["TopMass"]["Down"].SetFillStyle(0)
    sysHistograms["TopMass"]["Down"].Draw("SAMEHIST")
    '''
                    
    #uncertaintyBandHistDown.Draw("SameHIST")
    #uncertaintyBandHistUp.Draw("SameHIST")

    scaleText = ""
    if scale!=0:
        scaleText="10#scale[0.8]{#lower[-0.4]{"+str(scale)+"}}#kern[-0.5]{ }"

    if normalizeByBin:
        if unit=="":
            axis.GetYaxis().SetTitle("#lower[-0.1]{#LT}#kern[-0.5]{ }Events / "+scaleText+"units#kern[-0.5]{ }#lower[-0.1]{#GT}")
        else:
            axis.GetYaxis().SetTitle("#lower[-0.1]{#LT}#kern[-0.5]{ }Events / "+scaleText+unit+"#kern[-0.5]{ }#lower[-0.1]{#GT}")
    else:
        if unit=="":
            axis.GetYaxis().SetTitle("Events / "+scaleText+"bin")
        else:
            axis.GetYaxis().SetTitle("Events / "+scaleText+"bin")




    ROOT.gPad.RedrawAxis()

    #legend = ROOT.TLegend(cvxmax+0.01,cvymax,0.99,cvymax-0.075*len(legendEntries))
    legend = ROOT.TLegend(cvxmax-0.24,cvymax-0.02,cvxmax-0.05,cvymax-0.02-0.067*len(legendEntries))

    #legend = ROOT.TLegend(0.73,0.9,0.95,0.745-0.052*len(legendEntries))

    #legend = ROOT.TLegend(0.7,0.72,0.95,0.72-0.07*len(legendEntries))

    legend.SetFillColor(ROOT.kWhite)
    legend.SetBorderSize(0)
    legend.SetTextFont(43)
    legend.SetTextSize(30)
    for entry in reversed(legendEntries):
        legend.AddEntry(entry[0],entry[1],entry[2])
    '''
    pText=ROOT.TPaveText(cvxmin+0.1,0.86,cvxmin+0.1,0.86,"NDC")
    pText.SetFillColor(ROOT.kWhite)
    pText.SetBorderSize(0)
    pText.SetTextFont(63)
    pText.SetTextSize(24)
    pText.SetTextAlign(11)
    pText.AddText(qcd[2])
    pText.Draw("Same")
    '''
    pCMS=ROOT.TPaveText(cvxmin,0.94,cvxmin,0.94,"NDC")
    pCMS.SetFillColor(ROOT.kWhite)
    pCMS.SetBorderSize(0)
    pCMS.SetTextFont(63)
    pCMS.SetTextSize(35)
    pCMS.SetTextAlign(11)
    pCMS.AddText("CMS")
    pCMS.Draw("Same")

    pPreliminary=ROOT.TPaveText(cvxmin+0.095,0.94,cvxmin+0.095,0.94,"NDC")
    pPreliminary.SetFillColor(ROOT.kWhite)
    pPreliminary.SetBorderSize(0)
    pPreliminary.SetTextFont(53)
    pPreliminary.SetTextSize(35)
    pPreliminary.SetTextAlign(11)
    pPreliminary.AddText("Preliminary")
    pPreliminary.Draw("Same")


    pCat=ROOT.TPaveText(cvxmin+0.03,cvymax-0.03,cvxmin+0.03,cvymax-0.03,"NDC")
    pCat.SetFillColor(ROOT.kWhite)
    pCat.SetBorderSize(0)
    pCat.SetTextFont(63)
    pCat.SetTextSize(31)
    pCat.SetTextAlign(13)
    pCat.AddText(category)
    pCat.Draw("Same")
    '''
    pAdd=ROOT.TPaveText(cvxmin+0.11,0.875,cvxmin+0.11,0.875,"NDC")
    pAdd.SetFillColor(ROOT.kWhite)
    pAdd.SetBorderSize(0)
    pAdd.SetTextFont(43)
    pAdd.SetTextSize(22)
    pAdd.SetTextAlign(11)
    pAdd.AddText("(scaled to incl. fit result)")
    pAdd.Draw("Same")
    '''
    pCut=ROOT.TPaveText(cvxmin+0.03,cvymax-0.085,cvxmin+0.03,cvymax-0.085,"NDC")
    pCut.SetFillColor(ROOT.kWhite)
    pCut.SetBorderSize(0)
    pCut.SetTextFont(43)
    pCut.SetTextSize(31)
    pCut.SetTextAlign(13)
    pCut.AddText(cut)
    pCut.Draw("Same")
    '''
    pChi=ROOT.TPaveText(cvxmax-0.03,0.875,cvxmax-0.03,0.875,"NDC")
    pChi.SetFillColor(ROOT.kWhite)
    pChi.SetBorderSize(0)
    pChi.SetTextFont(43)
    pChi.SetTextSize(24)
    pChi.SetTextAlign(31)
    pChi.AddText("#lower[-0.18]{#chi^{2}}: %5.1f%%" % (100.0*sumHistData.Chi2Test(sumHistMC,"WW")))
    pChi.Draw("Same")

    pKS=ROOT.TPaveText(cvxmax-0.03,0.82,cvxmax-0.03,0.82,"NDC")
    pKS.SetFillColor(ROOT.kWhite)
    pKS.SetBorderSize(0)
    pKS.SetTextFont(43)
    pKS.SetTextSize(24)
    pKS.SetTextAlign(31)
    pKS.AddText("KS: %5.1f%%" % (100.0*sumHistData.KolmogorovTest(sumHistMC)))
    pKS.Draw("Same")
    '''
    pLumi=ROOT.TPaveText(cvxmax,0.94,cvxmax,0.94,"NDC")
    pLumi.SetFillColor(ROOT.kWhite)
    pLumi.SetBorderSize(0)
    pLumi.SetTextFont(43)
    pLumi.SetTextSize(35)
    pLumi.SetTextAlign(31)
    pLumi.AddText("2.3#kern[-0.5]{ }fb#lower[-0.7]{#scale[0.7]{-1}} (13#kern[-0.5]{ }TeV)")
    pLumi.Draw("Same")




    cv.cd(1)
    axisRes=None
    if unit!="":
        axisRes=ROOT.TH2F("axisRes"+str(random.random()),";"+variableTitle+" ("+unit+");Data/MC",50,sumHistData.GetXaxis().GetXmin(),sumHistData.GetXaxis().GetXmax(),50,resDown,resUp)
    else:
        axisRes=ROOT.TH2F("axisRes"+str(random.random()),";"+variableTitle+";Data/MC",50,sumHistData.GetXaxis().GetXmin(),sumHistData.GetXaxis().GetXmax(),50,resDown,resUp)
    axisRes.GetYaxis().SetNdivisions(406)
    axisRes.GetXaxis().SetNdivisions(504)
    axisRes.GetXaxis().SetTickLength(0.025/(1-cv.GetPad(1).GetLeftMargin()-cv.GetPad(1).GetRightMargin()))
    axisRes.GetYaxis().SetTickLength(0.015/(1-cv.GetPad(1).GetTopMargin()-cv.GetPad(1).GetBottomMargin()))

    axisRes.Draw("AXIS")

    rootObj=[]
    sumHistRes=sumHistData.Clone()
    boxGray = newColor(0.65,0.65,0.65).GetNumber()
    for ibin in range(sumHistData.GetNbinsX()):
        c = sumHistMC.GetBinCenter(ibin+1)
        w = sumHistMC.GetBinWidth(ibin+1)
        m = sumHistMC.GetBinContent(ibin+1)
        d = sumHistData.GetBinContent(ibin+1)
        e = sumHistData.GetBinError(ibin+1)
        
        cv.cd(2)
        hDown = max(uncertaintyBand[ibin][0]-uncertaintyBand[ibin][1]+sumHistMC.GetBinContent(ibin+1),0)
        hUp = uncertaintyBand[ibin][2]-uncertaintyBand[ibin][1]+sumHistMC.GetBinContent(ibin+1)


        
        box = ROOT.TBox(c-0.5*w,hDown,c+0.5*w,hUp)
        box.SetFillStyle(3345)
        box.SetLineColor(boxGray)
        box.SetFillColor(boxGray)
        box.SetLineWidth(2)
        rootObj.append(box)
        box.Draw("SameF")
        
        box2 = ROOT.TBox(c-0.5*w,hDown,c+0.5*w,hUp)
        box2.SetFillStyle(3345)
        box2.SetLineColor(boxGray)
        box2.SetFillColor(ROOT.kGray)
        box2.SetLineWidth(2)
        rootObj.append(box2)
        #box2.Draw("SameL")
        
        cv.cd(1)
        if m>0.0:
            sumHistRes.SetBinContent(ibin+1,d/m)
            sumHistRes.SetBinError(ibin+1,e/m)
        else:
            sumHistRes.SetBinContent(ibin+1,0.0)
            sumHistRes.SetBinError(ibin+1,0)
            
        if d>0:
        
            hDown = max((uncertaintyBand[ibin][0]-uncertaintyBand[ibin][1])/sumHistMC.GetBinContent(ibin+1)+1,resDown)
            hUp = min((uncertaintyBand[ibin][2]-uncertaintyBand[ibin][1])/sumHistMC.GetBinContent(ibin+1)+1,resUp)
            #hDown = max((sumHistMC.GetBinContent(ibin+1)-sumHistMC.GetBinError(ibin+1))/sumHistMC.GetBinContent(ibin+1),resDown)
            #hUp = min((sumHistMC.GetBinContent(ibin+1)+sumHistMC.GetBinError(ibin+1))/sumHistMC.GetBinContent(ibin+1),resUp)
            boxRes = ROOT.TBox(c-0.5*w,hDown,c+0.5*w,hUp)
            boxRes.SetFillStyle(3445)
            boxRes.SetLineColor(boxGray)
            boxRes.SetFillColor(ROOT.kGray)
            boxRes.SetLineWidth(2)
            rootObj.append(boxRes)
            boxRes.Draw("SameF")
            boxRes2 = ROOT.TBox(c-0.5*w,hDown,c+0.5*w,hUp)
            boxRes2.SetFillStyle(0)
            boxRes2.SetLineColor(boxGray)
            boxRes2.SetFillColor(ROOT.kGray)
            boxRes2.SetLineWidth(2)
            rootObj.append(boxRes2)
            boxRes2.Draw("SameL")

    cv.cd(2)
    sumHistData.Draw("PESame")
    cv.cd(1)
    sumHistRes.Draw("PESame")


    if len(rootObj)>0:
        legend.AddEntry(rootObj[1],"Total syst.","F")


    axisLine = ROOT.TF1("axisLine"+str(random.random()),"1",sumHistData.GetXaxis().GetXmin(),sumHistData.GetXaxis().GetXmax())
    axisLine.SetLineColor(ROOT.kBlack)
    axisLine.SetLineWidth(1)
    axisLine.Draw("SameL")
    ROOT.gPad.RedrawAxis()


    hidePave=ROOT.TPaveText(cvxmin-0.06,resHeight-0.028,cvxmin-0.005,resHeight+0.028,"NDC")
    hidePave.SetFillColor(ROOT.kWhite)
    hidePave.SetFillStyle(1001)
    hidePave.Draw("Same")

    cv.cd(2)
    legend.Draw("Same")

    cv.Update()


    cv.Print(outputFileName+".pdf")
    cv.Print(outputFileName+".png")
    cv.WaitPrimitive()






