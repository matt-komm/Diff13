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
        
        
        multiFitResultsPt=[]
        
        ### FITTING
        fitResultInc = self.module("ThetaFit").checkFitResult(modelName='fit')
        if fitResultInc==None:
            #self.module("ThetaModel").makeModel(pseudo=True,addCut="(Reconstructed_1__isBarrel==1)")
            self.module("ThetaModel").makeModel(pseudo=False, modelName='fit')
            self.module("ThetaFit").run(modelName='fit')
            fitResultInc = self.module("ThetaFit").readFitResult()
            
        self.module("Drawing").drawFitCorrelation(fitResultInc["correlations"])
        
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
                fitResultByPt = self.module("ThetaFit").readFitResult(modelName="fit_pt"+str(ipt))
            multiFitResultsPt.append({
                "range":[ptRecoBinning[ipt],ptRecoBinning[ipt+1]],
                "res":fitResultByPt
            })
            
        fmultiFitResultsPt=open(os.path.join(self.module("Utils").getOutputFolder(),"multiresultPt.txt"),"w")
        for comp in self.module("ThetaModel").getComponentsDict().keys():
            fmultiFitResultsPt.write(comp+" = \"(0\"\r")
            for ipt,fitResultByPt in enumerate(multiFitResultsPt):
                addCut=comp+" += \" + ("+ptRecoVariable+">"+str(ptRecoBinning[ipt])+")*("+ptRecoVariable+"<"+str(ptRecoBinning[ipt+1])+")*"+str(round(fitResultByPt["res"][comp]["mean"],3))+"\""
                fmultiFitResultsPt.write(addCut+'\r')
            fmultiFitResultsPt.write(comp+" += \")\"\r")
        fmultiFitResultsPt.close()
                
        multiFitResultsPt.append({
            "range":[ptRecoBinning[0],ptRecoBinning[-1]],
            "res":fitResultInc
        })
        
        self.module("Drawing").drawMultiFitResults(multiFitResultsPt,"top_pt")
        
        
        multiFitResultsY=[]
            
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
                fitResultByY = self.module("ThetaFit").readFitResult(modelName="fit_y"+str(iy))
            multiFitResultsY.append({
                "range":[yRecoBinning[iy],yRecoBinning[iy+1]],
                "res":fitResultByY
            })
            
        fmultiFitResultsY=open(os.path.join(self.module("Utils").getOutputFolder(),"multiresultY.txt"),"w")
        for comp in self.module("ThetaModel").getComponentsDict().keys():
            fmultiFitResultsY.write(comp+" = \"(0\"\r")
            for iy,fitResultByY in enumerate(multiFitResultsY):
                addCut=comp+" += \" + ("+yRecoVariable+">"+str(yRecoBinning[iy])+")*("+yRecoVariable+"<"+str(yRecoBinning[iy+1])+")*"+str(round(fitResultByY["res"][comp]["mean"],3))+"\""
                fmultiFitResultsY.write(addCut+'\r')
            fmultiFitResultsY.write(comp+" += \")\"\r")
        fmultiFitResultsY.close()
        
        multiFitResultsY.append({
            "range":[yRecoBinning[0],yRecoBinning[-1]],
            "res":fitResultInc
        })
        
        self.module("Drawing").drawMultiFitResults(multiFitResultsY,"top_y")
        
        
        

            
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
        self.module("HistogramCreator").saveHistograms(histogramsAllYield,"yields_all")
        self.module("HistogramCreator").scaleHistogramsToFitResult(histogramsAllYield,fitResultInc)
        self.module("HistogramCreator").saveHistograms(histogramsAllYield,"yields_all_scaled")
            
        histogramsMTWYield = self.module("HistogramCreator").loadHistograms("yields_mtw")
        if not histogramsMTWYield:
            histogramsMTWYield = self.module("HistogramCreator").makeHistograms(
                "Reconstructed_1__nSelectedJet*3+Reconstructed_1__nSelectedBJet",
                "(Reconstructed_1__nSelectedJet<4)"+"*"+self.module("Utils").getMTWCutStr(),
                numpy.linspace(-0.5,12.5,14),
                pseudo=False
            )
        self.module("HistogramCreator").saveHistograms(histogramsMTWYield,"yields_mtw")
        self.module("HistogramCreator").scaleHistogramsToFitResult(histogramsMTWYield,fitResultInc)
        self.module("HistogramCreator").saveHistograms(histogramsMTWYield,"yields_mtw_scaled")
            
        histogramsBDTYield = self.module("HistogramCreator").loadHistograms("yields_bdt")
        if not histogramsBDTYield:
            histogramsBDTYield = self.module("HistogramCreator").makeHistograms(
                "Reconstructed_1__nSelectedJet*3+Reconstructed_1__nSelectedBJet",
                "(Reconstructed_1__nSelectedJet<4)"+"*"+self.module("Utils").getMTWCutStr()+"*"+self.module("Utils").getBDTCutStr(),
                numpy.linspace(-0.5,12.5,14),
                pseudo=False
            )
        self.module("HistogramCreator").saveHistograms(histogramsBDTYield,"yields_bdt")
        self.module("HistogramCreator").scaleHistogramsToFitResult(histogramsBDTYield,fitResultInc)
        self.module("HistogramCreator").saveHistograms(histogramsBDTYield,"yields_bdt_scaled")
        '''
        
        
        
        ### RECO HIST AND SCALING
        histogramsPt = self.module("HistogramCreator").loadHistograms("reco_top_pt")
        if not histogramsPt:
            histogramsPt = self.module("HistogramCreator").makeHistograms(
                self.module("ResponseMatrixPt").getRecoUnfoldingVariable(),
                self.module("Utils").getCategoryCutStr(2,1),
                self.module("ResponseMatrixPt").getRecoBinning(),
                pseudo=False
            )
        self.module("HistogramCreator").saveHistograms(histogramsPt,"reco_top_pt")
        self._logger.info("scaling reco top pt")
        self.module("HistogramCreator").scaleHistogramsToMultiFitResult(histogramsPt,multiFitResultsPt)
        #self.module("HistogramCreator").scaleHistogramsToFitResult(histogramsPt,fitResultInc)
        self.module("HistogramCreator").saveHistograms(histogramsPt,"reco_top_pt_scaled")
        
        '''
        histogramsPt_BDT = self.module("HistogramCreator").loadHistograms("reco_top_pt_bdt")
        if not histogramsPt_BDT:
            histogramsPt_BDT = self.module("HistogramCreator").makeHistograms(
                self.module("ResponseMatrixPt").getRecoUnfoldingVariable(),
                self.module("Utils").getCategoryCutStr(2,1)+"*"+self.module("Utils").getMTWCutStr()+"*"+self.module("Utils").getBDTCutStr(),
                self.module("ResponseMatrixPt").getRecoBinning(),
                pseudo=False
            )
        self.module("HistogramCreator").saveHistograms(histogramsPt_BDT,"reco_top_pt_bdt")
        self.module("HistogramCreator").scaleHistogramsToMultiFitResult(histogramsPt_BDT,multiFitResultsPt)
        #self.module("HistogramCreator").scaleHistogramsToFitResult(histogramsPt,fitResultInc)
        self.module("HistogramCreator").saveHistograms(histogramsPt_BDT,"reco_top_pt_bdt_scaled")
        '''

        self.module("Drawing").plotHistograms(histogramsPt,"top quark pT","reco_top_Pt")
        #self.module("Drawing").plotHistograms(histogramsPt_BDT,"top quark pT","reco_top_Pt_bdt")
        
        
        
        
        
        histogramsY = self.module("HistogramCreator").loadHistograms("reco_top_y")
        if not histogramsY:
            histogramsY = self.module("HistogramCreator").makeHistograms(
                self.module("ResponseMatrixY").getRecoUnfoldingVariable(),
                self.module("Utils").getCategoryCutStr(2,1),
                self.module("ResponseMatrixY").getRecoBinning(),
                pseudo=False
            )
        self.module("HistogramCreator").saveHistograms(histogramsY,"reco_top_y")
        self._logger.info("scaling reco top y")
        self.module("HistogramCreator").scaleHistogramsToMultiFitResult(histogramsY,multiFitResultsY)
        #self.module("HistogramCreator").scaleHistogramsToFitResult(histogramsY,fitResultInc)
        self.module("HistogramCreator").saveHistograms(histogramsY,"reco_top_y_scaled")

        
        '''
        histogramsY_BDT = self.module("HistogramCreator").loadHistograms("reco_top_y_bdt")
        if not histogramsY_BDT:
            histogramsY_BDT = self.module("HistogramCreator").makeHistograms(
                self.module("ResponseMatrixY").getRecoUnfoldingVariable(),
                self.module("Utils").getCategoryCutStr(2,1)+"*"+self.module("Utils").getMTWCutStr()+"*"+self.module("Utils").getBDTCutStr(),
                self.module("ResponseMatrixY").getRecoBinning(),
                pseudo=False
            )
        self.module("HistogramCreator").saveHistograms(histogramsY_BDT,"reco_top_y_bdt")
        self.module("HistogramCreator").scaleHistogramsToMultiFitResult(histogramsY_BDT,multiFitResultsY)
        #self.module("HistogramCreator").scaleHistogramsToFitResult(histogramsY,fitResultInc)
        self.module("HistogramCreator").saveHistograms(histogramsY_BDT,"reco_top_y_bdt_scaled")
        '''
        self.module("Drawing").plotHistograms(histogramsY,"top quark |y|","reco_top_Y")
        #self.module("Drawing").plotHistograms(histogramsY_BDT,"top quark |y|","reco_top_Y_bdt")
        
       
        
        ### RESPONSEMATRIX
        genBinningPt = self.module("ResponseMatrixPt").getGenBinning()
        genBinningY = self.module("ResponseMatrixY").getGenBinning()
        
        
        
        responseMatrixPt = self.module("ResponseMatrixPt").loadResponseMatrix("response_pt")
        if responseMatrixPt==None:
            responseMatrixPt = self.module("ResponseMatrixPt").getResponseMatrix(
                self.module("Utils").getCategoryCutStr(2,1)
            )
            self.module("ResponseMatrixPt").saveResponseMatrix(responseMatrixPt,"response_pt")
        self.module("Drawing").drawPStest(responseMatrixPt,genBinningPt,"top quark pT","responsePt")
        self.module("Drawing").drawResponseMatrix(responseMatrixPt,"top quark pT","responsePt")
        '''
        responseMatrixPt_BDT = self.module("ResponseMatrixPt").loadResponseMatrix("response_pt_bdt")
        if responseMatrixPt_BDT==None:
            responseMatrixPt_BDT = self.module("ResponseMatrixPt").getResponseMatrix(
                self.module("Utils").getCategoryCutStr(2,1)+"*"+self.module("Utils").getMTWCutStr()+"*"+self.module("Utils").getBDTCutStr(),
            )
            self.module("ResponseMatrixPt").saveResponseMatrix(responseMatrixPt_BDT,"response_pt_bdt")
        self.module("Drawing").drawPStest(responseMatrixPt_BDT,genBinningPt,"top quark pT","responsePt_bdt")
        self.module("Drawing").drawResponseMatrix(responseMatrixPt_BDT,"top quark pT","responsePt_bdt")
       
        self.module("Drawing").drawBiasTest(responseMatrixPt.ProjectionX(),responseMatrixPt_BDT.ProjectionX(),"top quark pT","matching_pt_responsematrices")
        '''
        responseMatrixY = self.module("ResponseMatrixY").loadResponseMatrix("response_y")
        if responseMatrixY==None:
            responseMatrixY = self.module("ResponseMatrixY").getResponseMatrix(
                self.module("Utils").getCategoryCutStr(2,1)
            )
            self.module("ResponseMatrixY").saveResponseMatrix(responseMatrixY,"response_y")
        self.module("Drawing").drawPStest(responseMatrixY,genBinningY,"top quark |y|","responseY")
        self.module("Drawing").drawResponseMatrix(responseMatrixY,"top quark |y|","responseY")
        '''
        responseMatrixY_BDT = self.module("ResponseMatrixY").loadResponseMatrix("response_y_bdt")
        if responseMatrixY_BDT==None:
            responseMatrixY_BDT = self.module("ResponseMatrixY").getResponseMatrix(
                self.module("Utils").getCategoryCutStr(2,1)+"*"+self.module("Utils").getMTWCutStr()+"*"+self.module("Utils").getBDTCutStr(),
            )
            self.module("ResponseMatrixY").saveResponseMatrix(responseMatrixY_BDT,"response_y_bdt")
        self.module("Drawing").drawPStest(responseMatrixY_BDT,genBinningY,"top quark |y|","responseY_bdt")
        self.module("Drawing").drawResponseMatrix(responseMatrixY_BDT,"top quark |y|","responseY_bdt")

        self.module("Drawing").drawBiasTest(responseMatrixY.ProjectionX(),responseMatrixY_BDT.ProjectionX(),"top quark |y|","matching_y_responsematrices")
        '''

        dataHistPt = histogramsPt["data"]["hists"]["data"]
        #responseMatrixPt = responseMatrixPt_BDT
        #histogramsPt = histogramsPt_BDT

        
        dataHistY = histogramsY["data"]["hists"]["data"]
        #responseMatrixY = responseMatrixY_BDT
        #histogramsY = histogramsY_BDT

        
        
        ### BACKGROUND SUBTRACTION
        genHistPt = responseMatrixPt.ProjectionX()
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

            
        

        
        dataHistPtSubtracted = histogramsPt["tChannel"]["hists"]["sum"]
        dataHistYSubtracted = histogramsY["tChannel"]["hists"]["sum"]
        
        self._logger.debug("pt hist for unfolding uncertainties")
        for ibin in range(dataHistPtSubtracted.GetNbinsX()):
            self._logger.debug("\t bin %2i: %6.1f +- %4.2f%%" % (ibin+1,dataHistPtSubtracted.GetBinContent(ibin+1),100.*dataHistPtSubtracted.GetBinError(ibin+1)/dataHistPtSubtracted.GetBinContent(ibin+1)))
        self._logger.debug("|y| hist for unfolding uncertainties")
        for ibin in range(dataHistYSubtracted.GetNbinsX()):
            self._logger.debug("\t bin %2i: %6.1f +- %4.2f%%" % (ibin+1,dataHistYSubtracted.GetBinContent(ibin+1),100.*dataHistYSubtracted.GetBinError(ibin+1)/dataHistYSubtracted.GetBinContent(ibin+1)))


            
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

        self.module("Drawing").drawBiasTest(trueUnfoldedHistPt,genHistPt,"top quark pT","unfolded_true_Pt")
        self.module("Drawing").drawBiasTest(unfoldedHistPt,genHistPt,"top quark pT","unfolded_data_Pt")
        
        self.module("Utils").normalizeByBinWidth(unfoldedHistPt)
        self.module("Utils").normalizeByBinWidth(trueUnfoldedHistPt)
        self.module("Utils").normalizeByBinWidth(genHistPt)
        self.module("Drawing").drawBiasTest(trueUnfoldedHistPt,genHistPt,"top quark pT","unfolded_true_Pt_norm")
        self.module("Drawing").drawBiasTest(unfoldedHistPt,genHistPt,"top quark pT","unfolded_data_Pt_norm")
        

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
        
        self.module("Drawing").drawBiasTest(trueUnfoldedHistY,genHistY,"top quark |y|","unfolded_true_Y")
        self.module("Drawing").drawBiasTest(unfoldedHistY,genHistY,"top quark |y|","unfolded_data_Y")

        self.module("Utils").normalizeByBinWidth(unfoldedHistY)
        self.module("Utils").normalizeByBinWidth(trueUnfoldedHistY)
        self.module("Utils").normalizeByBinWidth(genHistY)
        self.module("Drawing").drawBiasTest(trueUnfoldedHistY,genHistY,"top quark |y|","unfolded_true_Y_norm")
        self.module("Drawing").drawBiasTest(unfoldedHistY,genHistY,"top quark |y|","unfolded_data_Y_norm")


        self._logger.debug("pt unfolded uncertainties")
        for ibin in range(unfoldedHistPt.GetNbinsX()):
            self._logger.debug("\t bin %2i: %6.1f +- %4.2f%%" % (ibin+1,unfoldedHistPt.GetBinContent(ibin+1),100.*unfoldedHistPt.GetBinError(ibin+1)/unfoldedHistPt.GetBinContent(ibin+1)))
        self._logger.debug("|y| unfolded uncertainties")
        for ibin in range(unfoldedHistY.GetNbinsX()):
            self._logger.debug("\t bin %2i: %6.1f +- %4.2f%%" % (ibin+1,unfoldedHistY.GetBinContent(ibin+1),100.*unfoldedHistY.GetBinError(ibin+1)/unfoldedHistY.GetBinContent(ibin+1)))


