import ROOT

import logging
logger = logging.getLogger(__file__)

class Program(object):
    def __init__(self,defaultModules,options=[]):
        self._defaultModules=defaultModules
        
    def execute(self):
        utils = self._defaultModules["Utils"]
        
        responseMatrix = self._defaultModules["ResponseMatrix"](self._defaultModules).getResponseMatrix()
        responseMatrixNorm = self._defaultModules["Utils"].normalizeByTransistionProbability(responseMatrix)
        cv = ROOT.TCanvas("cv","",800,600)
        responseMatrixNorm.Draw("colz text")
        cv.Update()
        cv.WaitPrimitive()
        
