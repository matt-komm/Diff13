import ROOT
import math
import numpy
import random
import os

from defaultModules import Module

import logging

class UtilsEnUp(Module.getClass("Utils")):
    def __init__(self,options=[]):
        UtilsEnUp.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getRecoSamplePrefix(self):
        return "_EnUp"
        
    def getOutputFolder(self):
        return "/home/fynu/mkomm/Diff13/analysis/unfolding/result/EnUp"
        
