import ROOT
import math
import numpy
import random
import os

from defaultModules import Module

import logging

class UtilsFit2j0t(Module.getClass("Utils")):
    def __init__(self,options=[]):
        UtilsFit2j0t.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
    def getOutputFolder(self):
        return os.path.join(os.getcwd(),"result/Fit2j0t_AMC")

class ProgramFit2j0t(Module.getClass("Program")):
    def __init__(self,options=[]):
        ProgramFit2j0t.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def execute(self):
        self.module("Utils").createOutputFolder()
        
        ROOT.gRandom.SetSeed(123)
        
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
        self.module("Drawing").drawFitCorrelation(fitResultNominal["correlations"])
                
class Fit2j0t(Module.getClass("ThetaModel")):
    def __init__(self,options=[]):
        Fit2j0t.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getUncertaintsDict(self):
        uncertainties = {
            "WZjets":self.module("ThetaModel").makeLogNormal(1.0,0.5),
            #"BF":{"type":"gauss","config":{"mean": "1.0", "width":"0.3", "range":"(0.0,\"inf\")"}},
            #"CF":{"type":"gauss","config":{"mean": "1.0", "width":"0.3", "range":"(0.0,\"inf\")"}},
            #"LF":{"type":"gauss","config":{"mean": "1.0", "width":"0.3", "range":"(0.0,\"inf\")"}},
            #"TopBkg":{"type":"gauss","config":{"mean": "1.0", "width":"0.3", "range":"(0.0,\"inf\")"}},
            #"tChannel":{"type":"gauss","config":{"mean": "1.0", "width":"1.0", "range":"(0.0,\"inf\")"}},
            "QCD_2j0t":self.module("ThetaModel").makeLogNormal(1.0,1.0),
            #"QCD_2j1t":{"type":"gauss","config":{"mean": "1.0", "width":"1.0", "range":"(0.0,\"inf\")"}},
            #"QCD_3j1t":{"type":"gauss","config":{"mean": "1.0", "width":"1.0", "range":"(0.0,\"inf\")"}},
            #"QCD_3j2t":{"type":"gauss","config":{"mean": "1.0", "width":"1.0", "range":"(0.0,\"inf\")"}},
            
            #"lumi":{"type":"gauss","config":{"mean": "1.0", "width":"0.1", "range":"(0.0,\"inf\")"}}
        }
        return uncertainties
        
    def getObservablesDict(self):
        observables = {
            "2j0t": {
                "weight":self.module("Utils").getCategoryCutStr(2,0)
            },
        }
        return observables
        
    def getFitVariableStr(self):
        return "(SingleTop_1__mtw_beforePz)"
        
        
    def getBinning(self):
        return 30
         
    def getRange(self):
        return [0.0,200.0]
        
    def getComponentsDict(self):
        components={
            "WZjets":
            {
                "sets":["WJetsAMC","tWChannel","TTJets","tChannel","DY"],
                "uncertainties":["WZjets"],
                "weight":"1",
                "color":ROOT.kGreen+1
            },
            
            "QCD_2j0t":
            {
                "sets":["data76_antiiso","MC_antiiso"],
                "uncertainties":["QCD_2j0t"],
                "weight":"(Reconstructed_1__nSelectedJet==2)*(Reconstructed_1__nSelectedBJet==0)",
                "color":ROOT.kGray
            }
        }
    
        return components
