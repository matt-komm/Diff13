import ROOT
import math
import numpy
import random
import os

from defaultModules import Module

import logging

class SamplesPowheg(Module.getClass("Samples")):
    def __init__(self,options=[]):
        SamplesPowheg.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getSample(self,name):
        syst = self.module("Utils").getRecoSamplePrefix()
        mcweight = self.module("Utils").getRecoWeightStr()+"*"+self.module("Utils").getTriggerCutMCStr()
        dataweight = "1"
        
        altDict = {
            "tChannel":
            {
                "processes":[
                    "ST_t-channel_5f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_iso"+syst,
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kRed),
                "title":"t-channel",
                "weight":mcweight
            }
        }
        
        if name not in altDict.keys():
            return SamplesPowheg.baseClass.getSample(self,name)
        else:
            return altDict[name]
            
        
class ResponseMatrixPtPowheg(Module.getClass("ResponseMatrixPt")):
    def __init__(self,options=[]):
        ResponseMatrixPtPowheg.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getSignalProcessNames(self):
        return [
            "ST_t-channel_5f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1"
        ]
        
class ResponseMatrixYPowheg(Module.getClass("ResponseMatrixY")):
    def __init__(self,options=[]):
        ResponseMatrixYPowheg.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getSignalProcessNames(self):
        return [
            "ST_t-channel_5f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1"
        ]
        
class UtilsPowheg(Module.getClass("Utils")):
    def __init__(self,options=[]):
        UtilsPowheg.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getOutputFolder(self):
        return os.path.join(os.getcwd(),"result/AMC5FS")
        
        
