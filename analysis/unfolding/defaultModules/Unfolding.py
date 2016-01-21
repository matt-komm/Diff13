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
        
    def unfold(self,responseMatrix,data,genBinning):
        genHist = responseMatrix.ProjectionX(responseMatrix.GetName()+"genX")

        responseMatrixReweighted = responseMatrix.Clone(responseMatrix.GetName()+"Reweighted")
        
        for ibin in range(responseMatrix.GetNbinsX()):
            '''
            colSum = 0.0
            for jbin in range(responseMatrix.GetNbinsY()):
                colSum += responseMatrix.GetBinContent(ibin+1,jbin+1)
            
            for jbin in range(responseMatrix.GetNbinsY()):
                responseMatrixReweighted.SetBinContent(
                    ibin+1,
                    jbin+1,
                    responseMatrix.GetBinContent(ibin+1,jbin+1)/colSum*responseMatrix.Integral()/(responseMatrix.GetNbinsX()*responseMatrix.GetNbinsY())
                )
            '''
            '''
            responseMatrixReweighted.SetBinContent(
                    ibin+1,
                    0,
                    responseMatrix.GetBinContent(ibin+1,0)/genHist.GetBinContent(ibin+1)*genHist.Integral()/genHist.GetNbinsX()
            )
            '''
        
        tunfold = ROOT.PyUnfold(responseMatrixReweighted)
        
            
        tunfold.setData(data)
        bestTau = tunfold.scanTau()
        print bestTau
        
        
        covariance = ROOT.TH2D("correlation","",len(genBinning)-1,genBinning,len(genBinning)-1,genBinning)
        unfoldedHist = ROOT.TH1D("unfoldedHist","",len(genBinning)-1,genBinning)
        unfoldedHist.Sumw2()
        tunfold.doUnfolding(bestTau,unfoldedHist,covariance)
        '''
        for ibin in range(unfoldedHist.GetNbinsX()):
            unfoldedHist.SetBinContent(
                ibin+1,
                unfoldedHist.GetBinContent(ibin+1)*genHist.GetBinContent(ibin+1)/genHist.Integral()*genHist.GetNbinsX()
            )
        '''
        return unfoldedHist,covariance
        
