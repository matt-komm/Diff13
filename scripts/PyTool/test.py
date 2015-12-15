#!/usr/bin/python

import ROOT
import math
import random

ROOT.gSystem.Load("libpyUnfold.so")

data = ROOT.TH1F("data","",24,-1,1)
response = ROOT.TH2F("res","",12,-1,1,24,-1,1)

for toy in range(10000):
    x = random.uniform(-1,1)
    smear = random.gauss(0,0.05)
    data.Fill(x+smear,x**2)
    response.Fill(x,x+smear)
        
#data.Draw()
#ROOT.gPad.WaitPrimitive()

unfold = ROOT.PyUnfold(response)
unfold.setData(data)

unfoldedHist = response.ProjectionX().Clone("unfoldedHist")
covariance = ROOT.TH2F("correlation","",
    unfoldedHist.GetNbinsX(),unfoldedHist.GetXaxis().GetXmin(),unfoldedHist.GetXaxis().GetXmax(),
    unfoldedHist.GetNbinsX(),unfoldedHist.GetXaxis().GetXmin(),unfoldedHist.GetXaxis().GetXmax()
)
print unfold.getNRecoBins(),unfold.getNGenBins()



bestTau = unfold.scanTau()

unfold.doUnfolding(bestTau,unfoldedHist,covariance)

print bestTau,ROOT.PyUtils.getBinByBinCorrelations(covariance)[0]

cv = ROOT.TCanvas("cv","",800,600)
cv.Divide(2,2)
cv.cd(1)
data.Draw()
cv.cd(2)
response.Draw("colz")

cv.cd(3)
unfoldedHist.Draw()
cv.cd(4)
covariance.Draw("colz")

ROOT.gPad.WaitPrimitive()
