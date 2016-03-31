import ROOT
import math
import numpy
import random
import os

from defaultModules import Module

import logging

class SamplesQScaleWjetsDown(Module.getClass("Samples")):
    def __init__(self,options=[]):
        SamplesQScaleWjetsDown.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getSample(self,name):
        if name!="WJetsAMC":
            return SamplesQScaleWjetsDown.baseClass.getSample(self,name)
        else:
            syst = self.module("Utils").getRecoSamplePrefix()
            mcweight = self.module("Utils").getRecoWeightStr()+"*"+self.module("Utils").getTriggerCutMCStr()
            dataweight = "1"
            return {
                "processes":[
                    "WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_ext_iso"+syst
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kGreen-2),
                "title":"W+jets",
                "addtitle":"(aMC@NLO)",
                "weight":mcweight+"*(Generated_1__lheweight_1009/Generated_1__lheweight_1001)*((Generated_1__genweight<0)/1.12+(Generated_1__genweight>0)/1.01)"
            }
            
            
        
        
class UtilsQScaleWjetsDown(Module.getClass("Utils")):
    def __init__(self,options=[]):
        UtilsQScaleWjetsDown.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG) 
        
    def getOutputFolder(self):
        return os.path.join(os.getcwd(),"result/QScaleWjetsDown")
        
        
