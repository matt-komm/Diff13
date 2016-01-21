import ROOT
import math

from Module import Module

import logging

class Program(Module):
    def __init__(self,options=[]):
        Module.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def execute(self):
        
        responseMatrix = self.module("ResponseMatrixY").getResponseMatrix()
        responseMatrixNorm = self.module("Utils").normalizeByTransistionProbability(responseMatrix)
        genHist = responseMatrix.ProjectionX()
        genBinning = self.module("ResponseMatrixY").getGenBinning()
        
        pseudoData = responseMatrix.ProjectionY("pseudodata")
        for ibin in range(pseudoData.GetNbinsX()):
            #print ibin+1,pseudoData.GetBinContent(ibin+1)
            pseudoData.SetBinError(ibin+1,math.sqrt(2*pseudoData.GetBinContent(ibin+1)))
            pass
        
        unfoldedHist, covariance = self.module("Unfolding").unfold(responseMatrix,pseudoData,genBinning)
        
        
        cvResponse = ROOT.TCanvas("cvResponse","",800,600)
        responseMatrixNorm.Draw("colz text")
        
        
        cvUnfold = ROOT.TCanvas("cvUnfold","",800,600)
        #pseudoData.Draw()

        #self.module("Utils").normalizeByBinWidth(unfoldedHist)
        #self.module("Utils").normalizeByBinWidth(genHist)
        unfoldedHist.Draw()
        genHist.Draw("HISTSame")
        
        ROOT.gPad.Update()
        ROOT.gPad.WaitPrimitive()
        
