import ROOT
import math
import numpy
import random
import os

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
        
        ROOT.gRandom.SetSeed(123)
        
        ### FITTING
        fitResultNominal = self.module("ThetaFit").checkFitResult(modelName='fit')
        if fitResultNominal==None:
            #self.module("ThetaModel").makeModel(pseudo=True,addCut="(Reconstructed_1__isBarrel==1)")
            self.module("ThetaModel").makeModel(pseudo=False, modelName='fit')
            self.module("ThetaFit").run(modelName='fit')
            fitResultNominal = self.module("ThetaFit").readFitResult()
            
        '''
        ptRecoBinning = self.module("ResponseMatrixPt").getRecoBinning()
        ptRecoVariable = self.module("ResponseMatrixPt").getRecoUnfoldingVariable()
        for ipt in range(len(ptRecoBinning)-1):
            fitResultByPt = self.module("ThetaFit").checkFitResult(modelName="fit_pt"+str(ipt))
            if fitResultByPt==None:
                self.module("ThetaModel").makeModel(
                    pseudo=False, 
                    modelName="fit_pt"+str(ipt),
                    addCut="("+ptRecoVariable+">"+str(ptRecoBinning[ipt])+")*("+ptRecoVariable+"<"+str(ptRecoBinning[ipt+1])+")"
                )
                self.module("ThetaFit").run(modelName="fit_pt"+str(ipt))
                fitResultByPt = self.module("ThetaFit").readFitResult()
            
            
        yRecoBinning = self.module("ResponseMatrixY").getRecoBinning()
        yRecoVariable = self.module("ResponseMatrixY").getRecoUnfoldingVariable()
        for iy in range(len(yRecoBinning)-1):
            fitResultByY = self.module("ThetaFit").checkFitResult(modelName="fit_y"+str(iy))
            if fitResultByY==None:
                self.module("ThetaModel").makeModel(
                    pseudo=False, 
                    modelName="fit_y"+str(iy),
                    addCut="("+yRecoVariable+">"+str(yRecoBinning[iy])+")*("+yRecoVariable+"<"+str(yRecoBinning[iy+1])+")"
                )
                self.module("ThetaFit").run(modelName="fit_y"+str(iy))
                fitResultByY = self.module("ThetaFit").readFitResult()
            
        #self.module("Drawing").drawFitCorrelation(fitResult["correlations"])
            
        '''    
        ###YIELDS
        histogramsAllYield = self.module("HistogramCreator").loadHistograms("yields_all")
        if not histogramsAllYield:
            histogramsAllYield = self.module("HistogramCreator").makeHistograms(
                "Reconstructed_1__nSelectedJet*3+Reconstructed_1__nSelectedBJet",
                "(Reconstructed_1__nSelectedJet<4)",
                numpy.linspace(-0.5,12.5,14),
                pseudo=False
            )
            self.module("HistogramCreator").scaleHistogramsToFitResult(histogramsAllYield,fitResultNominal)
            self.module("HistogramCreator").saveHistograms(histogramsAllYield,"yields_all")
        histogramsMTWYield = self.module("HistogramCreator").loadHistograms("yields_mtw")
        if not histogramsMTWYield:
            histogramsMTWYield = self.module("HistogramCreator").makeHistograms(
                "Reconstructed_1__nSelectedJet*3+Reconstructed_1__nSelectedBJet",
                "(Reconstructed_1__nSelectedJet<4)"+"*"+self.module("Utils").getMTWCutStr(),
                numpy.linspace(-0.5,12.5,14),
                pseudo=False
            )
            self.module("HistogramCreator").scaleHistogramsToFitResult(histogramsMTWYield,fitResultNominal)
            self.module("HistogramCreator").saveHistograms(histogramsMTWYield,"yields_mtw")
        histogramsBDTYield = self.module("HistogramCreator").loadHistograms("yields_bdt")
        if not histogramsBDTYield:
            histogramsBDTYield = self.module("HistogramCreator").makeHistograms(
                "Reconstructed_1__nSelectedJet*3+Reconstructed_1__nSelectedBJet",
                "(Reconstructed_1__nSelectedJet<4)"+"*"+self.module("Utils").getMTWCutStr()+"*"+self.module("Utils").getBDTCutStr(),
                numpy.linspace(-0.5,12.5,14),
                pseudo=False
            )
            self.module("HistogramCreator").scaleHistogramsToFitResult(histogramsBDTYield,fitResultNominal)
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
            self.module("HistogramCreator").scaleHistogramsToFitResult(histogramsPt,fitResultNominal)
            self.module("HistogramCreator").saveHistograms(histogramsPt,"reco_top_pt")

        self.module("Drawing").plotHistograms(histogramsPt,"top quark pT","reco_top_Pt")
            
        histogramsY = self.module("HistogramCreator").loadHistograms("reco_top_y")
        if not histogramsY:
            histogramsY = self.module("HistogramCreator").makeHistograms(
                self.module("ResponseMatrixY").getRecoUnfoldingVariable(),
                self.module("Utils").getCategoryCutStr(2,1)+"*"+self.module("Utils").getMTWCutStr()+"*"+self.module("Utils").getBDTCutStr(),
                self.module("ResponseMatrixY").getRecoBinning(),
                pseudo=False
            )
            self.module("HistogramCreator").scaleHistogramsToFitResult(histogramsY,fitResultNominal)
            self.module("HistogramCreator").saveHistograms(histogramsY,"reco_top_y")

        self.module("Drawing").plotHistograms(histogramsY,"top quark |y|","reco_top_Y")
        
       
       
       
        ### RESPONSEMATRIX
        responseMatrixPt = self.module("ResponseMatrixPt").loadResponseMatrix()
        if responseMatrixPt==None:
            responseMatrixPt = self.module("ResponseMatrixPt").getResponseMatrix()
            self.module("ResponseMatrixPt").saveResponseMatrix(responseMatrixPt)
        self.module("Drawing").drawResponseMatrix(responseMatrixPt,"top quark pT","responsePt")
        responseMatrixPt.Scale(fitResultNominal["tChannel"]["mean"])
        
        responseMatrixY = self.module("ResponseMatrixY").loadResponseMatrix()
        if responseMatrixY==None:
            responseMatrixY = self.module("ResponseMatrixY").getResponseMatrix()
            self.module("ResponseMatrixY").saveResponseMatrix(responseMatrixY)
        self.module("Drawing").drawResponseMatrix(responseMatrixY,"top quark |y|","responseY")
        responseMatrixY.Scale(fitResultNominal["tChannel"]["mean"])




        ### BACKGROUND SUBTRACTION
        genHistPt = responseMatrixPt.ProjectionX()
        genBinningPt = self.module("ResponseMatrixPt").getGenBinning()
        dataHistPt = histogramsPt["data"]["hists"]["data"]
        dataHistPtSubtracted = dataHistPt.Clone("datahistPt_subtracted")
        dataHistPtSubtracted.SetDirectory(0)
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

        
        
        
        
        genHistY = responseMatrixY.ProjectionX()
        genBinningY = self.module("ResponseMatrixY").getGenBinning()
        dataHistY = histogramsY["data"]["hists"]["data"]
        dataHistYSubtracted = dataHistY.Clone("datahistY_subtracted")
        dataHistYSubtracted.SetDirectory(0)
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

            

        

          
            
        ### UNFOLDING        
        recoPtHist=responseMatrixPt.ProjectionY()
        recoPtHist.SetDirectory(0)
        self.module("Drawing").drawDataSubtracted(recoPtHist,histogramsPt["tChannel"]["hists"]["sum"],"top quark pT","datasubtracted_true_Pt")
        self.module("Drawing").drawDataSubtracted(dataHistPtSubtracted,histogramsPt["tChannel"]["hists"]["sum"],"top quark pT","datasubtracted_data_Pt")
        
        unfoldedHistPt, covariancePt, bestTau = self.module("Unfolding").unfold(
            responseMatrixPt,
            dataHistPtSubtracted,
            genBinningPt,
            scan="scanPt"
        )
        unfoldedHistPt.SetDirectory(0)
        covariancePt.SetDirectory(0)
        
        trueUnfoldedHistPt, trueCovariancePt, trueBestTau = self.module("Unfolding").unfold(
            responseMatrixPt,
            recoPtHist,
            genBinningPt,
            scan="trueScanPt"
        )
        trueUnfoldedHistPt.SetDirectory(0)
        trueCovariancePt.SetDirectory(0)
        
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

        #self.module("Utils").normalizeByBinWidth(unfoldedHistPt)
        #self.module("Utils").normalizeByBinWidth(trueUnfoldedHistPt)
        #self.module("Utils").normalizeByBinWidth(genHistPt)
        self.module("Drawing").drawBiasTest(trueUnfoldedHistPt,genHistPt,"top quark pT","unfolded_true_Pt")
        self.module("Drawing").drawBiasTest(unfoldedHistPt,genHistPt,"top quark pT","unfolded_data_Pt")
        
        

        recoYHist=responseMatrixY.ProjectionY()
        recoYHist.SetDirectory(0)
        self.module("Drawing").drawDataSubtracted(recoYHist,histogramsY["tChannel"]["hists"]["sum"],"top quark |y|","datasubtracted_true_Y")
        self.module("Drawing").drawDataSubtracted(dataHistYSubtracted,histogramsY["tChannel"]["hists"]["sum"],"top quark |y|","datasubtracted_data_Y")
        unfoldedHistY, covarianceY, bestTau = self.module("Unfolding").unfold(
            responseMatrixY,
            dataHistYSubtracted,
            genBinningY,
            scan="scanY"
        )
        unfoldedHistY.SetDirectory(0)
        covarianceY.SetDirectory(0)
        
        trueUnfoldedHistY, trueCovarianceY, trueBestTau = self.module("Unfolding").unfold(
            responseMatrixY,
            recoYHist,
            genBinningY,
            scan="trueScanY"
        )
        trueUnfoldedHistY.SetDirectory(0)
        trueCovarianceY.SetDirectory(0)
        
        
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
        
        #self.module("Utils").normalizeByBinWidth(unfoldedHistY)
        #self.module("Utils").normalizeByBinWidth(trueUnfoldedHistY)
        #self.module("Utils").normalizeByBinWidth(genHistY)
        self.module("Drawing").drawBiasTest(trueUnfoldedHistY,genHistY,"top quark |y|","unfolded_true_Y")
        self.module("Drawing").drawBiasTest(unfoldedHistY,genHistY,"top quark |y|","unfolded_data_Y")
        
        
