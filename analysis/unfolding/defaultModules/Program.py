import ROOT

from Module import Module

import logging

class Program(Module):
    def __init__(self,options=[]):
        Module.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def execute(self):
        
        responseMatrix = self.module("ResponseMatrix").getResponseMatrix()
        responseMatrixNorm = self.module("Utils").normalizeByTransistionProbability(responseMatrix)
        cv = ROOT.TCanvas("cv","",800,600)
        responseMatrixNorm.Draw("colz text")
        cv.Update()
        cv.WaitPrimitive()
        
