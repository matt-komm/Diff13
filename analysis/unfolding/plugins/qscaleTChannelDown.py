import ROOT
import math
import numpy
import random
import os

from defaultModules import Module

import logging

class SamplesQScaleTChannelDown(Module.getClass("Samples")):
    def __init__(self,options=[]):
        SamplesQScaleTChannelDown.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getSample(self,name):
        if name!="tChannel":
            return SamplesQScaleTChannelDown.baseClass.getSample(self,name)
        else:
            mcweight = self.module("Utils").getRecoWeightStr()+"*"+self.module("Utils").getTriggerCutMCStr()
            dataweight = "1"
            return {
                    "processes":[
                        #"ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_iso",
                        "ST_t-channel_4f_scaledown_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_ext_iso",
                    ],
                    "color":ROOT.gROOT.GetColor(ROOT.kRed),
                    "title":"t-channel",
                    "weight":mcweight+"*(Generated_1__lheweight_1009/Generated_1__lheweight_1001/1.27)"
            }
        
class ResponseMatrixPtTopMassUp(Module.getClass("ResponseMatrixPt")):
    def __init__(self,options=[]):
        ResponseMatrixPtTopMassUp.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getSignalProcessNames(self):
        return ["ST_t-channel_4f_scaledown_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_ext"]
        
    def getAdditionalGenWeight(self):
        return "(Generated_1__lheweight_1009/Generated_1__lheweight_1001/1.27)"
        
    def getAdditionalRecoWeight(self):
        return "(Generated_1__lheweight_1009/Generated_1__lheweight_1001/1.27)"
        
class ResponseMatrixYTopMassUp(Module.getClass("ResponseMatrixY")):
    def __init__(self,options=[]):
        ResponseMatrixYTopMassUp.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getSignalProcessNames(self):
        return ["ST_t-channel_4f_scaledown_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_ext"]
        
    def getAdditionalGenWeight(self):
        return "(Generated_1__lheweight_1009/Generated_1__lheweight_1001/1.27)"
        
    def getAdditionalRecoWeight(self):
        return "(Generated_1__lheweight_1009/Generated_1__lheweight_1001/1.27)"
        
class UtilsQScaleTChannelDown(Module.getClass("Utils")):
    def __init__(self,options=[]):
        UtilsQScaleTChannelDown.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG) 
        
    def getOutputFolder(self):
        return os.path.join(os.getcwd(),"result/QScaleTChannelDown")
        
        
