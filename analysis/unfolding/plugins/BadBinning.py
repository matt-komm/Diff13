import ROOT
import math
import numpy
import random
import os

from defaultModules import Module

import logging

class BadResponseMatrixPt(Module.getClass("ResponseMatrixPt")):
    def __init__(self,options=[]):
        BadResponseMatrixPt.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getRecoBinning(self):
        return numpy.linspace(0,300,13)
        
    def getGenBinning(self):
        return numpy.linspace(0,300,7)

class BadResponseMatrixY(Module.getClass("ResponseMatrixY")):
    def __init__(self,options=[]):
        BadResponseMatrixY.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getRecoBinning(self):
        return numpy.linspace(0,2.4,9)
        
    def getGenBinning(self):
        return numpy.linspace(0,2.4,5)
