import ROOT
import math
import numpy
import random
import os
import copy

from defaultModules import Module

import logging


class SamplesTWUp(Module.getClass("Samples")):
    def __init__(self,options=[]):
        SamplesTWUp.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getSample(self,name):
        syst = self.module("Utils").getRecoSamplePrefix()
        #TW/TT = 10%
        # => TW*R
        # => TT*(1+10%*(1-R))
        
        R = 1.2
        W = (1.0+0.10*(1-R))
        norm = {
            "TTJets":"%5.3f"%W,
            "tWChannel":"%5.3f"%R,
        }
        sampleDict = copy.deepcopy(SamplesTWUp.baseClass.getSample(self,name))
        if norm.has_key(name):
            sampleDict["weight"]+="*("+str(norm[name])+")"
        return sampleDict
        
 
class UtilsTWUp(Module.getClass("Utils")):
    def __init__(self,options=[]):
        UtilsTWUp.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getOutputFolder(self):
        return os.path.join(os.getcwd(),"result/TWUp")
        
        