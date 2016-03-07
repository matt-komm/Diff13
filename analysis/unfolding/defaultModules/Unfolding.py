import ROOT
import pyTool
import numpy
import math
import os
import os.path
import re
import random
import sys

from Module import Module

import logging

class Unfolding(Module):
    def __init__(self,options=[]):
        Module.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def doScan(self,tunfold,genBins,output):
        N=100
        xvalues = numpy.logspace(-8,-2,N)
        yvalues = numpy.zeros((len(genBins)-1,N))
        for i in range(N):
            bestTau = xvalues[i]
            covariance = ROOT.TH2D("correlation","",len(genBins)-1,genBins,len(genBins)-1,genBins)
            unfoldedHist = ROOT.TH1D("unfoldedHist","",len(genBins)-1,genBins)
            unfoldedHist.Sumw2()
            tunfold.doUnfolding(bestTau,unfoldedHist,covariance)
            rhos = ROOT.PyUtils.getBinByBinCorrelations(covariance)
            for ibin in range(len(genBins)-1):
                yvalues[ibin][i]=rhos[ibin]
        cv = ROOT.TCanvas("cvScan","",800,600)
        cv.SetLogx()
        cv.SetRightMargin(0.18)
        axis = ROOT.TH2F("axisScan",";#tau;#rho",50,xvalues[0],xvalues[-1],50,-1.1,1.1)
        axis.Draw("AXIS")
        rootObj =[]
        legend = ROOT.TLegend(0.83,0.9,0.99,0.5)
        legend.SetFillColor(ROOT.kWhite)
        legend.SetBorderSize(0)
        legend.SetTextFont(42)
        for ibin in range(len(genBins)-2):
            graph = ROOT.TGraph(N,xvalues,yvalues[ibin+1])
            graph.SetLineWidth(len(genBins)-ibin)
            graph.SetLineColor(ROOT.kBlue-ibin+2)
            rootObj.append(graph)
            graph.Draw("SameL")
            legend.AddEntry(graph,"#rho (1,"+str(ibin+2)+")","L")
        graph = ROOT.TGraph(N,xvalues,yvalues[0])
        graph.SetLineWidth(3)
        graph.SetLineColor(ROOT.kOrange+10)
        graph.SetLineStyle(2)
        rootObj.append(graph)
        graph.Draw("SameL")
        legend.AddEntry(graph,"#bar{#rho}","L")
        legend.Draw("Same")
        #cv.Update()
        #cv.WaitPrimitive()
        cv.Print(os.path.join(self.module("Utils").getOutputFolder(),output+".pdf"))
        cv.Print(os.path.join(self.module("Utils").getOutputFolder(),output+".png"))
        
        
        
    def unfold(self,responseMatrix,data,genBinning,scan=None,fixedTau=None):
        genHist = responseMatrix.ProjectionX(responseMatrix.GetName()+"genX")

        responseMatrixReweighted = responseMatrix.Clone(responseMatrix.GetName()+"Reweighted")
        
        for ibin in range(responseMatrix.GetNbinsX()):
            w = 1.0/genHist.GetBinContent(ibin+1)*genHist.Integral()/genHist.GetNbinsX()
            responseMatrixReweighted.SetBinContent(
                    ibin+1,
                    0,
                    responseMatrix.GetBinContent(ibin+1,0)*w
            )
        
        
        tunfold = ROOT.PyUnfold(responseMatrixReweighted)
        tunfold.setData(data)

        if scan:
            self.module("Unfolding").doScan(tunfold,genBinning,scan)
        if fixedTau==None:
            bestTau = tunfold.scanTau()
        else:
            bestTau=fixedTau
            

        self._logger.info("Found tau for regularization: "+str(bestTau))
        
        
        covariance = ROOT.TH2D("correlation","",len(genBinning)-1,genBinning,len(genBinning)-1,genBinning)
        unfoldedHist = ROOT.TH1D("unfoldedHist","",len(genBinning)-1,genBinning)
        unfoldedHist.Sumw2()
        tunfold.doUnfolding(bestTau,unfoldedHist,covariance,True,False,False)
        
        for ibin in range(unfoldedHist.GetNbinsX()):
            w = 1.0/genHist.GetBinContent(ibin+1)*genHist.Integral()/genHist.GetNbinsX()
            unfoldedHist.SetBinContent(
                ibin+1,
                unfoldedHist.GetBinContent(ibin+1)/w
            )
            unfoldedHist.SetBinError(
                ibin+1,
                unfoldedHist.GetBinError(ibin+1)/w
            )
        
        return unfoldedHist,covariance,bestTau
        
