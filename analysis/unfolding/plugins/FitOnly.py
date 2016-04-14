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
            fitResultNominal = self.module("ThetaFit").checkFitResult(
                modelName='fit',
                fileName='fit'
            )
            if fitResultNominal==None:
                self.module("ThetaModel").makeFitHists(
                    pseudo=False, 
                    histFile='fit'
                )
                self.module("ThetaModel").makeModel(
                    pseudo=False, 
                    modelName='fit',
                    histFile='fit',
                    outputFile='fit_diced'
                )
                for i in range(self.module("Utils").getNumberOfPseudoExp()):
                    nll = self.module("ThetaModel").makeMCDicedHistograms("fit","fit_diced")
                    
                    self.module("ThetaFit").run(modelName='fit')
                    try:
                        fitResultNominalNew = self.module("ThetaFit").readFitResult(
                            modelName="fit",
                            fileName="fit_diced"
                        )
                        fitResultNominalNew["nll"]+=nll #need to correct likelihood in case dicing was far away
                        
                    except Exception, e:
                        self._logger.error("theta did not produced a valid fit result: "+str(e))
                        continue
                    if (fitResultNominal==None) or (fitResultNominal["nll"]>fitResultNominalNew["nll"]):
                        fitResultNominal=fitResultNominalNew
                        self.module("ThetaFit").printFitResult(fitResultNominal)
                        os.rename(
                            os.path.join(self.module("Utils").getOutputFolder(),"fit_diced.root"),
                            os.path.join(self.module("Utils").getOutputFolder(),"fit.root")
                        )
                    break
            else:
                self.module("ThetaFit").printFitResult(fitResultNominal)
            
            
        ptRecoBinning = self.module("ResponseMatrixPt").getRecoBinning()
        ptRecoVariable = self.module("ResponseMatrixPt").getRecoUnfoldingVariable()
        for ipt in range(len(ptRecoBinning)-1):
            if self.getOption("fit")=="pt"+str(ipt):
                fitResultByPt = self.module("ThetaFit").checkFitResult(
                    modelName="fit_pt"+str(ipt),
                    fileName="fit_pt"+str(ipt)
                )
                if fitResultByPt==None:
                    self.module("ThetaModel").makeFitHists(
                        pseudo=False, 
                        histFile="fit_pt"+str(ipt),
                        addCut="("+ptRecoVariable+">"+str(ptRecoBinning[ipt])+")*("+ptRecoVariable+"<"+str(ptRecoBinning[ipt+1])+")"
                    )
                    self.module("ThetaModel").makeModel(
                        pseudo=False, 
                        modelName="fit_pt"+str(ipt),
                        histFile="fit_pt"+str(ipt),
                        outputFile="fit_pt"+str(ipt)+'_diced'
                    )
                    for i in range(self.module("Utils").getNumberOfPseudoExp()):
                        nll=self.module("ThetaModel").makeMCDicedHistograms(
                            "fit_pt"+str(ipt),
                            "fit_pt"+str(ipt)+"_diced"
                        )
                        
                        self.module("ThetaFit").run(modelName="fit_pt"+str(ipt))
                        try:
                            fitResultByPtNew = self.module("ThetaFit").readFitResult(
                                modelName="fit_pt"+str(ipt),
                                fileName="fit_pt"+str(ipt)+'_diced',
                            )
                            fitResultByPtNew["nll"]+=nll #need to correct likelihood in case dicing was far away
                            
                        except Exception, e:
                            self._logger.error("theta did not produced a valid fit result: "+str(e))
                            continue
                        if (fitResultByPt==None) or (fitResultByPt["nll"]>fitResultByPtNew["nll"]):
                            fitResultByPt=fitResultByPtNew
                            self.module("ThetaFit").printFitResult(fitResultByPt)
                            os.rename(
                                os.path.join(self.module("Utils").getOutputFolder(),"fit_pt"+str(ipt)+"_diced.root"),
                                os.path.join(self.module("Utils").getOutputFolder(),"fit_pt"+str(ipt)+".root")
                            )
                        break
                else:
                    self.module("ThetaFit").printFitResult(fitResultByPt)

        yRecoBinning = self.module("ResponseMatrixY").getRecoBinning()
        yRecoVariable = self.module("ResponseMatrixY").getRecoUnfoldingVariable()
        for iy in range(len(yRecoBinning)-1):
            if self.getOption("fit")=="y"+str(iy):
                fitResultByY = self.module("ThetaFit").checkFitResult(
                    modelName="fit_y"+str(iy),
                    fileName="fit_y"+str(iy),
                )
                if fitResultByY==None:
                    self.module("ThetaModel").makeFitHists(
                        pseudo=False, 
                        histFile="fit_y"+str(iy),
                        addCut="("+yRecoVariable+">"+str(yRecoBinning[iy])+")*("+yRecoVariable+"<"+str(yRecoBinning[iy+1])+")"
                    )
                    self.module("ThetaModel").makeModel(
                        pseudo=False, 
                        modelName="fit_y"+str(iy),
                        histFile="fit_y"+str(iy),
                        outputFile="fit_y"+str(iy)+'_diced'
                    )
                    for i in range(self.module("Utils").getNumberOfPseudoExp()):
                        nll=self.module("ThetaModel").makeMCDicedHistograms(
                            "fit_y"+str(iy),
                            "fit_y"+str(iy)+"_diced"
                        )
                        
                        self.module("ThetaFit").run(modelName="fit_y"+str(iy))
                        try:
                            fitResultByYNew = self.module("ThetaFit").readFitResult(
                                modelName="fit_y"+str(iy),
                                fileName="fit_y"+str(iy)+'_diced',
                            )
                            fitResultByYNew["nll"]+=nll #need to correct likelihood in case dicing was far away
                        
                        except Exception, e:
                            self._logger.error("theta did not produced a valid fit result: "+str(e))
                            continue
                        if (fitResultByY==None) or (fitResultByY["nll"]>fitResultByYNew["nll"]):
                            fitResultByY=fitResultByYNew
                            self.module("ThetaFit").printFitResult(fitResultByY)
                            os.rename(
                                os.path.join(self.module("Utils").getOutputFolder(),"fit_y"+str(iy)+"_diced.root"),
                                os.path.join(self.module("Utils").getOutputFolder(),"fit_y"+str(iy)+".root")
                            )
                        break
                else:
                    self.module("ThetaFit").printFitResult(fitResultByY)
                    
