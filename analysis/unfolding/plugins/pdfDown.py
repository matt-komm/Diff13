import ROOT
import math
import numpy
import random
import os
import copy

from defaultModules import Module

import logging


class SamplesPDFDown(Module.getClass("Samples")):
    def __init__(self,options=[]):
        SamplesPDFDown.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getSample(self,name):
        syst = self.module("Utils").getRecoSamplePrefix()
        norm = {
            "tChannel":0.995,
            "TTJets":0.985,
            "WJetsAMC":0.991,
            "DY":0.992
        }
        sampleDict = copy.deepcopy(SamplesPDFDown.baseClass.getSample(self,name))
        if norm.has_key(name):
            sampleDict["weight"]+="*Reconstructed_1__pdfDown/"+str(norm[name])
        return sampleDict
        

class UtilsPDFDown(Module.getClass("Utils")):
    def __init__(self,options=[]):
        UtilsPDFDown.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getOutputFolder(self):
        return os.path.join(os.getcwd(),"result/PDFDown")
        
        
