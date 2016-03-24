import ROOT
import math
import numpy
import random
import os

from defaultModules import Module

import logging

class UtilsYieldDown(Module.getClass("Utils")):
    def __init__(self,options=[]):
        UtilsYieldDown.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getOutputFolder(self):
        return os.path.join(os.getcwd(),"result/"+self.getOption("scalefactor")+"YieldDown")
        

class HistogramCreatorYieldDown(Module.getClass("HistogramCreator")):
    def __init__(self,options=[]):
        HistogramCreatorYieldDown.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
        
    def scaleHistogramsToFitResult(self,histograms,fitResult):
        for component in self.module("ThetaModel").getComponentsDict().keys():
            scaleFactor = 1.0
            for uncName in histograms[component]["unc"]:
                if uncName==self.getOption("scalefactor"):
                    scaleFactor*=(fitResult[uncName]["mean"]-fitResult[uncName]["unc"])
                else:
                    scaleFactor*=fitResult[uncName]["mean"]
                
            for sampleName in histograms[component]["hists"].keys():
                histograms[component]["hists"][sampleName].Scale(scaleFactor)
                
    def scaleHistogramsToMultiFitResult(self,histograms,multiFitResults):
        for component in self.module("ThetaModel").getComponentsDict().keys():
            for sampleName in histograms[component]["hists"].keys():
                for ibin in range(histograms[component]["hists"][sampleName].GetNbinsX()):
                    scaleFactor = 1.0
                    scaleFactor_unc2 = 0.0
                    for uncName in histograms[component]["unc"]:
                        if uncName==self.getOption("scalefactor"):
                            scaleFactor *= (multiFitResults[ibin]["res"][uncName]["mean"]-multiFitResults[ibin]["res"][uncName]["unc"])
                        else:
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
