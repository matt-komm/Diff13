import ROOT
import math
import numpy
import random
import os

from defaultModules import Module

import logging

class UtilsNoPseudo(Module.getClass("Utils")):
    def __init__(self,options=[]):
        UtilsNoPseudo.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getRecoSamplePrefix(self):
        return ""
        
    def getOutputFolder(self):
        return "/home/fynu/mkomm/Diff13/analysis/unfolding/result/NoPseudo"
        
class NoPseudo(Module.getClass("Program")):
    def __init__(self,options=[]):
        NoPseudo.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def execute(self):
        #self.module("HistogramsForFitting").buildFitModel()
        
        self.module("Utils").createOutputFolder()
        
        ROOT.gRandom.SetSeed(123)
        
        ### FITTING
        fitResult = self.module("ThetaFit").checkFitResult()
        if fitResult==None:
            self.module("ThetaModel").makeModel(pseudo=False,addCut="(Reconstructed_1__isBarrel==1)")
            #self.module("ThetaModel").makeModel(pseudo=True)
            self.module("ThetaFit").run()
            fitResult = self.module("ThetaFit").readFitResult()
            self.module("Drawing").drawFitCorrelation(fitResult["correlations"])
               
        ###YIELDS
        histogramsAllYield = self.module("HistogramCreator").loadHistograms("yields_all")
        if not histogramsAllYield:
            histogramsAllYield = self.module("HistogramCreator").makeHistograms(
                "Reconstructed_1__nSelectedJet*3+Reconstructed_1__nSelectedBJet",
                "(Reconstructed_1__nSelectedJet<4)*(Reconstructed_1__isBarrel==1)",
                numpy.linspace(-0.5,12.5,14),
                pseudo=False
            )
            self.module("HistogramCreator").scaleHistogramsToFitResult(histogramsAllYield,fitResult)
            self.module("HistogramCreator").saveHistograms(histogramsAllYield,"yields_all")
        histogramsMTWYield = self.module("HistogramCreator").loadHistograms("yields_mtw")
        if not histogramsMTWYield:
            histogramsMTWYield = self.module("HistogramCreator").makeHistograms(
                "Reconstructed_1__nSelectedJet*3+Reconstructed_1__nSelectedBJet",
                "(Reconstructed_1__nSelectedJet<4)*(Reconstructed_1__isBarrel==1)"+"*"+self.module("Utils").getMTWCutStr(),
                numpy.linspace(-0.5,12.5,14),
                pseudo=False
            )
            self.module("HistogramCreator").scaleHistogramsToFitResult(histogramsMTWYield,fitResult)
            self.module("HistogramCreator").saveHistograms(histogramsMTWYield,"yields_mtw")
        histogramsBDTYield = self.module("HistogramCreator").loadHistograms("yields_bdt")
        if not histogramsBDTYield:
            histogramsBDTYield = self.module("HistogramCreator").makeHistograms(
                "Reconstructed_1__nSelectedJet*3+Reconstructed_1__nSelectedBJet",
                "(Reconstructed_1__nSelectedJet<4)*(Reconstructed_1__isBarrel==1)"+"*"+self.module("Utils").getMTWCutStr()+"*"+self.module("Utils").getBDTCutStr(),
                numpy.linspace(-0.5,12.5,14),
                pseudo=False
            )
            self.module("HistogramCreator").scaleHistogramsToFitResult(histogramsBDTYield,fitResult)
            self.module("HistogramCreator").saveHistograms(histogramsBDTYield,"yields_bdt")
        
        ### RECO HIST AND SCALING
        histogramsPt = self.module("HistogramCreator").loadHistograms("reco_top_pt")
        if not histogramsPt:
            histogramsPt = self.module("HistogramCreator").makeHistograms(
                self.module("ResponseMatrixPt").getRecoUnfoldingVariable(),
                self.module("Utils").getCategoryCutStr(2,1)+"*"+self.module("Utils").getMTWCutStr()+"*"+self.module("Utils").getBDTCutStr(),
                self.module("ResponseMatrixPt").getRecoBinning(),
                pseudo=False
            )
            self.module("HistogramCreator").scaleHistogramsToFitResult(histogramsPt,fitResult)
            self.module("HistogramCreator").saveHistograms(histogramsPt,"reco_top_pt")
        '''
        for ibin in range(histogramsPt["data"]["hists"]["data"].GetNbinsX()):
            histogramsPt["data"]["hists"]["data"].SetBinContent(ibin+1,
                ROOT.gRandom.Poisson(histogramsPt["data"]["hists"]["data"].GetBinContent(ibin+1))
            )
        '''
        self.module("Drawing").plotHistograms(histogramsPt,"top quark pT","reco_top_Pt")
            
        histogramsY = self.module("HistogramCreator").loadHistograms("reco_top_y")
        if not histogramsY:
            histogramsY = self.module("HistogramCreator").makeHistograms(
                self.module("ResponseMatrixY").getRecoUnfoldingVariable(),
                self.module("Utils").getCategoryCutStr(2,1)+"*"+self.module("Utils").getMTWCutStr()+"*"+self.module("Utils").getBDTCutStr(),
                self.module("ResponseMatrixY").getRecoBinning(),
                pseudo=False
            )
            self.module("HistogramCreator").scaleHistogramsToFitResult(histogramsY,fitResult)
            self.module("HistogramCreator").saveHistograms(histogramsY,"reco_top_y")
        '''
        for ibin in range(histogramsY["data"]["hists"]["data"].GetNbinsX()):
            histogramsY["data"]["hists"]["data"].SetBinContent(ibin+1,
                ROOT.gRandom.Poisson(histogramsY["data"]["hists"]["data"].GetBinContent(ibin+1))
            )
        '''
        self.module("Drawing").plotHistograms(histogramsY,"top quark |y|","reco_top_Y")
        
       
       
       
        ### RESPONSEMATRIX
        responseMatrixPt = self.module("ResponseMatrixPt").loadResponseMatrix()
        if responseMatrixPt==None:
            responseMatrixPt = self.module("ResponseMatrixPt").getResponseMatrix()
            self.module("ResponseMatrixPt").saveResponseMatrix(responseMatrixPt)
        self.module("Drawing").drawResponseMatrix(responseMatrixPt,"top quark pT","responsePt")
        responseMatrixPt.Scale(0.7)
        
        responseMatrixY = self.module("ResponseMatrixY").loadResponseMatrix()
        if responseMatrixY==None:
            responseMatrixY = self.module("ResponseMatrixY").getResponseMatrix()
            self.module("ResponseMatrixY").saveResponseMatrix(responseMatrixY)
        self.module("Drawing").drawResponseMatrix(responseMatrixY,"top quark |y|","responseY")
        responseMatrixY.Scale(0.7)




        ### BACKGROUND SUBTRACTION
        genHistPt = responseMatrixPt.ProjectionX()
        genBinningPt = self.module("ResponseMatrixPt").getGenBinning()
        dataHistPt = histogramsPt["data"]["hists"]["data"]
        dataHistPtSubtracted = dataHistPt.Clone("datahistPt_subtracted")
        backgroundDictPt = {}
        
        for componentName in histogramsPt.keys():
            componentHist = None
            for componentSetName in histogramsPt[componentName]["hists"].keys():
                if componentHist==None:
                    componentHist = histogramsPt[componentName]["hists"][componentSetName].Clone(componentName+"_sum"+str(random.random()))
                else:
                    componentHist.Add(histogramsPt[componentName]["hists"][componentSetName])
            
            histogramsPt[componentName]["hists"]["sum"]=componentHist
            if componentName=="tChannel" or componentName=="data":
                continue
            dataHistPtSubtracted.Add(componentHist,-1.0)
            '''
            backgroundDictPt[componentName]={}
            backgroundDictPt[componentName]["hist"]=componentHist
            backgroundDictPt[componentName]["mean"]=1.0
            backgroundDictPt[componentName]["unc"]=fitResult[componentName]["unc"]
            '''
        
        
        
        
        genHistY = responseMatrixY.ProjectionX()
        genBinningY = self.module("ResponseMatrixY").getGenBinning()
        dataHistY = histogramsY["data"]["hists"]["data"]
        dataHistYSubtracted = dataHistY.Clone("datahistY_subtracted")
        backgroundDictY = {}

        for componentName in histogramsY.keys():
            componentHist = None
            for componentSetName in histogramsY[componentName]["hists"].keys():
                if componentHist==None:
                    componentHist = histogramsY[componentName]["hists"][componentSetName].Clone(componentName+"_sum"+str(random.random()))
                else:
                    componentHist.Add(histogramsY[componentName]["hists"][componentSetName])
            
            histogramsY[componentName]["hists"]["sum"]=componentHist
            if componentName=="tChannel" or componentName=="data":
                continue
            dataHistYSubtracted.Add(componentHist,-1.0)
            '''
            backgroundDictY[componentName]={}
            backgroundDictY[componentName]["hist"]=componentHist
            backgroundDictY[componentName]["mean"]=1.0
            backgroundDictY[componentName]["unc"]=fitResult[componentName]["unc"]
            '''
            

        

          
            
        ### UNFOLDING        
        dataHistPtMatrix=responseMatrixPt.ProjectionY()
        self.module("Drawing").drawDataSubtracted(dataHistPtSubtracted,dataHistPtMatrix,"top quark pT","datasubtracted_Pt")
        self.module("Drawing").drawDataSubtracted(dataHistPtSubtracted,histogramsPt["tChannel"]["hists"]["sum"],"top quark pT","datasubtracted2_Pt")
        unfoldedHistPt, covariancePt = self.module("Unfolding").unfold(responseMatrixPt,dataHistPtSubtracted,genBinningPt)
        
        rootFile = ROOT.TFile(os.path.join(self.module("Utils").getOutputFolder(),"unfoldingPt.root"),"RECREATE")
        unfoldedHistPtWrite = unfoldedHistPt.Clone("unfoldedHistPt")
        unfoldedHistPtWrite.SetDirectory(rootFile)
        unfoldedHistPtWrite.Write()
        covariancePtWrite = covariancePt.Clone("covariancePt")
        covariancePtWrite.SetDirectory(rootFile)
        covariancePtWrite.Write()
        responseMatrixPtWrite = responseMatrixPt.Clone("responsePt")
        responseMatrixPtWrite.SetDirectory(rootFile)
        responseMatrixPtWrite.Write()
        dataHistPtSubtractedWrite = dataHistPtSubtracted.Clone("dataSubtractedPt")
        dataHistPtSubtractedWrite.SetDirectory(rootFile)
        dataHistPtSubtractedWrite.Write()
        rootFile.Close()

        self.module("Utils").normalizeByBinWidth(unfoldedHistPt)
        self.module("Utils").normalizeByBinWidth(genHistPt)
        self.module("Drawing").drawBiasTest(unfoldedHistPt,genHistPt,"top quark pT","bias_Pt")
        
        
        

        dataHistYMatrix=responseMatrixY.ProjectionY()
        self.module("Drawing").drawDataSubtracted(dataHistYSubtracted,dataHistYMatrix,"top quark |y|","datasubtracted_Y")
        self.module("Drawing").drawDataSubtracted(dataHistYSubtracted,histogramsY["tChannel"]["hists"]["sum"],"top quark |y|","datasubtracted2_Y")
        unfoldedHistY, covarianceY = self.module("Unfolding").unfold(responseMatrixY,dataHistYSubtracted,genBinningY)
        
        rootFile = ROOT.TFile(os.path.join(self.module("Utils").getOutputFolder(),"unfoldingY.root"),"RECREATE")
        unfoldedHistYWrite = unfoldedHistY.Clone("unfoldedHistY")
        unfoldedHistYWrite.SetDirectory(rootFile)
        unfoldedHistYWrite.Write()
        covarianceYWrite = covarianceY.Clone("covarianceY")
        covarianceYWrite.SetDirectory(rootFile)
        covarianceYWrite.Write()
        responseMatrixYWrite = responseMatrixY.Clone("responseY")
        responseMatrixYWrite.SetDirectory(rootFile)
        responseMatrixYWrite.Write()
        dataHistYSubtractedWrite = dataHistYSubtracted.Clone("dataSubtractedY")
        dataHistYSubtractedWrite.SetDirectory(rootFile)
        dataHistYSubtractedWrite.Write()
        rootFile.Close()
        
        self.module("Utils").normalizeByBinWidth(unfoldedHistY)
        self.module("Utils").normalizeByBinWidth(genHistY)
        self.module("Drawing").drawBiasTest(unfoldedHistY,genHistY,"top quark |y|","bias_Y")

        
