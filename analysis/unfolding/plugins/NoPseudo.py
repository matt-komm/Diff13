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
            self.module("ThetaModel").makeModel(pseudo=False,addCut="1")
            self.module("ThetaFit").run()
            fitResult = self.module("ThetaFit").readFitResult()
            self.module("Drawing").drawFitCorrelation(fitResult["correlations"])
               
        ###YIELDS
        histogramsAllYield = self.module("HistogramCreator").loadHistograms("yields_all")
        if not histogramsAllYield:
            histogramsAllYield = self.module("HistogramCreator").makeHistograms(
                "Reconstructed_1__nSelectedJet*3+Reconstructed_1__nSelectedBJet",
                "(Reconstructed_1__nSelectedJet<4)",
                numpy.linspace(-0.5,12.5,14),
                pseudo=False
            )
            self.module("HistogramCreator").scaleHistogramsToFitResult(histogramsAllYield,fitResult)
            self.module("HistogramCreator").saveHistograms(histogramsAllYield,"yields_all")
        histogramsMTWYield = self.module("HistogramCreator").loadHistograms("yields_mtw")
        if not histogramsMTWYield:
            histogramsMTWYield = self.module("HistogramCreator").makeHistograms(
                "Reconstructed_1__nSelectedJet*3+Reconstructed_1__nSelectedBJet",
                "(Reconstructed_1__nSelectedJet<4)"+"*"+self.module("Utils").getMTWCutStr(),
                numpy.linspace(-0.5,12.5,14),
                pseudo=False
            )
            self.module("HistogramCreator").scaleHistogramsToFitResult(histogramsMTWYield,fitResult)
            self.module("HistogramCreator").saveHistograms(histogramsMTWYield,"yields_mtw")
        histogramsBDTYield = self.module("HistogramCreator").loadHistograms("yields_bdt")
        if not histogramsBDTYield:
            histogramsBDTYield = self.module("HistogramCreator").makeHistograms(
                "Reconstructed_1__nSelectedJet*3+Reconstructed_1__nSelectedBJet",
                "(Reconstructed_1__nSelectedJet<4)"+"*"+self.module("Utils").getMTWCutStr()+"*"+self.module("Utils").getBDTCutStr(),
                numpy.linspace(-0.5,12.5,14),
                pseudo=False
            )
            self.module("HistogramCreator").scaleHistogramsToFitResult(histogramsBDTYield,fitResult)
            self.module("HistogramCreator").saveHistograms(histogramsBDTYield,"yields_bdt")
        
        
