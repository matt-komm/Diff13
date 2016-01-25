import logging
import ROOT
import os
from Module import Module

class Drawing(Module):
    def __init__(self,options=[]):
        Module.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def drawResponseMatrix(self,responseMatrix, varTitle, outputName):
        cvResponse = ROOT.TCanvas("cvResponse","",800,700)
        cvResponse.SetRightMargin(0.19)
        responseMatrixNorm = self.module("Utils").normalizeByTransistionProbability(responseMatrix)
        responseMatrixNorm.GetXaxis().SetTitle("parton-level "+varTitle)
        responseMatrixNorm.GetYaxis().SetTitle("reconstructed "+varTitle)
        responseMatrixNorm.GetZaxis().SetTitle("P(parton-l.#rightarrow reco.)")
        responseMatrixNorm.SetMarkerSize(1.2)
        #responseMatrixNorm.SetMarkerColor(ROOT.kWhite)
        responseMatrixNorm.Draw("colz text")
        
        pCMS=ROOT.TPaveText(1-cvResponse.GetRightMargin()-0.25,0.94,1-cvResponse.GetRightMargin()-0.25,0.94,"NDC")
        pCMS.SetFillColor(ROOT.kWhite)
        pCMS.SetBorderSize(0)
        pCMS.SetTextFont(63)
        pCMS.SetTextSize(30)
        pCMS.SetTextAlign(11)
        pCMS.AddText("CMS")
        pCMS.Draw("Same")
        
        pPreliminary=ROOT.TPaveText(1-cvResponse.GetRightMargin()-0.165,0.94,1-cvResponse.GetRightMargin()-0.165,0.94,"NDC")
        pPreliminary.SetFillColor(ROOT.kWhite)
        pPreliminary.SetBorderSize(0)
        pPreliminary.SetTextFont(53)
        pPreliminary.SetTextSize(30)
        pPreliminary.SetTextAlign(11)
        pPreliminary.AddText("Simulation")
        pPreliminary.Draw("Same")
        
        cvResponse.Update()
        cvResponse.Print(outputName+".png")
        cvResponse.Print(outputName+".pdf")
        
        
    def drawBiasTest(self,unfoldedHist,genHist,varTitle,outputName):
        cvUnfold = ROOT.TCanvas("cvUnfold","",800,700)

        axisUnfolding = ROOT.TH2F("axisUnfolding",";unfolded "+varTitle+";a.u.",50,genHist.GetXaxis().GetXmin(),genHist.GetXaxis().GetXmax(),50,0,1.2*max(unfoldedHist.GetMaximum(),genHist.GetMaximum()))
        axisUnfolding.Draw("AXIS")
        genHist.SetLineColor(ROOT.kAzure-4)
        genHist.SetLineWidth(2)
        genHist.Draw("HISTSame")
        unfoldedHist.SetMarkerSize(1.0)
        unfoldedHist.SetMarkerStyle(20)
        unfoldedHist.SetLineWidth(2)
        unfoldedHist.Draw("SamePE")
        
        pCMS=ROOT.TPaveText(1-cvUnfold.GetRightMargin()-0.25,0.94,1-cvUnfold.GetRightMargin()-0.25,0.94,"NDC")
        pCMS.SetFillColor(ROOT.kWhite)
        pCMS.SetBorderSize(0)
        pCMS.SetTextFont(63)
        pCMS.SetTextSize(30)
        pCMS.SetTextAlign(11)
        pCMS.AddText("CMS")
        pCMS.Draw("Same")
        
        pPreliminary=ROOT.TPaveText(1-cvUnfold.GetRightMargin()-0.165,0.94,1-cvUnfold.GetRightMargin()-0.165,0.94,"NDC")
        pPreliminary.SetFillColor(ROOT.kWhite)
        pPreliminary.SetBorderSize(0)
        pPreliminary.SetTextFont(53)
        pPreliminary.SetTextSize(30)
        pPreliminary.SetTextAlign(11)
        pPreliminary.AddText("Simulation")
        pPreliminary.Draw("Same")
        
        cvUnfold.Print(outputName+".png")
        cvUnfold.Print(outputName+".pdf")
        
    def drawFitCorrelation(self,covariance):
        cvCovariance = ROOT.TCanvas("cvCorrelations","",800,700)
        cvCovariance.SetRightMargin(0.17)
        cvCovariance.SetLeftMargin(0.165)
        covariance.SetMarkerSize(1.2)
        
        covariance.GetXaxis().SetTitleOffset(1.3)
        covariance.GetZaxis().SetTitle("correlation")
        covariance.Draw("colz text")
        
        pCMS=ROOT.TPaveText(1-cvCovariance.GetRightMargin()-0.25,0.94,1-cvCovariance.GetRightMargin()-0.25,0.94,"NDC")
        pCMS.SetFillColor(ROOT.kWhite)
        pCMS.SetBorderSize(0)
        pCMS.SetTextFont(63)
        pCMS.SetTextSize(30)
        pCMS.SetTextAlign(11)
        pCMS.AddText("CMS")
        pCMS.Draw("Same")
        
        pPreliminary=ROOT.TPaveText(1-cvCovariance.GetRightMargin()-0.165,0.94,1-cvCovariance.GetRightMargin()-0.165,0.94,"NDC")
        pPreliminary.SetFillColor(ROOT.kWhite)
        pPreliminary.SetBorderSize(0)
        pPreliminary.SetTextFont(53)
        pPreliminary.SetTextSize(30)
        pPreliminary.SetTextAlign(11)
        pPreliminary.AddText("Preliminary")
        pPreliminary.Draw("Same")
        
        cvCovariance.Print(os.path.join(self.module("Utils").getOutputFolder(),"fitCorrelations.pdf"))
        cvCovariance.Print(os.path.join(self.module("Utils").getOutputFolder(),"fitCorrelations.png"))
        
        cvCovariance.Update()
        cvCovariance.WaitPrimitive()
        

