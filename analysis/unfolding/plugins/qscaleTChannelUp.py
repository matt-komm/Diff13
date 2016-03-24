import ROOT
import math
import numpy
import random
import os

from defaultModules import Module

import logging

class SamplesQScaleTChannelUp(Module.getClass("Samples")):
    def __init__(self,options=[]):
        SamplesQScaleTChannelUp.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getSample(self,name):
        if name!="tChannel":
            return SamplesQScaleTChannelUp.baseClass.getSample(self,name)
        else:
    
            syst = self.module("Utils").getRecoSamplePrefix()
            mcweight = self.module("Utils").getRecoWeightStr()+"*"+self.module("Utils").getTriggerCutMCStr()
            dataweight = "1"
            return {
                    "processes":[
                        #"ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_iso"+syst,
                        "ST_t-channel_4f_scaleup_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_ext_iso"+syst,
                    ],
                    "color":ROOT.gROOT.GetColor(ROOT.kRed),
                    "title":"t-channel",
                    "weight":mcweight+"*(Generated_1__lheweight_1005/Generated_1__lheweight_1001/0.812)"
            }
        
        
class UtilsQScaleTChannelUp(Module.getClass("Utils")):
    def __init__(self,options=[]):
        UtilsQScaleTChannelUp.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG) 
        
    def getOutputFolder(self):
        return os.path.join(os.getcwd(),"result/QScaleTChannelUp")
        
        
