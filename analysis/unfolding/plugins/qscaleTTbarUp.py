import ROOT
import math
import numpy
import random
import os

from defaultModules import Module

import logging

class SamplesQScaleTTbarUp(Module.getClass("Samples")):
    def __init__(self,options=[]):
        SamplesQScaleTTbarUp.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getSample(self,name):
        if name!="TTJets":
            return SamplesQScaleTTbarUp.baseClass.getSample(self,name)
        else:
            syst = self.module("Utils").getRecoSamplePrefix()
            mcweight = self.module("Utils").getRecoWeightStr()+"*"+self.module("Utils").getTriggerCutMCStr()
            dataweight = "1"
            return {
                "processes":[
                    "TT_TuneCUETP8M1_13TeV-powheg-scaleup-pythia8_ext_iso"+syst
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kOrange-3),
                "title":"t#bar{t}",
                "weight":mcweight+"*(Generated_1__lheweight_1005/Generated_1__lheweight_1001/0.872)"
            }
        
        
class UtilsQScaleTTbarUp(Module.getClass("Utils")):
    def __init__(self,options=[]):
        UtilsQScaleTTbarUp.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG) 
        
    def getOutputFolder(self):
        return os.path.join(os.getcwd(),"result/QScaleTTbarUp")
