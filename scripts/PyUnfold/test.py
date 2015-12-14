#!/usr/bin/python

import ROOT
import math

ROOT.gSystem.Load("libpyUnfold.so")

data = ROOT.TH1F("data","",20,-1,1)
response = ROOT.TH2F("res","",10,-1,1,20,-1,1)

for i in range(20):
    data.SetBinContent((i+1),i+0.5)
    for j in range(10):
        response.SetBinContent(j+1,i+1,math.exp(-0.9*(i/2.0-j)*(i/2.0-j)))
        
data.Draw("")
#ROOT.gPad.WaitPrimitive()

unfold = ROOT.PyUnfold(response)
unfold.setData(data)

unfoldedHist = response.ProjectionX().Clone("unfoldedHist")
correlationMatrix = ROOT.TH2F("correlation","",
    unfoldedHist.GetNbinsX(),unfoldedHist.GetXaxis().GetXmin(),unfoldedHist.GetXaxis().GetXmax(),
    unfoldedHist.GetNbinsX(),unfoldedHist.GetXaxis().GetXmin(),unfoldedHist.GetXaxis().GetXmax()
)

unfold.unfold(0.,unfoldedHist,correlationMatrix)

unfoldedHist.Draw("colz")
ROOT.gPad.WaitPrimitive()
