import ROOT
import math
import numpy
import random
import os

from defaultModules import Module

import logging

class UtilsMuonDown(Module.getClass("Utils")):
    def __init__(self,options=[]):
        UtilsMuonDown.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getRecoWeightStr(self):
        return self.module("Utils").getGenWeightStr()+"*(testing==1)/splitweight*(Reconstructed_1__PU69000_weight*Reconstructed_1__btagging_nominal*SingleTop_1__TightMuon_1__id_SF_down*SingleTop_1__TightMuon_1__iso_SF_down*SingleTop_1__TightMuon_1__trigger_SF_down)"

        
    def getOutputFolder(self):
        return os.path.join(os.getcwd(),"result/MuonDown")
