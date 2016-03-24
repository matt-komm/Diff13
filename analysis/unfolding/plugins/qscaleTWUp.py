import ROOT
import math
import numpy
import random
import os

from defaultModules import Module

import logging

class SamplesQScaleTWUp(Module.getClass("Samples")):
    def __init__(self,options=[]):
        SamplesQScaleTWUp.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getSample(self,name):
        if name!="tWChannel":
            return SamplesQScaleTWUp.baseClass.getSample(self,name)
        else:
            syst = self.module("Utils").getRecoSamplePrefix()
            mcweight = self.module("Utils").getRecoWeightStr()+"*"+self.module("Utils").getTriggerCutMCStr()
            dataweight = "1"
            return {
                "processes":[
                    "ST_tW_top_5f_scaleup_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_iso"+syst,
                    "ST_tW_antitop_5f_scaleup_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_iso"+syst
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kOrange-3),
                "title":"t#bar{t}",
                "weight":mcweight
            }
            
            
        
        
class UtilsQScaleTWUp(Module.getClass("Utils")):
    def __init__(self,options=[]):
        UtilsQScaleTWUp.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG) 
        
    def getOutputFolder(self):
        return os.path.join(os.getcwd(),"result/QScaleTWUp")
        
        
