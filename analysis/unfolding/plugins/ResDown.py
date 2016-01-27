import ROOT
import math
import numpy
import random
import os

from defaultModules import Module

import logging

class UtilsResDown(Module.getClass("Utils")):
    def __init__(self,options=[]):
        UtilsResDown.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getRecoSamplePrefix(self):
        return "_ResDown"
        
    def getOutputFolder(self):
        return "/home/fynu/mkomm/Diff13/analysis/unfolding/result/ResDown"
        
    def getGenWeightStr(self):
        return str(self.module("Utils").getLumi())+"*mc_weight*((Generated_1__genweight<0)*(-1)+(Generated_1__genweight>0)*1)"
    
