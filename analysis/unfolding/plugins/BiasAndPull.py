import ROOT
import math
import numpy
import random
import os

from defaultModules import Module

import logging

class BiasAndPullUtils(Module.getClass("Utils")):
    def __init__(self,options=[]):
        BiasAndPullUtils.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getOutputFolder(self):
        return os.path.join(os.getcwd(),"result/BiasAndPull")
       
class BiasAndPull(Module.getClass("Program")):
    def __init__(self,options=[]):
        BiasAndPull.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def execute(self):
        #self.module("HistogramsForFitting").buildFitModel()
        
        self.module("Utils").createOutputFolder()
        
        ROOT.gRandom.SetSeed(123)
        
        
        ### RECO HIST AND SCALING
        
        histogramsPt_BDT = self.module("HistogramCreator").loadHistograms("reco_top_pt_bdt")
        if not histogramsPt_BDT:
            histogramsPt_BDT = self.module("HistogramCreator").makeHistograms(
                self.module("ResponseMatrixPt").getRecoUnfoldingVariable(),
                self.module("Utils").getCategoryCutStr(2,1)+"*"+self.module("Utils").getMTWCutStr()+"*"+self.module("Utils").getBDTCutStr(),
                self.module("ResponseMatrixPt").getRecoBinning(),
                pseudo=True
            )
        self.module("HistogramCreator").saveHistograms(histogramsPt_BDT,"reco_top_pt_bdt")
        self.module("Drawing").plotHistograms(histogramsPt_BDT,"top quark pT","reco_top_Pt_bdt")
        
        
        
        histogramsY_BDT = self.module("HistogramCreator").loadHistograms("reco_top_y_bdt")
        if not histogramsY_BDT:
            histogramsY_BDT = self.module("HistogramCreator").makeHistograms(
                self.module("ResponseMatrixY").getRecoUnfoldingVariable(),
                self.module("Utils").getCategoryCutStr(2,1)+"*"+self.module("Utils").getMTWCutStr()+"*"+self.module("Utils").getBDTCutStr(),
                self.module("ResponseMatrixY").getRecoBinning(),
                pseudo=True
            )
        self.module("HistogramCreator").saveHistograms(histogramsY_BDT,"reco_top_y_bdt")
        self.module("Drawing").plotHistograms(histogramsY_BDT,"top quark |y|","reco_top_Y_bdt")
        
       
        
        ### RESPONSEMATRIX
        genBinningPt = self.module("ResponseMatrixPt").getGenBinning()
        genBinningY = self.module("ResponseMatrixY").getGenBinning()
        

        
        responseMatrixPt_BDT = self.module("ResponseMatrixPt").loadResponseMatrix("response_pt_bdt")
        if responseMatrixPt_BDT==None:
            responseMatrixPt_BDT = self.module("ResponseMatrixPt").getResponseMatrix(
                self.module("Utils").getCategoryCutStr(2,1)+"*"+self.module("Utils").getMTWCutStr()+"*"+self.module("Utils").getBDTCutStr(),
            )
            self.module("ResponseMatrixPt").saveResponseMatrix(responseMatrixPt_BDT,"response_pt_bdt")
        self.module("Drawing").drawPStest(responseMatrixPt_BDT,genBinningPt,"top quark pT","responsePt_bdt")
        self.module("Drawing").drawResponseMatrix(responseMatrixPt_BDT,"top quark pT","responsePt_bdt")
              
        responseMatrixY_BDT = self.module("ResponseMatrixY").loadResponseMatrix("response_y_bdt")
        if responseMatrixY_BDT==None:
            responseMatrixY_BDT = self.module("ResponseMatrixY").getResponseMatrix(
                self.module("Utils").getCategoryCutStr(2,1)+"*"+self.module("Utils").getMTWCutStr()+"*"+self.module("Utils").getBDTCutStr(),
            )
            self.module("ResponseMatrixY").saveResponseMatrix(responseMatrixY_BDT,"response_y_bdt")
        self.module("Drawing").drawPStest(responseMatrixY_BDT,genBinningY,"top quark |y|","responseY_bdt")
        self.module("Drawing").drawResponseMatrix(responseMatrixY_BDT,"top quark |y|","responseY_bdt")


        dataHistPt = histogramsPt_BDT["data"]["hists"]["data"]
        responseMatrixPt = responseMatrixPt_BDT
        histogramsPt = histogramsPt_BDT

        
        dataHistY = histogramsY_BDT["data"]["hists"]["data"]
        responseMatrixY = responseMatrixY_BDT
        histogramsY = histogramsY_BDT

        
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

        

          
            
        ### UNFOLDING        
        dataHistPtMatrix=responseMatrixPt.ProjectionY()
        
        for ibin in range(dataHistPtMatrix.GetNbinsX()):
            dataHistPtMatrix.SetBinContent(ibin+1,
                #math.sqrt(dataHistPtMatrix.GetBinContent(ibin+1))
                dataHistPtMatrix.GetBinContent(ibin+1)+ROOT.gRandom.Gaus(0,0.005*dataHistPtSubtracted.GetBinError(ibin+1))
            )
            dataHistPtMatrix.SetBinError(ibin+1,
                #math.sqrt(dataHistPtMatrix.GetBinContent(ibin+1))
                dataHistPtSubtracted.GetBinError(ibin+1)
            )
        #dataHistPtMatrix = dataHistPtSubtracted
        
        self.module("Drawing").drawDataSubtracted(dataHistPtSubtracted,histogramsPt["tChannel"]["hists"]["sum"],"top quark pT","datasubtracted_data_Pt")
        
        unfoldedHistPt, covariancePt, bestTau = self.module("Unfolding").unfold(responseMatrixPt,dataHistPtMatrix,genBinningPt,scan="scanPt")
        self.module("Drawing").drawBiasTest(unfoldedHistPt,genHistPt,"top quark pT","bias_Pt")
        
        
        #bestTau*=0.1
        
        dataHistPtMatrixToy = dataHistPtMatrix.Clone(dataHistPtMatrix.GetName()+"toy")
        NTOYS = 5000
        toyResult = numpy.zeros((NTOYS,dataHistPtMatrixToy.GetNbinsX()))
        for itoy in range(NTOYS):
            print "running toys ",itoy+1,"/",NTOYS
            for ibin in range(dataHistPtMatrix.GetNbinsX()):
                dataHistPtMatrixToy.SetBinContent(ibin+1,
                    dataHistPtMatrix.GetBinContent(ibin+1)+ROOT.gRandom.Gaus(0.0,dataHistPtMatrix.GetBinError(ibin+1))
                )
            unfoldedHistPtToy, covariancePtToy,bestTau = self.module("Unfolding").unfold(responseMatrixPt,dataHistPtMatrixToy,genBinningPt,fixedTau=bestTau)
            for ibin in range(unfoldedHistPtToy.GetNbinsX()):
                toyResult[itoy][ibin]=unfoldedHistPtToy.GetBinContent(ibin+1)
                

        
        for ibin in range(unfoldedHistPtToy.GetNbinsX()):
            hist = ROOT.TH1F("histPull"+str(random.random()),"",50,-5,5)
            hist.Sumw2()
            for itoy in range(NTOYS):
                hist.Fill((toyResult[itoy][ibin]-genHistPt.GetBinContent(ibin+1))/unfoldedHistPt.GetBinError(ibin+1))
            cv = ROOT.TCanvas("cvPull"+str(random.random()),"",800,700)
            
            hist.Scale(1.0/hist.Integral())
            hist.SetMarkerStyle(20)
            hist.SetMarkerSize(1.0)
            
            axis = ROOT.TH2F("axis"+str(random.random()),";(diced - nominal)/#sigma;normalized toys",50,hist.GetXaxis().GetXmin(),hist.GetXaxis().GetXmax(),50,0.0,1.2*hist.GetMaximum())
            axis.Draw("AXIS")
            
            tf = ROOT.TF1("tf"+str(random.random()),"[0]*TMath::Gaus(x,[1],[2])",-10,10)
            tf.SetParameter(0,0.5)
            tf.SetParameter(1,0.0)
            tf.SetParLimits(1,-0.5,0.5)
            tf.SetParameter(2,1.0)
            tf.SetParLimits(2,0.01,10.0)
            hist.Fit(tf,"WLQN",)
            tf.SetLineColor(ROOT.kAzure-4)
            tf.SetLineWidth(4)
            tf.Draw("SameL")
            
            hist.Draw("SamePE")
            #print "rms ",tf.GetParameter(2)
            
            pRes = ROOT.TPaveText(0.59,0.89,0.88,0.78,"NDC")
            pRes.SetFillColor(ROOT.kWhite)
            pRes.SetTextFont(43)
            pRes.SetTextAlign(12)
            pRes.SetTextSize(30)
            pRes.SetLineWidth(0)
            pRes.AddText("Bias: %+4.3f#pm%4.3f"%(tf.GetParameter(1),tf.GetParError(1)))
            pRes.AddText("Pull: %+4.3f#pm%4.3f"%(tf.GetParameter(2),tf.GetParError(2)))
            pRes.Draw("Same")
            
            pCMS=ROOT.TPaveText(1-cv.GetRightMargin()-0.25,0.94,1-cv.GetRightMargin()-0.25,0.94,"NDC")
            pCMS.SetFillColor(ROOT.kWhite)
            pCMS.SetBorderSize(0)
            pCMS.SetTextFont(63)
            pCMS.SetTextSize(30)
            pCMS.SetTextAlign(11)
            pCMS.AddText("CMS")
            pCMS.Draw("Same")
            
            pPreliminary=ROOT.TPaveText(1-cv.GetRightMargin()-0.165,0.94,1-cv.GetRightMargin()-0.165,0.94,"NDC")
            pPreliminary.SetFillColor(ROOT.kWhite)
            pPreliminary.SetBorderSize(0)
            pPreliminary.SetTextFont(53)
            pPreliminary.SetTextSize(30)
            pPreliminary.SetTextAlign(11)
            pPreliminary.AddText("Simulation")
            pPreliminary.Draw("Same")
            
            
            pBin=ROOT.TPaveText(cv.GetLeftMargin()+0.07,0.94,1-cv.GetLeftMargin()+0.07,0.94,"NDC")
            pBin.SetFillColor(ROOT.kWhite)
            pBin.SetBorderSize(0)
            pBin.SetTextFont(43)
            pBin.SetTextSize(30)
            pBin.SetTextAlign(11)
            pBin.AddText("pT bin "+str(ibin+1))
            pBin.Draw("Same")
                
            
            cv.Update()
            cv.Print(os.path.join(self.module("Utils").getOutputFolder(),"pull_pt_bin"+str(ibin+1)+".pdf"))
            cv.Print(os.path.join(self.module("Utils").getOutputFolder(),"pull_pt_bin"+str(ibin+1)+".png"))
            #cv.WaitPrimitive()
            
        
        
        
        
        
        
        dataHistYMatrix=responseMatrixY.ProjectionY()
        
        for ibin in range(dataHistYMatrix.GetNbinsX()):
            dataHistYMatrix.SetBinContent(ibin+1,
                #math.sqrt(dataHistPtMatrix.GetBinContent(ibin+1))
                dataHistYMatrix.GetBinContent(ibin+1)+ROOT.gRandom.Gaus(0,0.005*dataHistYSubtracted.GetBinError(ibin+1))
            )
            dataHistYMatrix.SetBinError(ibin+1,
                #math.sqrt(dataHistPtMatrix.GetBinContent(ibin+1))
                dataHistYSubtracted.GetBinError(ibin+1)
            )
        #dataHistYMatrix = dataHistYSubtracted
        
        self.module("Drawing").drawDataSubtracted(dataHistYSubtracted,histogramsY["tChannel"]["hists"]["sum"],"top quark |y|","datasubtracted_data_Y")
        
        unfoldedHistY, covarianceY, bestTau = self.module("Unfolding").unfold(responseMatrixY,dataHistYMatrix,genBinningY,scan="scanY")
        self.module("Drawing").drawBiasTest(unfoldedHistY,genHistY,"top quark |y|","bias_Y")
        
        #bestTau*=0.1
        
        dataHistYMatrixToy = dataHistYMatrix.Clone(dataHistYMatrix.GetName()+"toy")
        NTOYS = 5000
        toyResult = numpy.zeros((NTOYS,dataHistYMatrixToy.GetNbinsX()))
        for itoy in range(NTOYS):
            print "running toys ",itoy+1,"/",NTOYS
            for ibin in range(dataHistYMatrix.GetNbinsX()):
                dataHistYMatrixToy.SetBinContent(ibin+1,
                    dataHistYMatrix.GetBinContent(ibin+1)+ROOT.gRandom.Gaus(0.0,dataHistYMatrix.GetBinError(ibin+1))
                )
            unfoldedHistYToy, covarianceYToy,bestTau = self.module("Unfolding").unfold(responseMatrixY,dataHistYMatrixToy,genBinningY,fixedTau=bestTau)
            for ibin in range(unfoldedHistYToy.GetNbinsX()):
                toyResult[itoy][ibin]=unfoldedHistYToy.GetBinContent(ibin+1)
                

        
        for ibin in range(unfoldedHistYToy.GetNbinsX()):
            hist = ROOT.TH1F("histPull"+str(random.random()),"",50,-5,5)
            hist.Sumw2()
            for itoy in range(NTOYS):
                hist.Fill((toyResult[itoy][ibin]-genHistY.GetBinContent(ibin+1))/unfoldedHistY.GetBinError(ibin+1))
            cv = ROOT.TCanvas("cvPull"+str(random.random()),"",800,700)
            
            hist.Scale(1.0/hist.Integral())
            hist.SetMarkerStyle(20)
            hist.SetMarkerSize(1.0)
            
            axis = ROOT.TH2F("axis"+str(random.random()),";(diced - nominal)/#sigma;normalized toys",50,hist.GetXaxis().GetXmin(),hist.GetXaxis().GetXmax(),50,0.0,1.2*hist.GetMaximum())
            axis.Draw("AXIS")
            
            tf = ROOT.TF1("tf"+str(random.random()),"[0]*TMath::Gaus(x,[1],[2])",-10,10)
            tf.SetParameter(0,0.5)
            tf.SetParameter(1,0.0)
            tf.SetParLimits(1,-0.5,0.5)
            tf.SetParameter(2,1.0)
            tf.SetParLimits(2,0.01,10.0)
            hist.Fit(tf,"WLQN",)
            tf.SetLineColor(ROOT.kAzure-4)
            tf.SetLineWidth(4)
            tf.Draw("SameL")
            
            hist.Draw("SamePE")
            #print "rms ",tf.GetParameter(2)
            
            pRes = ROOT.TPaveText(0.59,0.89,0.88,0.78,"NDC")
            pRes.SetFillColor(ROOT.kWhite)
            pRes.SetTextFont(43)
            pRes.SetTextAlign(12)
            pRes.SetTextSize(30)
            pRes.SetLineWidth(0)
            pRes.AddText("Bias: %+4.3f#pm%4.3f"%(tf.GetParameter(1),tf.GetParError(1)))
            pRes.AddText("Pull: %+4.3f#pm%4.3f"%(tf.GetParameter(2),tf.GetParError(2)))
            pRes.Draw("Same")
            
            
            pCMS=ROOT.TPaveText(1-cv.GetRightMargin()-0.25,0.94,1-cv.GetRightMargin()-0.25,0.94,"NDC")
            pCMS.SetFillColor(ROOT.kWhite)
            pCMS.SetBorderSize(0)
            pCMS.SetTextFont(63)
            pCMS.SetTextSize(30)
            pCMS.SetTextAlign(11)
            pCMS.AddText("CMS")
            pCMS.Draw("Same")
            
            pPreliminary=ROOT.TPaveText(1-cv.GetRightMargin()-0.165,0.94,1-cv.GetRightMargin()-0.165,0.94,"NDC")
            pPreliminary.SetFillColor(ROOT.kWhite)
            pPreliminary.SetBorderSize(0)
            pPreliminary.SetTextFont(53)
            pPreliminary.SetTextSize(30)
            pPreliminary.SetTextAlign(11)
            pPreliminary.AddText("Simulation")
            pPreliminary.Draw("Same")
            
            
            pBin=ROOT.TPaveText(cv.GetLeftMargin()+0.07,0.94,1-cv.GetLeftMargin()+0.07,0.94,"NDC")
            pBin.SetFillColor(ROOT.kWhite)
            pBin.SetBorderSize(0)
            pBin.SetTextFont(43)
            pBin.SetTextSize(30)
            pBin.SetTextAlign(11)
            pBin.AddText("|y| bin "+str(ibin+1))
            pBin.Draw("Same")
            
            cv.Update()
            cv.Print(os.path.join(self.module("Utils").getOutputFolder(),"pull_y_bin"+str(ibin+1)+".pdf"))
            cv.Print(os.path.join(self.module("Utils").getOutputFolder(),"pull_y_bin"+str(ibin+1)+".png"))
        
        
        
        
        
        self.module("Utils").normalizeByBinWidth(unfoldedHistPt)
        self.module("Utils").normalizeByBinWidth(genHistPt)
        self.module("Drawing").drawBiasTest(unfoldedHistPt,genHistPt,"top quark pT","bias_Pt_norm")
        
        
        self.module("Utils").normalizeByBinWidth(unfoldedHistY)
        self.module("Utils").normalizeByBinWidth(genHistY)
        self.module("Drawing").drawBiasTest(unfoldedHistY,genHistY,"top quark |y|","bias_Y_norm")
