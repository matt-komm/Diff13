import ROOT
import math
import numpy
import random
import os
import copy

from defaultModules import Module

import logging


class SamplesDYDown(Module.getClass("Samples")):
    def __init__(self,options=[]):
        SamplesDYDown.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getSample(self,name):
        syst = self.module("Utils").getRecoSamplePrefix()
        #DY/Wjet = 11%
        # => DY*R
        # => Wjet*(1+11%*(1-R))
        
        R = 0.8
        W = (1.0+0.11*(1-R))
        norm = {
            "WJetsAMC":"%5.3f"%W,
            "DY":"%5.3f"%R,
        }
        sampleDict = copy.deepcopy(SamplesDYDown.baseClass.getSample(self,name))
        if norm.has_key(name):
            sampleDict["weight"]+="*("+str(norm[name])+")"
        return sampleDict
        
 
class UtilsDYDown(Module.getClass("Utils")):
    def __init__(self,options=[]):
        UtilsDYDown.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getOutputFolder(self):
        return os.path.join(os.getcwd(),"result/DYDown")
        
        
