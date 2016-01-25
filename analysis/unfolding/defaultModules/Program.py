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
        #self.module("HistogramsForFitting").buildFitModel()
    
    
        '''
        responseMatrix = self.module("ResponseMatrixY").getResponseMatrix()
        self.module("Drawing").drawResponseMatrix(responseMatrix,"top quark |y|","responseY")

        genHist = responseMatrix.ProjectionX()
        genBinning = self.module("ResponseMatrixY").getGenBinning()
        
        pseudoData = responseMatrix.ProjectionY("pseudodata")
        for ibin in range(pseudoData.GetNbinsX()):
            #print ibin+1,pseudoData.GetBinContent(ibin+1)
            pseudoData.SetBinError(ibin+1,math.sqrt(2*pseudoData.GetBinContent(ibin+1)))
            pass
        
        unfoldedHist, covariance = self.module("Unfolding").unfold(responseMatrix,pseudoData,genBinning)

        self.module("Drawing").drawBiasTest(unfoldedHist,genHist,"top quark |y|","biasY")


        #self.module("Utils").normalizeByBinWidth(unfoldedHist)
        #self.module("Utils").normalizeByBinWidth(genHist)
        '''
        
        self.module("ThetaModel").makeModel(pseudo=True)
        self.module("ThetaFit").run()
        result = self.module("ThetaFit").readFitResult()
        self.module("ThetaFit").plotCorrelations(result["cov"])
        
