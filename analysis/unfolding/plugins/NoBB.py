import ROOT
import math
import numpy
import random
import os
from ModelClasses import *

from defaultModules import Module

import logging

class ThetaModelNoBB(Module.getClass("ThetaModel")):
    def __init__(self,options=[]):
        ThetaModelNoBB.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)

    def getModel(self,name):
        return Model(name, {"bb_uncertainties":"false"})
        
class UtilsNoBB(Module.getClass("Utils")):
    def __init__(self,options=[]):
        UtilsNoBB.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getOutputFolder(self):
        return os.path.join(os.getcwd(),"result/NoBB")
    
 
