import ROOT
import math
import numpy
import random

from Module import Module

import logging

class Program(Module):
    def __init__(self,options=[]):
        Module.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def execute(self):
        #self.module("HistogramsForFitting").buildFitModel()
        
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
        responseMatrix.Scale(0.7)


        genHist = responseMatrix.ProjectionX()
        genBinning = self.module("ResponseMatrixPt").getGenBinning()
        
        
        dataHist = histograms["data"]["hists"]["data"]
        #dataHist = responseMatrix.ProjectionY()
        '''
        for ibin in range(dataHist.GetNbinsX()):
            
            dataHist.SetBinContent(ibin+1,
                ROOT.gRandom.Poisson(dataHist.GetBinContent(ibin+1))
            )
            
            dataHist.SetBinError(ibin+1,
                math.sqrt(dataHist.GetBinContent(ibin+1))
            )
        '''
        
        dataHistSubtracted = dataHist.Clone("datahist_subtracted")
        backgroundDict = {}
        
        for componentName in histograms.keys():
            
            componentHist = None
            for componentSetName in histograms[componentName]["hists"].keys():
                if componentHist==None:
                    componentHist = histograms[componentName]["hists"][componentSetName].Clone(componentName+"_sum"+str(random.random()))
                else:
                    componentHist.Add(histograms[componentName]["hists"][componentSetName])
            
            histograms[componentName]["hists"]["sum"]=componentHist
            if componentName=="tChannel" or componentName=="data":
                continue
            '''
            backgroundDict[componentName]={}
            backgroundDict[componentName]["hist"]=componentHist
            backgroundDict[componentName]["mean"]=1.0
            backgroundDict[componentName]["unc"]=fitResult[componentName]["unc"]
            '''
            dataHistSubtracted.Add(componentHist,-1.0)
            

        self.module("Drawing").plotHistogram(dataHistSubtracted,"top pT","dataSubtracted")
        self.module("Drawing").plotHistogram(histograms["tChannel"]["hists"]["sum"],"top pT","MCsignal")
        
        mcTruth = responseMatrix.ProjectionY()
        mcTruth.SetMarkerStyle(20)
        mcTruth.SetMarkerSize(1)
        self.module("Drawing").plotHistogram(mcTruth,"top pT","MCtruth")
        
        unfoldedHist, covariance = self.module("Unfolding").unfold(responseMatrix,dataHistSubtracted,genBinning)

        #self.module("Utils").normalizeByBinWidth(unfoldedHist)
        #self.module("Utils").normalizeByBinWidth(genHist)

        self.module("Drawing").drawBiasTest(unfoldedHist,genHist,"top quark pT","biasPt")


        
        
