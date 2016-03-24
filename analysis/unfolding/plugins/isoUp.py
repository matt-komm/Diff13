import ROOT
import math
import numpy
import random
import os

from defaultModules import Module

import logging

class SamplesIsoUp(Module.getClass("Samples")):
    def __init__(self,options=[]):
        SamplesIsoUp.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getQCDIsoCutStr(self):
        return "(SingleTop_1__TightMuon_1__relIso>0.4)*(SingleTop_1__TightMuon_1__relIso<10000.0)"

        
class UtilsIsoUp(Module.getClass("Utils")):
    def __init__(self,options=[]):
        UtilsIsoUp.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getOutputFolder(self):
        return os.path.join(os.getcwd(),"result/IsoUp")
