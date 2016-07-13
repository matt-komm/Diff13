import ROOT
import pyTool
import numpy
import math
import os
import os.path
import re
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
ROOT.gStyle.SetPadLeftMargin(0.15)
ROOT.gStyle.SetPadRightMargin(0.04)
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
ROOT.gStyle.SetLabelOffset(0.008, "XYZ")
ROOT.gStyle.SetLabelSize(34, "XYZ")
#ROOT.gStyle.SetLabelSize(0.04, "XYZ")

# For the axis:

ROOT.gStyle.SetAxisColor(1, "XYZ")
ROOT.gStyle.SetAxisColor(1, "XYZ")
ROOT.gStyle.SetStripDecimals(True)
ROOT.gStyle.SetTickLength(0.025, "Y")
ROOT.gStyle.SetTickLength(0.025, "X")
ROOT.gStyle.SetNdivisions(1005, "X")
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
ROOT.gStyle.SetPaintTextFormat("4.2f")


inputFolder = "result"
normalize =True
'''
outputName="unfolded_top_pt"
rootFileName = "unfoldingPt"
histName = "unfoldedHistPt"
responseName = "responsePt"


'''
outputName="unfolded_top_y"
rootFileName = "unfoldingY"
histName = "unfoldedHistY"
responseName = "responseY"



fNominal = ROOT.TFile(inputFolder+"/nominal/"+rootFileName+".root")
nominalHist=fNominal.Get(histName).Clone()
nominalHist.SetDirectory(0)
nominalHist.SetMarkerColor(ROOT.kBlack)
nominalHist.SetMarkerStyle(20)
nominalHist.SetMarkerSize(1.4)
dataNorm = nominalHist.Integral()


genHist = fNominal.Get(responseName).Clone().ProjectionX("gen")
genHist.SetDirectory(0)
genHist.SetLineColor(ROOT.kAzure-5)
genHist.SetLineWidth(5)
N = nominalHist.GetNbinsX()


def normalizeByBinWidth(hist,isSys=False):
    if normalize:
        norm = hist.Integral()
        if isSys:
            norm=dataNorm
        
        if rootFileName=="unfoldingPt":
            hist.Scale(1000.)
        for ibin in range(hist.GetNbinsX()):
            hist.SetBinError(ibin+1,hist.GetBinError(ibin+1)/hist.GetBinWidth(ibin+1)/norm)
            hist.SetBinContent(ibin+1,hist.GetBinContent(ibin+1)/hist.GetBinWidth(ibin+1)/norm)
            
        testSum = 0.0
        for ibin in range(hist.GetNbinsX()):
            testSum+=hist.GetBinContent(ibin+1)*hist.GetBinWidth(ibin+1)
        #print testSum
        
normalizeByBinWidth(nominalHist)
normalizeByBinWidth(genHist)

sysHistograms = []

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

print "%36s & %11s & %11s & %11s & %11s" % ("systematic source","bin 1 / \\$10^{-2}$", "bin 2 / \\$10^{-2}$","bin 3 / \\$10^{-2}$", "bin 4 / \\$10^{-2}$")

print "%36s " % ("nominal"),
for ibin in range(4):
    print "& %11.1f " % (100.0*(nominalHist.GetBinContent(ibin+1))),
print "\\\\"
print "\\hline"
print "%36s " % ("statistical"),
for ibin in range(4):
    print "& $\\pm$%6.1f " % (100.0*(nominalHist.GetBinError(ibin+1))), 
print "\\\\"
print "\\hline"

averageShifts={}

for unc in uncertainties:
    sysDict = []
    averageShifts[unc[0]]=[]
    
    shiftMin = 1000.0
    shiftMax = -1000.0
    for shift in ["Up","Down"]:
        f = ROOT.TFile(inputFolder+"/"+unc[0]+shift+"/"+rootFileName+".root")
        hist = f.Get(histName).Clone()
        hist.SetDirectory(0)
        normalizeByBinWidth(hist,isSys=True)
        print "%36s " % (unc[1]+" "+shift),
        
        avgShift = 0.0
        for ibin in range(4):
            nominal = nominalHist.GetBinContent(ibin+1)
            sysVal = hist.GetBinContent(ibin+1)
            print "& %+11.1f " % ((sysVal-nominal)*100.),
            shiftMin = min(shiftMin,(sysVal-nominal)/nominal)
            shiftMax = max(shiftMax,(sysVal-nominal)/nominal)
            avgShift+=sysVal/nominal
        print "\\\\"
        averageShifts[unc[0]].append(avgShift/4.0)
        
        hist.SetLineWidth(2)
        #hist.SetLineStyle(2)
        sysDict.append(hist)
        f.Close()
    print "\\hline"
    print "shift range = %+5.1f - %+5.1f => %5.1f "%(100.0*shiftMin,100.0*shiftMax,0.5*(math.fabs(100.0*shiftMin)+math.fabs(100.0*shiftMax)))
    sysHistograms.append(sysDict)
    
for unc in uncertaintiesSpecial:
    sysDict = []
    averageShifts[unc[0]]=[]
    f = ROOT.TFile(inputFolder+"/"+unc[0]+"/"+rootFileName+".root")
    hist = f.Get(histName).Clone()
    hist.SetDirectory(0)
    normalizeByBinWidth(hist,isSys=True)
    print "%36s " % (unc[1]),
    sysDict.append(nominalHist)
    avgShift = 0.0
    for ibin in range(4):
        nominal = nominalHist.GetBinContent(ibin+1)
        sysVal = hist.GetBinContent(ibin+1)
        print "& %+11.1f " % ((sysVal-nominal)*100.), 
        avgShift+=sysVal/nominal
    print "\\\\"
    averageShifts[unc[0]].append(avgShift/4.0)
    
    hist.SetLineWidth(2)
    sysDict.append(hist)
    f.Close()
    print "\\hline"
    sysHistograms.append(sysDict)





fPowheg = ROOT.TFile(inputFolder+"/Powheg/"+rootFileName+".root")
genPowhegHist=fPowheg.Get(responseName).Clone().ProjectionX("gen_powheg")
genPowhegHist.SetDirectory(0)
genPowhegHist.SetLineColor(ROOT.kOrange+7)
genPowhegHist.SetLineWidth(2)
genPowhegHist.SetLineStyle(1)
normalizeByBinWidth(genPowhegHist)

fAMC5FS = ROOT.TFile(inputFolder+"/AMC5FS/"+rootFileName+".root")
genAMC5FSHist=fAMC5FS.Get(responseName).Clone().ProjectionX("gen_AMC5FS")
genAMC5FSHist.SetDirectory(0)
genAMC5FSHist.SetLineColor(ROOT.kTeal+4)
genAMC5FSHist.SetLineWidth(5)
genAMC5FSHist.SetLineStyle(3)
normalizeByBinWidth(genAMC5FSHist)

fHerwig = ROOT.TFile(inputFolder+"/Herwig/"+rootFileName+".root")
genHerwigHist=fHerwig.Get(responseName).Clone().ProjectionX("gen_Herwig")
genHerwigHist.SetDirectory(0)
genHerwigHist.SetLineColor(ROOT.kGray+1)
genHerwigHist.SetLineWidth(3)
genHerwigHist.SetLineStyle(2)
normalizeByBinWidth(genHerwigHist)

for i,sys in enumerate(uncertainties+uncertaintiesSpecial):
    cvSys = ROOT.TCanvas("cvSys"+str(random.random()),"",800,750)
    ymax = max([sysHistograms[i][0].GetMaximum(),sysHistograms[i][1].GetMaximum(),genHist.GetMaximum(),nominalHist.GetMaximum()])
    if rootFileName == "unfoldingPt":
        if normalize:
            axis = ROOT.TH2F("axisUnfold",";p#lower[0.4]{#scale[0.7]{T}}#kern[-0.5]{ }(t+t#lower[-0.87]{#kern[-0.91]{-}}) (GeV);1 #/#sigma #times d#sigma #/d#kern[-0.5]{ }p#lower[0.4]{#scale[0.7]{T}}#kern[-0.5]{ }(t+t#lower[-0.87]{#kern[-0.89]{-}}) / #lower[-0.12]{#scale[0.7]{#frac{10#scale[0.8]{#lower[-0.4]{-3}}}{GeV}}}",50,genHist.GetXaxis().GetXmin(),genHist.GetXaxis().GetXmax(),50,0,1.3*ymax)
        else:
            axis = ROOT.TH2F("axisUnfold",";p#lower[0.4]{#scale[0.7]{T}}#kern[-0.5]{ }(t+t#lower[-0.87]{#kern[-0.91]{-}}) (GeV);a.u.",50,genHist.GetXaxis().GetXmin(),genHist.GetXaxis().GetXmax(),50,0,1.4*ymax)
    else:
        if normalize:
            axis = ROOT.TH2F("axisUnfold"+str(random.random()),";|y|#kern[-0.5]{ }(t+t#lower[-0.87]{#kern[-0.91]{-}}) (GeV);1 #/#sigma #times d#sigma #/d#kern[-0.5]{ }|y|#kern[-0.5]{ }(t+t#lower[-0.87]{#kern[-0.89]{-}})",50,genHist.GetXaxis().GetXmin(),genHist.GetXaxis().GetXmax(),50,0,1.3*ymax)
        else:
            axis = ROOT.TH2F("axisUnfold"+str(random.random()),";|y|#kern[-0.5]{ }(t+t#lower[-0.87]{#kern[-0.91]{-}}) (GeV);a.u.",50,genHist.GetXaxis().GetXmin(),genHist.GetXaxis().GetXmax(),50,0,1.65*ymax)
    axis.Draw("AXIS")
    genHist.Draw("HISTSame")
    nominalHist.Draw("PESame")
    sysHistograms[i][0].SetLineColor(ROOT.kViolet-1)
    sysHistograms[i][0].Draw("HISTSame")
    sysHistograms[i][1].SetLineColor(ROOT.kTeal+5)
    sysHistograms[i][1].Draw("HISTSame")
    
    
    legend=ROOT.TLegend(0.5,0.885,0.88,0.885-0.067*4)
    legend.SetBorderSize(0)
    legend.SetTextFont(43)
    legend.SetTextSize(28)
    legend.SetFillColor(ROOT.kWhite)
    
    legend.AddEntry(genHist,"Signal MC","L")
    legend.AddEntry(nominalHist,"Data "+"nominal","P")
    legend.AddEntry(sysHistograms[i][0],sys[0]+" up","L")
    legend.AddEntry(sysHistograms[i][1],sys[0]+" down","L")
    legend.Draw("Same")
    
    cvSys.Update()
    cvSys.Print(sys[0]+".pdf")
    cvSys.Print(sys[0]+".png")
    


statHistData = numpy.zeros((N,3))
totalHistData = numpy.zeros((N,3))
for ibin in range(N):
    totalHistData[ibin][0] = nominalHist.GetBinContent(ibin+1)
    totalHistData[ibin][1] = nominalHist.GetBinError(ibin+1)
    totalHistData[ibin][2] = nominalHist.GetBinError(ibin+1)
    

NTOYS = 50000



posterior = numpy.zeros((int(N),int(NTOYS)))
for itoy in range(NTOYS):
    for ibin in range(N):
        n = nominalHist.GetBinContent(ibin+1)
    
        posterior[ibin][itoy]+=numpy.random.normal()*nominalHist.GetBinError(ibin+1)
        
        for sysPair in sysHistograms:
            up = sysPair[0].GetBinContent(ibin+1)-n
            down = sysPair[1].GetBinContent(ibin+1)-n
            
            diced = numpy.random.normal()
            if diced<0:
                diced=math.fabs(diced)*down
            else:
                diced=math.fabs(diced)*up
            posterior[ibin][itoy]+=diced   
         

         
sumTot = 0.0
for ibin in range(N):
    n = nominalHist.GetBinContent(ibin+1)
    posteriorQuantils = numpy.percentile(posterior[ibin],[15.865,50.0,84.135])
    totalHistData[ibin][0] = n+posteriorQuantils[1]
    totalHistData[ibin][1] = n+posteriorQuantils[0]
    totalHistData[ibin][2] = n+posteriorQuantils[2]

    statHistData[ibin][0] = n+posteriorQuantils[1]
    statHistData[ibin][1] = statHistData[ibin][0]-nominalHist.GetBinError(ibin+1)
    statHistData[ibin][2] = statHistData[ibin][0]+nominalHist.GetBinError(ibin+1)

    sumTot+=(n+posteriorQuantils[1])*nominalHist.GetBinWidth(ibin+1)
    
scale =1.0
if rootFileName == "unfoldingPt":
    scale =1000.0
    
covariance = numpy.cov(posterior)*100.
print "uncertainty from cov"
for ibin in range(N):
    print "\t", ibin, scale*math.sqrt(covariance[ibin][ibin])/sumTot

print "bin-by-bin covariance"
for ibin in range(N):
    print "bin %i "%(ibin+1),
    for jbin in range(N):
        xy = covariance[ibin][jbin]
        print "& %+6.3f  "%(xy),
    print " \\\\"

print "bin-by-bin correlations"
for ibin in range(N):
    print "bin %i "%(ibin+1),
    for jbin in range(N):
        xy = covariance[ibin][jbin]
        xx = covariance[ibin][ibin]
        yy = covariance[jbin][jbin]
        corr = xy/math.sqrt(xx*yy)
        print "& %+6.3f  "%(corr),
    print " \\\\"
    
    
    
for ibin in range(N):
    totalHistData[ibin][0]=scale*totalHistData[ibin][0]/sumTot
    totalHistData[ibin][1]=scale*totalHistData[ibin][1]/sumTot
    totalHistData[ibin][2]=scale*totalHistData[ibin][2]/sumTot
    
    statHistData[ibin][0]=scale*statHistData[ibin][0]/sumTot
    statHistData[ibin][1]=scale*statHistData[ibin][1]/sumTot
    statHistData[ibin][2]=scale*statHistData[ibin][2]/sumTot
    
testSum = 0.0
for ibin in range(N):
    testSum+=totalHistData[ibin][0]*nominalHist.GetBinWidth(ibin+1)
print testSum


cv = ROOT.TCanvas("cv","",800,750)
if rootFileName == "unfoldingPt":
    if normalize:
        axis = ROOT.TH2F("axisUnfold",";p#lower[0.4]{#scale[0.7]{T}}#kern[-0.5]{ }(t+t#lower[-0.87]{#kern[-0.91]{-}}) (GeV);1 #/#sigma #times d#sigma #/d#kern[-0.5]{ }p#lower[0.4]{#scale[0.7]{T}}#kern[-0.5]{ }(t+t#lower[-0.87]{#kern[-0.89]{-}}) / #lower[-0.12]{#scale[0.7]{#frac{10#scale[0.8]{#lower[-0.4]{-3}}}{GeV}}}",50,genHist.GetXaxis().GetXmin(),genHist.GetXaxis().GetXmax(),50,0,12)
    else:
        axis = ROOT.TH2F("axisUnfold",";p#lower[0.4]{#scale[0.7]{T}}#kern[-0.5]{ }(t+t#lower[-0.87]{#kern[-0.91]{-}}) (GeV);a.u.",50,genHist.GetXaxis().GetXmin(),genHist.GetXaxis().GetXmax(),50,0,genHist.GetMaximum()*1.65)
else:
    if normalize:
        axis = ROOT.TH2F("axisUnfold",";|y|#kern[-0.5]{ }(t+t#lower[-0.87]{#kern[-0.91]{-}}) (GeV);1 #/#sigma #times d#sigma #/d#kern[-0.5]{ }|y|#kern[-0.5]{ }(t+t#lower[-0.87]{#kern[-0.89]{-}})",50,genHist.GetXaxis().GetXmin(),genHist.GetXaxis().GetXmax(),50,0,1.2)
    else:
        axis = ROOT.TH2F("axisUnfold",";|y|#kern[-0.5]{ }(t+t#lower[-0.87]{#kern[-0.91]{-}}) (GeV);a.u.",50,genHist.GetXaxis().GetXmin(),genHist.GetXaxis().GetXmax(),50,0,genHist.GetMaximum()*2.5)

axis.GetXaxis().SetTickLength(0.015/(1-cv.GetLeftMargin()-cv.GetRightMargin()))
axis.GetYaxis().SetTickLength(0.015/(1-cv.GetTopMargin()-cv.GetBottomMargin()))
axis.Draw("AXIS")

legend=ROOT.TLegend(0.55,0.89,0.88,0.49)
legend.SetBorderSize(0)
legend.SetTextFont(43)
legend.SetTextSize(30)
legend.SetFillStyle(0)

genHist.Draw("HISTSame")
legend.AddEntry(nominalHist,"Data","P")

legend.AddEntry(genHist,"aMC@NLO (4#kern[-0.5]{ }FS)","L")
legend.AddEntry(""," + Pythia#kern[-0.5]{ }8","")

genPowhegHist.Draw("HISTSame")
legend.AddEntry(genPowhegHist,"Powheg (4#kern[-0.5]{ }FS)","L")
legend.AddEntry(""," + Pythia#kern[-0.5]{ }8","")

genAMC5FSHist.Draw("HISTSame")
legend.AddEntry(genAMC5FSHist,"aMC@NLO (5#kern[-0.5]{ }FS)","L")
legend.AddEntry(""," + Pythia#kern[-0.5]{ }8","")

genHerwigHist.Draw("HISTSame")
legend.AddEntry(genHerwigHist,"aMC@NLO (4#kern[-0.5]{ }FS)","L")
legend.AddEntry(""," + Herwig#kern[-0.5]{ }","")




print "%36s " % ("total"),
for ibin in range(N):
    print "& %11.1f " % (totalHistData[ibin][0]*100.0), 
print "\\\\"
print "%36s " % ("total Up"),
for ibin in range(N):
    print "& %+11.1f " % ((totalHistData[ibin][2]-totalHistData[ibin][0])*100.0), 
print "\\\\"
print "%36s " % ("total Down"),
for ibin in range(N):
    print "& %+11.1f " % ((totalHistData[ibin][1]-totalHistData[ibin][0])*100.0), 
print "\\\\"


rootObj = []
for ibin in range(N):
    totalN = max(0,totalHistData[ibin][0])*10
    totalDown = max(0,totalHistData[ibin][1])*10
    totalUp = max(0,totalHistData[ibin][2])*10
    
    statDown = max(0,statHistData[ibin][1])*10
    statUp = max(0,statHistData[ibin][2])*10
    
    statPlus = statUp-totalN
    statMinus = statDown-totalN
    statUnc = (statPlus-statMinus)*.5
    totalPlus = totalUp-totalN
    totalMinus = totalDown-totalN
    systPlus = math.sqrt(totalPlus**2-statPlus**2)
    systMinus = -math.sqrt(totalMinus**2-statMinus**2)
    
    xdown = genHist.GetXaxis().GetBinLowEdge(ibin+1)
    xup = genHist.GetXaxis().GetBinUpEdge(ibin+1)
    
    
    print "bin %i "%(ibin+1),
    print " & [%3.2f; %3.2f]"%(xdown,xup),
    print " & $%5.2f$"%(totalN),
    print " & $\\pm%5.2f$"%(statUnc),
    print " & ${}^{%+5.2f}_{%+5.2f}$"%(systPlus,systMinus),
    print " & ${}^{%+5.2f}_{%+5.2f}$"%(totalPlus,totalMinus),
    print " & $%5.2f$"%(genHist.GetBinContent(ibin+1)*10), #aMCatNLO 4FS P8
    print " & $%5.2f$"%(genPowhegHist.GetBinContent(ibin+1)*10), #powheg 4FS P8
    print " & $%5.2f$"%(genAMC5FSHist.GetBinContent(ibin+1)*10), #aMCatNLO 5FS P8
    print " & $%5.2f$"%(genHerwigHist.GetBinContent(ibin+1)*10), #aMCatNLO 4FS H++
    print " \\\\"
    
    c = genHist.GetBinCenter(ibin+1)
    w = (genHist.GetXaxis().GetXmax()-genHist.GetXaxis().GetXmin())/12.0#genHist.GetBinWidth(ibin+1)
    
    marker = ROOT.TMarker(c,totalN,20)
    rootObj.append(marker)
    marker.SetMarkerSize(1.2)
    marker.Draw("Same")

    hLine = ROOT.TLine(c,totalDown,c,totalUp)
    rootObj.append(hLine)
    hLine.SetLineWidth(1)
    hLine.Draw("Same")
    
    statUpLine = ROOT.TLine(c-w*0.22,statUp,c+w*0.22,statUp)
    rootObj.append(statUpLine)
    statUpLine.SetLineWidth(2)
    statUpLine.Draw("Same")
    
    statDownLine = ROOT.TLine(c-w*0.22,statDown,c+w*0.22,statDown)
    rootObj.append(statDownLine)
    statDownLine.SetLineWidth(2)
    statDownLine.Draw("Same")
    
  

pCMS=ROOT.TPaveText(cv.GetLeftMargin(),0.94,cv.GetLeftMargin(),0.94,"NDC")
pCMS.SetFillColor(ROOT.kWhite)
pCMS.SetBorderSize(0)
pCMS.SetTextFont(63)
pCMS.SetTextSize(35)
pCMS.SetTextAlign(11)
pCMS.AddText("CMS")
pCMS.Draw("Same")

pPreliminary=ROOT.TPaveText(cv.GetLeftMargin()+0.095,0.94,cv.GetLeftMargin()+0.095,0.94,"NDC")
pPreliminary.SetFillColor(ROOT.kWhite)
pPreliminary.SetBorderSize(0)
pPreliminary.SetTextFont(53)
pPreliminary.SetTextSize(35)
pPreliminary.SetTextAlign(11)
pPreliminary.AddText("Preliminary")
pPreliminary.Draw("Same")

pLumi=ROOT.TPaveText(1-cv.GetRightMargin(),0.94,1-cv.GetRightMargin(),0.94,"NDC")
pLumi.SetFillColor(ROOT.kWhite)
pLumi.SetBorderSize(0)
pLumi.SetTextFont(43)
pLumi.SetTextSize(35)
pLumi.SetTextAlign(31)
pLumi.AddText("2.3#kern[-0.5]{ }fb#lower[-0.7]{#scale[0.7]{-1}} (13#kern[-0.5]{ }TeV)")
pLumi.Draw("Same")



legend.Draw("Same")

cv.Update()
cv.Print(outputName+".pdf")
cv.Print(outputName+".png")




'''
for unc in sorted(uncertainties+uncertaintiesSpecial):
    print "%36s " % (unc[1]),
    if len(averageShifts[unc[0]])==2:
        if (math.fabs(averageShifts[unc[0]][0])<0.01) and (math.fabs(averageShifts[unc[0]][1])<0.01):
            print "$%8s\\%%$" % ("<1"),
        else if averageShifts[unc[0]][0]*averageShifts[unc[0]][1]<0:
            if math.fabs(math.fabs(averageShifts[unc[0]][0])-math.fabs(averageShifts[unc[0]][1]))<0.05:
                print "$\\pm %8.0f\\%%$" % (averageShifts[unc[0]][0]),
            else:
                print "$\\pm %8.0f\\%%$" % (averageShifts[unc[0]][0]),
'''
