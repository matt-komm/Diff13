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
        
    def unfold(self,responseMatrix,data,genBinning,backgroundDict={}):
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
        for backgroundName in backgroundDict.keys():
            tunfold.addBackground(
                backgroundDict[backgroundName]["hist"],
                backgroundName,
                backgroundDict[backgroundName]["mean"],
                backgroundDict[backgroundName]["error"]
            )
        bestTau = tunfold.scanTau()

        print bestTau
        
        
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
        
        return unfoldedHist,covariance
        
