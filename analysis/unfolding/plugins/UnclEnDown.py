import ROOT
import math
import numpy
import random
import os

from defaultModules import Module

import logging

class UtilsUnclEnDown(Module.getClass("Utils")):
    def __init__(self,options=[]):
        UtilsUnclEnDown.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getRecoSamplePrefix(self):
        return "_UnclEnDown"
        
    def getOutputFolder(self):
        return os.path.join(os.getcwd(),"result/UnclEnDown")
        
