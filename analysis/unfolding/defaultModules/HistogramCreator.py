from Module import Module

from ModelClasses import *
import pyTool
import random
import logging
import ROOT
import os
import csv

class HistogramCreator(Module):
    def __init__(self,options=[]):
        Module.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def makeHistograms(self, varName, weight, binningScheme, pseudo=False,addUnderOverflows=False):
        self._logger.info("Creating histograms: "+varName)
        
        uncertainties = self.module("ThetaModel").getUncertaintsDict()
        observables = self.module("ThetaModel").getObservablesDict()
        components = self.module("ThetaModel").getComponentsDict()
        
        rootFiles = self.module("Files").getMCFiles()
        rootFiles+=self.module("Files").getDataFiles()

        histograms={}
        
        for componentName in components.keys():
            componentWeight = components[componentName]["weight"]
            componentUncertaintyNames = components[componentName]["uncertainties"]

            componentHist = ROOT.TH1F("hist_"+componentName+"_"+str(random.random()),";"+varName+";Events",len(binningScheme)-1,binningScheme)
            componentHist.SetDirectory(0)
            componentHist.Sumw2()
            componentHist.SetFillColor(components[componentName]["color"])
            componentHist.SetLineColor(components[componentName]["color"])
            
            histograms[componentName]={"hists":{},"unc":componentUncertaintyNames}
            
            for componentSetName in components[componentName]["sets"]:
                if pseudo:
                    sampleDict = self.module("Samples").getSamplePseudo(componentSetName)
                else:
                    sampleDict = self.module("Samples").getSample(componentSetName)
                histograms[componentName]["hists"][componentSetName]=componentHist.Clone(componentHist.GetName()+componentSetName+str(random.random()))
                
                for processName in sampleDict["processes"]:
                    processWeight = sampleDict["weight"]
                    
                    for i,f in enumerate(rootFiles):
                        self.module("Utils").getHist1D(
                            histograms[componentName]["hists"][componentSetName],
                            f,
                            processName,
                            varName,weight+"*"+componentWeight+"*"+processWeight,
                            addUnderOverflows=addUnderOverflows
                        )
                        #break
                        
                self._logger.debug("projected "+componentName+" "+componentSetName+": "+str(round(histograms[componentName]["hists"][componentSetName].Integral(),2)))
        
        if pseudo:
            dataHist = ROOT.TH1F("hist_"+str(random.random()),";"+varName+";Events",len(binningScheme)-1,binningScheme)
            dataHist.SetDirectory(0)
            dataHist.Sumw2()
            dataHist.SetMarkerStyle(20)
            dataHist.SetMarkerSize(0.8)
            histograms["data"]={"hists":{},"unc":[]}
            
            for componentName in components.keys():
                componentWeight = components[componentName]["weight"]
                componentUncertaintyNames = components[componentName]["uncertainties"]

                for componentSetName in components[componentName]["sets"]:
                    sampleDict = self.module("Samples").getSamplePseudo(componentSetName)
                    
                    
                    for processName in sampleDict["processes"]:
                        processWeight = sampleDict["weight"]

                        for i,f in enumerate(rootFiles):
                            self.module("Utils").getHist1D(
                                dataHist,
                                f,
                                processName,
                                varName,
                                weight+"*"+componentWeight+"*"+processWeight,
                                addUnderOverflows=addUnderOverflows
                            )
                            #break
        
            for ibin in range(dataHist.GetNbinsX()):
                '''
                dataHist.SetBinContent(ibin+1,
                    ROOT.gRandom.Poisson(dataHist.GetBinContent(ibin+1))
                )
                '''
                dataHist.SetBinError(ibin+1,
                    math.sqrt(dataHist.GetBinContent(ibin+1))
                )
            histograms["data"]["hists"]["data"]=dataHist
            
        else:
            data = self.module("ThetaModel").getDataDict()
            
            dataHist = ROOT.TH1F("hist_"+str(random.random()),";"+varName+";Events",len(binningScheme)-1,binningScheme)
            dataHist.SetDirectory(0)
            dataHist.Sumw2()
            dataHist.SetMarkerStyle(20)
            dataHist.SetMarkerSize(0.8)
            histograms["data"]={"hists":{}, "unc":[]}
            histograms["data"]["hists"]["data"]=dataHist
            
            for componentName in data.keys():
                componentWeight=data[componentName]["weight"]
                for componentSetName in data[componentName]["sets"]:
                    sampleDict = self.module("Samples").getSample(componentSetName)
                    for processName in sampleDict["processes"]:
                        processWeight = sampleDict["weight"]
                        for i,f in enumerate(rootFiles):
                            self.module("Utils").getHist1D(
                                histograms["data"]["hists"]["data"],
                                f,
                                processName,
                                varName,
                                weight+"*"+componentWeight+"*"+processWeight,
                                addUnderOverflows=addUnderOverflows
                            )
                            #break
                self._logger.debug("projected "+componentName+" data: "+str(round(histograms["data"]["hists"]["data"].Integral(),2)))
            
        return histograms
        
        
    def scaleHistogramsToFitResult(self,histograms,fitResult):
        for component in self.module("ThetaModel").getComponentsDict().keys():
            scaleFactor = 1.0
            for uncName in histograms[component]["unc"]:
                scaleFactor*=fitResult[uncName]["mean"]
            #self._logger.debug("Scale "+component+" to "+str(scaleFactor))
            for sampleName in histograms[component]["hists"].keys():
                histograms[component]["hists"][sampleName].Scale(scaleFactor)
                
    def scaleHistogramsToMultiFitResult(self,histograms,multiFitResults):
        for component in self.module("ThetaModel").getComponentsDict().keys():
            for sampleName in histograms[component]["hists"].keys():
                for ibin in range(histograms[component]["hists"][sampleName].GetNbinsX()):
                    scaleFactor = 1.0
                    scaleFactor_unc2 = 0.0
                    for uncName in histograms[component]["unc"]:
                        scaleFactor *= multiFitResults[ibin]["res"][uncName]["mean"]
                        scaleFactor_unc2 += multiFitResults[ibin]["res"][uncName]["unc"]**2
                    histograms[component]["hists"][sampleName].SetBinContent(
                        ibin+1,
                        scaleFactor*histograms[component]["hists"][sampleName].GetBinContent(ibin+1)
                    )
                    histograms[component]["hists"][sampleName].SetBinError(
                        ibin+1,
                        math.sqrt(scaleFactor_unc2)*histograms[component]["hists"][sampleName].GetBinContent(ibin+1)
                    )
                    self._logger.debug("scaling %s bin %i to %6.3f+-%6.3f"%(sampleName,ibin+1,scaleFactor,math.sqrt(scaleFactor_unc2)))
                
    def saveHistograms(self,histograms,output):
        rootFile = ROOT.TFile(os.path.join(self.module("Utils").getOutputFolder(),output+".root"),"RECREATE")
        for component in histograms.keys():
            for sampleName in histograms[component]["hists"].keys():
                histToWrite = histograms[component]["hists"][sampleName].Clone(component+"__"+sampleName)
                histToWrite.SetDirectory(rootFile)
                histToWrite.Write()
        rootFile.Close()
        
    def loadHistograms(self,output):
        fullPath = os.path.join(self.module("Utils").getOutputFolder(),output+".root")
        if not os.path.exists(fullPath):
            return None
        rootFile = ROOT.TFile(fullPath)
        components = self.module("ThetaModel").getComponentsDict()
        
        histograms={}
        for componentName in components.keys():
            histograms[componentName]={"hists":{},"unc":components[componentName]["uncertainties"]}
            for componentSetName in components[componentName]["sets"]:
                histograms[componentName]["hists"][componentSetName]=rootFile.Get(componentName+"__"+componentSetName).Clone(componentName+"__"+componentSetName+str(random.random()))
                histograms[componentName]["hists"][componentSetName].SetDirectory(0)
        
        histograms["data"]={"hists":{},"unc":[]}
        histograms["data"]["hists"]["data"]=rootFile.Get("data__data").Clone("data__data"+str(random.random()))
        histograms["data"]["hists"]["data"].SetDirectory(0)
        rootFile.Close()
        return histograms
                
