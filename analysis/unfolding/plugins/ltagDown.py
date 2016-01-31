import ROOT
import math
import numpy
import random
import os

from defaultModules import Module

import logging

class UtilsLtagDown(Module.getClass("Utils")):
    def __init__(self,options=[]):
        UtilsLtagDown.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getRecoWeightStr(self):
        return self.module("Utils").getGenWeightStr()+"*(Reconstructed_1__PU69000_weight*Reconstructed_1__btagging_l_down)"

        
    def getOutputFolder(self):
        return "/home/fynu/mkomm/Diff13/analysis/unfolding/result/LtagDown"