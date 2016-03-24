import ROOT
import math
import numpy
import random
import os
import copy

from defaultModules import Module

import logging


class SamplesPDFUp(Module.getClass("Samples")):
    def __init__(self,options=[]):
        SamplesPDFUp.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getSample(self,name):
        syst = self.module("Utils").getRecoSamplePrefix()
        norm = {
            "tChannel":1.008,
            "TTJets":1.014,
            "WJetsAMC":0.991,
            "DY":1.012
        }
        sampleDict = copy.deepcopy(SamplesPDFUp.baseClass.getSample(self,name))
        if norm.has_key(name):
            sampleDict["weight"]+="*Reconstructed_1__pdfUp/"+str(norm[name])
        return sampleDict

class UtilsPDFUp(Module.getClass("Utils")):
    def __init__(self,options=[]):
        UtilsPDFUp.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getOutputFolder(self):
        return os.path.join(os.getcwd(),"result/PDFUp")
