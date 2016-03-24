import ROOT
import math
import numpy
import random
import os

from defaultModules import Module

import logging


class ProgramFitOnly(Module.getClass("Program")):
    def __init__(self,options=[]):
        ProgramFitOnly.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def execute(self):
        self.module("Utils").createOutputFolder()
        
        ROOT.gRandom.SetSeed(123)
        
        if self.getOption("fit")=="incl":
            ### FITTING
            fitResultNominal = self.module("ThetaFit").checkFitResult(modelName='fit')
            if fitResultNominal==None:
                self.module("ThetaModel").makeModel(pseudo=False, modelName='fit')
                self.module("ThetaFit").run(modelName='fit')
                fitResultNominal = self.module("ThetaFit").readFitResult()
            
            
        ptRecoBinning = self.module("ResponseMatrixPt").getRecoBinning()
        ptRecoVariable = self.module("ResponseMatrixPt").getRecoUnfoldingVariable()
        for ipt in range(len(ptRecoBinning)-1):
            if self.getOption("fit")=="pt"+str(ipt):
                fitResultByPt = self.module("ThetaFit").checkFitResult(modelName="fit_pt"+str(ipt))
                if fitResultByPt==None:
                    self.module("ThetaModel").makeModel(
                        pseudo=False, 
                        modelName="fit_pt"+str(ipt),
                        addCut="("+ptRecoVariable+">"+str(ptRecoBinning[ipt])+")*("+ptRecoVariable+"<"+str(ptRecoBinning[ipt+1])+")"
                    )
                    self.module("ThetaFit").run(modelName="fit_pt"+str(ipt))
                    fitResultByPt = self.module("ThetaFit").readFitResult(modelName="fit_pt"+str(ipt))
            
            
        yRecoBinning = self.module("ResponseMatrixY").getRecoBinning()
        yRecoVariable = self.module("ResponseMatrixY").getRecoUnfoldingVariable()
        for iy in range(len(yRecoBinning)-1):
            if self.getOption("fit")=="y"+str(iy):
                fitResultByY = self.module("ThetaFit").checkFitResult(modelName="fit_y"+str(iy))
                if fitResultByY==None:
                    self.module("ThetaModel").makeModel(
                        pseudo=False, 
                        modelName="fit_y"+str(iy),
                        addCut="("+yRecoVariable+">"+str(yRecoBinning[iy])+")*("+yRecoVariable+"<"+str(yRecoBinning[iy+1])+")"
                    )
                    self.module("ThetaFit").run(modelName="fit_y"+str(iy))
                    fitResultByY = self.module("ThetaFit").readFitResult(modelName="fit_y"+str(iy))


