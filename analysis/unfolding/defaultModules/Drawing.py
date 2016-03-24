import logging
import ROOT
import os
import random
import math
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
        
        cvResponse.Print(os.path.join(self.module("Utils").getOutputFolder(),outputName+".png"))
        cvResponse.Print(os.path.join(self.module("Utils").getOutputFolder(),outputName+".pdf"))
        
        
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
        
        cvUnfold.Print(os.path.join(self.module("Utils").getOutputFolder(),outputName+".png"))
        cvUnfold.Print(os.path.join(self.module("Utils").getOutputFolder(),outputName+".pdf"))
        
        
    def drawDataSubtracted(self,dataHist,recoHist,varTitle,outputName):
        cvData = ROOT.TCanvas("cvData","",800,700)

        axisUnfolding = ROOT.TH2F("axisUnfolding",";reconstructed"+varTitle+";a.u.",50,recoHist.GetXaxis().GetXmin(),recoHist.GetXaxis().GetXmax(),50,0,1.2*max(recoHist.GetMaximum(),dataHist.GetMaximum()))
        axisUnfolding.Draw("AXIS")
        recoHist.SetLineColor(ROOT.kAzure-4)
        recoHist.SetLineWidth(2)
        recoHist.Draw("HISTSame")
        dataHist.SetMarkerSize(1.0)
        dataHist.SetMarkerStyle(20)
        dataHist.SetLineWidth(2)
        dataHist.Draw("SamePE")
        
        pCMS=ROOT.TPaveText(1-cvData.GetRightMargin()-0.25,0.94,1-cvData.GetRightMargin()-0.25,0.94,"NDC")
        pCMS.SetFillColor(ROOT.kWhite)
        pCMS.SetBorderSize(0)
        pCMS.SetTextFont(63)
        pCMS.SetTextSize(30)
        pCMS.SetTextAlign(11)
        pCMS.AddText("CMS")
        pCMS.Draw("Same")
        
        pPreliminary=ROOT.TPaveText(1-cvData.GetRightMargin()-0.165,0.94,1-cvData.GetRightMargin()-0.165,0.94,"NDC")
        pPreliminary.SetFillColor(ROOT.kWhite)
        pPreliminary.SetBorderSize(0)
        pPreliminary.SetTextFont(53)
        pPreliminary.SetTextSize(30)
        pPreliminary.SetTextAlign(11)
        pPreliminary.AddText("Simulation")
        pPreliminary.Draw("Same")
        
        cvData.Print(os.path.join(self.module("Utils").getOutputFolder(),outputName+".png"))
        cvData.Print(os.path.join(self.module("Utils").getOutputFolder(),outputName+".pdf"))
        
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
        
    def drawMultiFitResults(self,fitResults,name):
    
        axisRange = {}
        values = {}

        for fitResult in fitResults:
            ranges = fitResult["range"]
            for comp in self.module("ThetaModel").getComponentsDict().keys():
                if not axisRange.has_key(comp):
                    axisRange[comp]={"xmin":1000,"xmax":-1000,"ymin":1000,"ymax":-1000}
                    values[comp]=[]
                mean = fitResult["res"][comp]["mean"]
                unc = fitResult["res"][comp]["unc"]
                
                values[comp].append({"mean":mean,"unc":unc,"range":ranges})
                
                axisRange[comp]["xmin"]=min(axisRange[comp]["xmin"],ranges[0])
                axisRange[comp]["xmax"]=max(axisRange[comp]["xmax"],ranges[1])

                axisRange[comp]["ymin"]=min(axisRange[comp]["ymin"],mean-2*unc)
                axisRange[comp]["ymax"]=max(axisRange[comp]["ymax"],mean+2*unc)
                
                
        for comp in values.keys():
            cvMulti = ROOT.TCanvas("cvMulti"+str(random.random()),"",800,700)
            #cvMulti.SetRightMargin(0.17)
            #cvMulti.SetLeftMargin(0.165)
            axis = ROOT.TH2F("axis"+str(random.random()),";"+name.replace("_"," ")+";"+comp+" nuisance parameter",50,axisRange[comp]["xmin"],axisRange[comp]["xmax"],50,axisRange[comp]["ymin"],axisRange[comp]["ymax"])
            axis.Draw("AXIS")
            
            rootObj=[]
            for value in values[comp]:
                reRange = 0.1*math.fabs(value["range"][0]-value["range"][1])
            
                marker = ROOT.TMarker(
                    0.5*(value["range"][0]+value["range"][1]),value["mean"],
                    20
                )
                rootObj.append(marker)
                marker.SetMarkerColor(ROOT.kBlack)
                marker.SetMarkerSize(1.2)
                marker.Draw("SameP")
            
            
                line = ROOT.TLine(
                    value["range"][0],value["mean"],
                    value["range"][1],value["mean"],
                )
                rootObj.append(line)
                line.SetLineColor(ROOT.kBlack)
                line.SetLineWidth(2)
                line.Draw("SameL")
                
                lineUp = ROOT.TLine(
                    value["range"][0]+reRange,value["mean"]+value["unc"],
                    value["range"][1]-reRange,value["mean"]+value["unc"],
                )
                rootObj.append(lineUp)
                lineUp.SetLineColor(ROOT.kBlack)
                lineUp.SetLineWidth(1)
                lineUp.SetLineStyle(1)
                lineUp.Draw("SameL")
                
                lineDown = ROOT.TLine(
                    value["range"][0]+reRange,value["mean"]-value["unc"],
                    value["range"][1]-reRange,value["mean"]-value["unc"],
                )
                rootObj.append(lineDown)
                lineDown.SetLineColor(ROOT.kBlack)
                lineDown.SetLineWidth(1)
                lineDown.SetLineStyle(1)
                lineDown.Draw("SameL")
                
                lineCenter = ROOT.TLine(
                    0.5*(value["range"][0]+value["range"][1]),value["mean"]-value["unc"],
                    0.5*(value["range"][0]+value["range"][1]),value["mean"]+value["unc"],
                )
                rootObj.append(lineCenter)
                lineCenter.SetLineColor(ROOT.kBlack)
                lineCenter.SetLineWidth(2)
                lineCenter.Draw("SameL")
                
                pCMS=ROOT.TPaveText(1-cvMulti.GetRightMargin()-0.25,0.94,1-cvMulti.GetRightMargin()-0.25,0.94,"NDC")
                pCMS.SetFillColor(ROOT.kWhite)
                pCMS.SetBorderSize(0)
                pCMS.SetTextFont(63)
                pCMS.SetTextSize(30)
                pCMS.SetTextAlign(11)
                pCMS.AddText("CMS")
                pCMS.Draw("Same")
                
                pPreliminary=ROOT.TPaveText(1-cvMulti.GetRightMargin()-0.165,0.94,1-cvMulti.GetRightMargin()-0.165,0.94,"NDC")
                pPreliminary.SetFillColor(ROOT.kWhite)
                pPreliminary.SetBorderSize(0)
                pPreliminary.SetTextFont(53)
                pPreliminary.SetTextSize(30)
                pPreliminary.SetTextAlign(11)
                pPreliminary.AddText("Preliminary")
                pPreliminary.Draw("Same")
                
            cvMulti.Update()
            cvMulti.Print(os.path.join(self.module("Utils").getOutputFolder(),"multi_"+name+"_"+comp+".png"))
            cvMulti.Print(os.path.join(self.module("Utils").getOutputFolder(),"multi_"+name+"_"+comp+".pdf"))

        
    def drawPStest(self,responseMatrix,genBinning,varname,name):
        purityHist = ROOT.TH1F("purity"+str(random.random()),";top quark pT;purity=N(reco. & gen.)/N(reco.)",len(genBinning)-1,genBinning)
        stabilityHist = ROOT.TH1F("stability"+str(random.random()),";top quark pT;stability=N(reco. & gen.)/N(gen.)",len(genBinning)-1,genBinning)
        
        responseMatrixSelectedPStest = responseMatrix.Clone(responseMatrix.GetName()+"RStest")
        responseMatrixSelectedPStest.RebinY(2)
        
        for recoBin in range(responseMatrixSelectedPStest.GetNbinsY()):
            sumGen = 0.0
            for genBin in range(responseMatrixSelectedPStest.GetNbinsX()):
                sumGen+=responseMatrixSelectedPStest.GetBinContent(genBin+1,recoBin+1)
            stability = responseMatrixSelectedPStest.GetBinContent(recoBin+1,recoBin+1)/sumGen
            stabilityHist.SetBinContent(recoBin+1,stability)
        
        for genBin in range(responseMatrixSelectedPStest.GetNbinsX()):
            sumReco = 0.0
            for recoBin in range(responseMatrixSelectedPStest.GetNbinsY()):
                sumReco+=responseMatrixSelectedPStest.GetBinContent(genBin+1,recoBin+1)
            purity = responseMatrixSelectedPStest.GetBinContent(genBin+1,genBin+1)/sumReco
            purityHist.SetBinContent(genBin+1,purity)
        
        axis = ROOT.TH2F("axis"+str(random.random()),";"+varname+";",50,genBinning[0],genBinning[-1],50,0.0,1.0)
        cv = ROOT.TCanvas("cvPS"+str(random.random()),"",800,700)
        axis.Draw("AXIS")
        stabilityHist.SetLineWidth(3)
        stabilityHist.SetLineColor(ROOT.kAzure-5)
        stabilityHist.Draw("SAME")
        purityHist.SetLineWidth(3)
        purityHist.SetLineColor(ROOT.kOrange+8)
        purityHist.Draw("SAME")
        
        legend = ROOT.TLegend(0.35,0.35,0.65,0.24)
        legend.SetBorderSize(0)
        legend.SetTextFont(42)
        legend.SetFillColor(ROOT.kWhite)
        legend.AddEntry(stabilityHist,"stability","L")
        legend.AddEntry(purityHist,"purity","L")
        legend.Draw("Same")
        
        cv.Print(os.path.join(self.module("Utils").getOutputFolder(),"PStest_"+name+".pdf"))
        cv.Print(os.path.join(self.module("Utils").getOutputFolder(),"PStest_"+name+".png"))

    def plotHistogram(self,histogram,title,output):
        
        cvHist = ROOT.TCanvas("cvHist","",800,700)
        histogram.GetXaxis().SetTitle(title)
        histogram.Draw()
        
        pCMS=ROOT.TPaveText(1-cvHist.GetRightMargin()-0.25,0.94,1-cvHist.GetRightMargin()-0.25,0.94,"NDC")
        pCMS.SetFillColor(ROOT.kWhite)
        pCMS.SetBorderSize(0)
        pCMS.SetTextFont(63)
        pCMS.SetTextSize(30)
        pCMS.SetTextAlign(11)
        pCMS.AddText("CMS")
        pCMS.Draw("Same")
        
        pPreliminary=ROOT.TPaveText(1-cvHist.GetRightMargin()-0.165,0.94,1-cvHist.GetRightMargin()-0.165,0.94,"NDC")
        pPreliminary.SetFillColor(ROOT.kWhite)
        pPreliminary.SetBorderSize(0)
        pPreliminary.SetTextFont(53)
        pPreliminary.SetTextSize(30)
        pPreliminary.SetTextAlign(11)
        pPreliminary.AddText("Preliminary")
        pPreliminary.Draw("Same")
        
        cvHist.Print(os.path.join(self.module("Utils").getOutputFolder(),output+".pdf"))
        cvHist.Print(os.path.join(self.module("Utils").getOutputFolder(),output+".png"))
        
        
    def plotHistograms(self,histograms,title,output):
        mcStack = ROOT.THStack()
        mcSumHist = None
        for component in self.module("ThetaModel").getComponentsDict():
            compHist = None
            for sampleName in histograms[component]["hists"].keys():
                if compHist==None:
                    compHist = histograms[component]["hists"][sampleName].Clone(component+str(random.random()))
                else:
                    compHist.Add(histograms[component]["hists"][sampleName])
            mcStack.Add(compHist,"HIST")
            
            if mcSumHist==None:
                mcSumHist=compHist.Clone("mcSumHist")
            else:
                mcSumHist.Add(compHist)
            
        dataHist = histograms["data"]["hists"]["data"]
        
        cvPlot = ROOT.TCanvas("cvPlot","",800,700)
        
        axis = ROOT.TH2F("axis",";title;Events",
            50,dataHist.GetXaxis().GetXmin(),dataHist.GetXaxis().GetXmax(),
            50,0.0,1.2*max(dataHist.GetMaximum(),mcSumHist.GetMaximum())
        )
        axis.Draw("AXIS")
        mcStack.Draw("SameHIST")
        dataHist.Draw("SamePE") 
        
        cvPlot.Update()
        cvPlot.WaitPrimitive()
        
        cvPlot.Print(os.path.join(self.module("Utils").getOutputFolder(),output+".png"))
        cvPlot.Print(os.path.join(self.module("Utils").getOutputFolder(),output+".pdf"))
        

