import ROOT
import math
import numpy
import random
import os

from defaultModules import Module

import logging

class SamplesQScaleTTbarDown(Module.getClass("Samples")):
    def __init__(self,options=[]):
        SamplesQScaleTTbarDown.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getSample(self,name):
        if name!="TTJets":
            return SamplesQScaleTTbarDown.baseClass.getSample(self,name)
        else:
            mcweight = self.module("Utils").getRecoWeightStr()+"*"+self.module("Utils").getTriggerCutMCStr()
            dataweight = "1"
            return {
                "processes":[
                    "TT_TuneCUETP8M1_13TeV-powheg-scaledown-pythia8_ext_iso"
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kOrange-3),
                "title":"t#bar{t}",
                "weight":mcweight+"*(Generated_1__lheweight_1009/Generated_1__lheweight_1001/1.14)"+"*Generated_1__top_pt_rew"
            }
            
            
class UtilsQScaleTTbarDown(Module.getClass("Utils")):
    def __init__(self,options=[]):
        UtilsQScaleTTbarDown.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG) 
        
    def getOutputFolder(self):
        return os.path.join(os.getcwd(),"result/QScaleTTbarDown")
        
        
