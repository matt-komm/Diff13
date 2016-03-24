import ROOT
import math
import numpy
import random
import os

from defaultModules import Module

import logging

class SamplesQScaleTWDown(Module.getClass("Samples")):
    def __init__(self,options=[]):
        SamplesQScaleTWDown.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getSample(self,name):
        if name!="tWChannel":
            return SamplesQScaleTWDown.baseClass.getSample(self,name)
        else:
            syst = self.module("Utils").getRecoSamplePrefix()
            mcweight = self.module("Utils").getRecoWeightStr()+"*"+self.module("Utils").getTriggerCutMCStr()
            dataweight = "1"
            return {
                "processes":[
                    "ST_tW_top_5f_scaledown_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_iso"+syst,
                    "ST_tW_antitop_5f_scaledown_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_iso"+syst
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kOrange-3),
                "title":"t#bar{t}",
                "weight":mcweight
            }
            
            
        
        
class UtilsQScaleTWDown(Module.getClass("Utils")):
    def __init__(self,options=[]):
        UtilsQScaleTWDown.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG) 
        
    def getOutputFolder(self):
        return os.path.join(os.getcwd(),"result/QScaleTWDown")
        
        
