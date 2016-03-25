import ROOT
import math
import numpy
import random
import os

from defaultModules import Module

import logging

class UtilsMuonUp(Module.getClass("Utils")):
    def __init__(self,options=[]):
        UtilsMuonUp.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getRecoWeightStr(self):
        cut =  self.module("Utils").getGenWeightStr()+"*(testing==1)/splitweight"
        cut+="*(Reconstructed_1__PU69000_weight*Reconstructed_1__btagging_nominal)"
        cut+="*("
        cut+="(Reconstructed_1__nSelectedJet==2)*(Reconstructed_1__nSelectedBJet<2)*(SingleTop_1__TightMuon_1__id_SF_up*SingleTop_1__TightMuon_1__iso_SF_up*SingleTop_1__TightMuon_1__trigger_SF_up-1.)"
        cut+="+(Reconstructed_1__nSelectedJet==3)*(Reconstructed_1__nSelectedBJet<3)*(SingleTop_1__TightMuon_1__id_SF_up*SingleTop_1__TightMuon_1__iso_SF_up*SingleTop_1__TightMuon_1__trigger_SF_up-1.)"
        cut+="+1.)"
        return cut
        
    def getOutputFolder(self):
        return os.path.join(os.getcwd(),"result/MuonUp")
