import ROOT
import math
import numpy
import random
import os

from defaultModules import Module

import logging

class SamplesNoTopPt(Module.getClass("Samples")):
    def __init__(self,options=[]):
        SamplesNoTopPt.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getSample(self,name):
        syst = self.module("Utils").getRecoSamplePrefix()
        mcweight = self.module("Utils").getRecoWeightStr()+"*"+self.module("Utils").getTriggerCutMCStr()
        dataweight = "1"
        
        altDict = {
            "TTJets":
            {
                "processes":[
                    "TT_TuneCUETP8M1_13TeV-powheg-pythia8_ext_iso"+syst
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kOrange-3),
                "title":"t#bar{t}",
                "weight":mcweight
            },
        }
        
        if name not in altDict.keys():
            return SamplesNoTopPt.baseClass.getSample(self,name)
        else:
            return altDict[name]
            

        
class UtilsNoTopPt(Module.getClass("Utils")):
    def __init__(self,options=[]):
        UtilsNoTopPt.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getOutputFolder(self):
        return os.path.join(os.getcwd(),"result/NoTopPt")
        
        
