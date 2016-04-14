#!/usr/bin/python

import ROOT
import os
import numpy
import math
import csv
import random

ROOT.gROOT.Reset()
ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptFit(0)
ROOT.gROOT.Reset()
ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptFit(1111)
ROOT.gStyle.SetPadTopMargin(0.08)
ROOT.gStyle.SetPadLeftMargin(0.14)
ROOT.gStyle.SetPadRightMargin(0.06)
ROOT.gStyle.SetPadBottomMargin(0.12)
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

ROOT.gStyle.SetHatchesSpacing(0.8)
ROOT.gStyle.SetHatchesLineWidth(2)

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
ROOT.gStyle.SetTitleYOffset(1.2)
#ROOT.gStyle.SetTitleOffset(1.15, "YZ") # Another way to set the Offset

# For the axis labels:

ROOT.gStyle.SetLabelColor(1, "XYZ")
ROOT.gStyle.SetLabelFont(43, "XYZ")
ROOT.gStyle.SetLabelOffset(0.0077, "XYZ")
ROOT.gStyle.SetLabelSize(34, "XYZ")
#ROOT.gStyle.SetLabelSize(0.04, "XYZ")

# For the axis:

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
ROOT.gStyle.SetPaintTextFormat("4.2f")

def readPrefitYield(f):
    #"category","component","process","entries","integral","unc"
    rootFile = ROOT.TFile(f)
    result = {}
    for k in rootFile.GetListOfKeys():
        cat,comp,name=k.GetName().split("__",3)
        if name=="total":
            if not result.has_key(cat):
                result[cat]={}
            result[cat][comp]=rootFile.Get(k.GetName()).Clone(k.GetName()+str(random.random()))
            result[cat][comp].SetDirectory(0)
            if comp.find("QCD")>=0:
                result[cat][comp].Scale(0.2)
    rootFile.Close()
    return result

uncertainties = [
    ["DY","Z+jets yield"],
    ["TW","tW yield"],
    ["Btag","b-tagging efficiency"],
    ["Ltag","mis-tagging efficiency"],
    ["PU","pileup reweighting"],
    ["Iso","multijet isolation region"],
    ["En","jet energy scale"],
    ["Res","jet resolution"],
    ["Muon","muon selection"],
    ["PDF","PDF"],
    #["UnclEn","unclustered energy"],
    ["QScaleTChannel","t-ch. Q scale"], 
    ["QScaleTTbar","t#bar{t} Q scale"],
    ["QScaleTW","tW Q scale"],
    ["QScaleWjets","W+jets Q scale"],
    ["TopMass","top quark mass"]
]

uncertaintiesSpecial = [
    ["NoTopPt","top quark pt rew."],
    ["Powheg","generator modeling"],
    ["Herwig","hadronization modeling"]
]


fitSetup = "fit_pt0"

nominal = readPrefitYield("result/nominal/"+fitSetup+"_fitHists.root")

sysUp={}
sysDown={}
for unc in uncertainties:
    sysUp[unc[0]] = readPrefitYield("result/"+unc[0]+"Up/"+fitSetup+"_fitHists.root")
    sysDown[unc[0]] = readPrefitYield("result/"+unc[0]+"Down/"+fitSetup+"_fitHists.root")

for unc in uncertaintiesSpecial:
    sysUp[unc[0]] = readPrefitYield("result/"+unc[0]+"/"+fitSetup+"_fitHists.root")
    sysDown[unc[0]] = nominal

for cat in ["2j1t","3j1t","3j2t"]:
    for unc in uncertainties+uncertaintiesSpecial:
        totalHist = None
        totalHistUp = None
        totalHistDown = None
        
        stackedNominal = []
        stackedUp = []
        stackedDown = []
        
        legend=ROOT.TLegend(0.74,0.885,0.9,0.66)
        legend.SetBorderSize(0)
        legend.SetTextFont(43)
        legend.SetTextSize(23)
        legend.SetFillColor(ROOT.kWhite)

        for comp in [
            ["tChannel",ROOT.kMagenta],
            ["TopBkg",ROOT.kRed],
            ["WZjets",ROOT.kGreen],
            ["QCD",ROOT.kBlue]
        ]:

            if comp[0]=="QCD":
                comp[0]+="_"+cat

            nominalHist = nominal[cat][comp[0]].Clone(nominal[cat][comp[0]].GetName()+str(random.random()))
            nominalHist.SetLineColor(comp[1]+1)
            nominalHist.SetLineWidth(3)
            
            nominalHistStacked=nominalHist.Clone(nominalHist.GetName()+str(random.random()))
                    
            legend.AddEntry(nominalHistStacked,comp[0],"L")
            if comp[0].find("QCD")>=0:
                legend.AddEntry("","(#times 0.2)","")

            if totalHist!=None:
                nominalHistStacked.Add(totalHist)
            stackedNominal.append(nominalHistStacked)
            

            upHist = sysUp[unc[0]][cat][comp[0]].Clone("upHist"+str(random.random()))
            upHist.SetLineColor(comp[1]-9)
            upHist.SetLineWidth(2)
            upHist.SetLineStyle(1)
            upHist.SetMarkerColor(comp[1]-9)
            upHist.SetMarkerStyle(22)
            upHist.SetMarkerSize(0.9)
            downHist = sysDown[unc[0]][cat][comp[0]].Clone("upHist"+str(random.random()))
            downHist.SetLineColor(comp[1]-9)
            downHist.SetLineWidth(2)
            downHist.SetLineStyle(1)
            downHist.SetMarkerColor(comp[1]-9)
            downHist.SetMarkerStyle(23)
            downHist.SetMarkerSize(0.9)
            
            upHistStacked=upHist.Clone(upHist.GetName()+str(random.random()))
            downHistStacked=downHist.Clone(downHist.GetName()+str(random.random()))
            if totalHistUp!=None:
                upHistStacked.Add(totalHist)
                downHistStacked.Add(totalHist)
            stackedUp.append(upHistStacked)
            stackedDown.append(downHistStacked)
            
            
            if totalHist==None:
                totalHist=nominalHist.Clone("total"+str(random.random()))
                totalHist.SetLineColor(ROOT.kGray+1)
            else:
                totalHist.Add(nominalHist)
            
            if totalHistUp==None:
                totalHistUp=upHist.Clone("totalUp"+str(random.random()))
                totalHistUp.SetLineColor(ROOT.kGray+1)
                totalHistUp.SetLineStyle(3)
                totalHistUp.SetLineWidth(3)
                totalHistUp.SetMarkerColor(ROOT.kGray+1)
                totalHistUp.SetMarkerStyle(22)
                totalHistUp.SetMarkerSize(0.9)
                
                totalHistDown=downHist.Clone("totalDown"+str(random.random()))
                totalHistDown.SetLineColor(ROOT.kGray+1)
                totalHistDown.SetLineStyle(3)
                totalHistDown.SetLineWidth(3)
                totalHistDown.SetMarkerColor(ROOT.kGray+1)
                totalHistDown.SetMarkerStyle(23)
                totalHistDown.SetMarkerSize(0.9)
            else:
                totalHistUp.Add(upHist)
                totalHistDown.Add(downHist)
            
            
        cv = ROOT.TCanvas("cv"+str(random.random()),"",800,750)
        ymax = nominal[cat]["data"].GetMaximum()
        axis = ROOT.TH2F("axis"+str(random.random()),";Fit variable;MC events",50,totalHist.GetXaxis().GetXmin(),totalHist.GetXaxis().GetXmax(),50,0,1.5*ymax)
        axis.Draw("AXIS")
        
        

        
        for upHist in stackedUp:
            upHist.Draw("SameHist")
            upHist.Draw("SameHistP")
            
        for downHist in stackedDown:
            downHist.Draw("SameHist")
            downHist.Draw("SameHistP")
            
        for nominalHist in stackedNominal:
            nominalHist.Draw("SameHist")

        legend.AddEntry(totalHistUp,"total","L")
        
        totalHistUp.Draw("SameHist")
        totalHistDown.Draw("SameHist")
        totalHistUp.Draw("SameHistP")
        totalHistDown.Draw("SameHistP")
        
        nominal[cat]["data"].SetMarkerStyle(20)
        nominal[cat]["data"].SetMarkerSize(0.9)
        nominal[cat]["data"].Draw("SamePE")

        legend.AddEntry(nominal[cat]["data"],"data","P")

        #totalHist.Draw("SameHist")
        
        pCMS=ROOT.TPaveText(cv.GetLeftMargin()+0.1,0.94,cv.GetLeftMargin()+0.1,0.94,"NDC")
        pCMS.SetFillColor(ROOT.kWhite)
        pCMS.SetBorderSize(0)
        pCMS.SetTextFont(63)
        pCMS.SetTextSize(35)
        pCMS.SetTextAlign(11)
        pCMS.AddText("CMS")
        pCMS.Draw("Same")

        pPreliminary=ROOT.TPaveText(cv.GetLeftMargin()+0.1+0.095,0.94,cv.GetLeftMargin()+0.1+0.095,0.94,"NDC")
        pPreliminary.SetFillColor(ROOT.kWhite)
        pPreliminary.SetBorderSize(0)
        pPreliminary.SetTextFont(53)
        pPreliminary.SetTextSize(35)
        pPreliminary.SetTextAlign(11)
        pPreliminary.AddText("Preliminary")
        pPreliminary.Draw("Same")
        
        pCut=ROOT.TPaveText(cv.GetLeftMargin()+0.03,0.885,cv.GetLeftMargin()+0.03,0.885,"NDC")
        pCut.SetFillColor(ROOT.kWhite)
        pCut.SetBorderSize(0)
        pCut.SetTextFont(43)
        pCut.SetTextSize(29)
        pCut.SetTextAlign(13)
        pCut.AddText(cat+", "+unc[1])
        pCut.Draw("Same")
        


        legend.Draw("Same")
            
        cv.Update()
        cv.Print(cat+"_prefitShape_"+unc[0]+".pdf")
        cv.Print(cat+"_prefitShape_"+unc[0]+".png")
            
 
