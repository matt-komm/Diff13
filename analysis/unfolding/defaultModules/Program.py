import ROOT
import math
import numpy

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
        
        self.module("Utils").createOutputFolder()
        
        ### FITTING
        fitResult = self.module("ThetaFit").checkFitResult()
        if fitResult==None:
            #self.module("ThetaModel").makeModel(pseudo=True,addCut="(Reconstructed_1__isBarrel==1)")
            self.module("ThetaModel").makeModel(pseudo=True)
            self.module("ThetaFit").run()
            fitResult = self.module("ThetaFit").readFitResult()
            self.module("Drawing").drawFitCorrelation(fitResult["correlations"])
    
        ### RECO HIST AND SCALING
        histograms = self.module("HistogramCreator").loadHistograms("reco_top_pt")
        if not histograms:
            histograms = self.module("HistogramCreator").makeHistograms(
                self.module("ResponseMatrixPt").getRecoUnfoldingVariable(),
                self.module("Utils").getCategoryCutStr(2,1)+"*"+self.module("Utils").getMTWCutStr()+"*"+self.module("Utils").getBDTCutStr(),
                self.module("ResponseMatrixPt").getRecoBinning(),
                #numpy.linspace(0,250,20),
                pseudo=True
            )
            self.module("HistogramCreator").scaleHistogramsToFitResult(histograms,fitResult)
            self.module("HistogramCreator").saveHistograms(histograms,"reco_top_pt")

        self.module("Drawing").plotHistograms(histograms,"top pT","reco_top_pt")
        
       
        ### RESPONSEMATRIX
        responseMatrix = self.module("ResponseMatrixPt").loadResponseMatrix()
        if responseMatrix==None:
            responseMatrix = self.module("ResponseMatrixPt").getResponseMatrix()
            self.module("ResponseMatrixPt").saveResponseMatrix(responseMatrix)
        self.module("Drawing").drawResponseMatrix(responseMatrix,"top quark pT","responsePt")

        genHist = responseMatrix.ProjectionX()
        genBinning = self.module("ResponseMatrixPt").getGenBinning()
        
        
        dataHist = histograms["data"]["hists"]["data"]
        '''
        for ibin in range(dataHist.GetNbinsX()):
            dataHist.SetBinContent(ibin+1,
                ROOT.gRandom.Poisson(dataHist.GetBinContent(ibin+1))
            )
        '''
        #TODO: check uncertainties!!!
        
        for componentName in histograms.keys():
            if componentName=="tChannel" or componentName=="data":
                continue
            for componentSetName in histograms[componentName]["hists"].keys():
                for ibin in range(dataHist.GetNbinsX()):
                    d = dataHist.GetBinContent(ibin+1)
                    b = histograms[componentName]["hists"][componentSetName].GetBinContent(ibin+1)
                    n = d-b
                    if n<0:
                        n=0
                    dataHist.SetBinContent(ibin+1,n)
                    
        
        self.module("Drawing").plotHistogram(dataHist,"top pT","dataSubtracted")
        
        
        unfoldedHist, covariance = self.module("Unfolding").unfold(responseMatrix,dataHist,genBinning)

        #self.module("Utils").normalizeByBinWidth(unfoldedHist)
        #self.module("Utils").normalizeByBinWidth(genHist)

        self.module("Drawing").drawBiasTest(unfoldedHist,genHist,"top quark pT","biasPt")


        
        
