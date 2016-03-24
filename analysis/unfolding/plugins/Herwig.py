import ROOT
import math
import numpy
import random
import os

from defaultModules import Module

import logging

class SamplesHerwig(Module.getClass("Samples")):
    def __init__(self,options=[]):
        SamplesHerwig.baseClass.__init__(self,options)
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
                    "ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-herwigpp_TuneEE5C_iso"+syst,
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kRed),
                "title":"t-channel",
                "weight":mcweight
            }
        }
        
        if name not in altDict.keys():
            return SamplesHerwig.baseClass.getSample(self,name)
        else:
            return altDict[name]
            
        
class ResponseMatrixPtHerwig(Module.getClass("ResponseMatrixPt")):
    def __init__(self,options=[]):
        ResponseMatrixPtHerwig.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getSignalProcessNames(self):
        return ["ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-herwigpp_TuneEE5C"]
        
class ResponseMatrixYHerwig(Module.getClass("ResponseMatrixY")):
    def __init__(self,options=[]):
        ResponseMatrixYHerwig.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getSignalProcessNames(self):
        return ["ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-herwigpp_TuneEE5C"]
        
class UtilsHerwig(Module.getClass("Utils")):
    def __init__(self,options=[]):
        UtilsHerwig.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getOutputFolder(self):
        return os.path.join(os.getcwd(),"result/Herwig")
        
        
