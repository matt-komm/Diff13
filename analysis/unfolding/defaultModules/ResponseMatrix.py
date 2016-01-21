import numpy
import ROOT

from Module import Module

import logging


class ResponseMatrix(Module):
    def __init__(self,defaultModules,options=[]):
        Module.__init__(self,defaultModules,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
       
    def getRecoUnfoldingVariable(self):
        return "SingleTop_1__Top_1__Pt"
        
    def getGenUnfoldingVariable(self):
        return "Generated_1__top_pt"
        
    def getRecoBinning(self):
        #return numpy.array([0, 20., 30., 37.5, 45., 55., 65., 75., 85., 105., 130, 180.,300])
        return numpy.array([0.,20.,40.,60.,80.,100.,120.,140.,160.,180.,200.,250.,300.])
        
    def getGenBinning(self):
        #return numpy.array([0,      30.,       45.,      65.,      85.,       130,      300])
        return numpy.array([0.,    40.,    80.,     120.,     160.,     200.,     300.])
        
    def getSignalProcessNames(self):
        return ["ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_ext_iso"]
        
    def buildResponseMatrix(self,responseMatrix, efficiencyHist):
        res = responseMatrix.Clone(responseMatrix.GetName()+"Clone")
        for igen in range(responseMatrix.GetNbinsX()):
            res.SetBinContent(igen+1,0,efficiencyHist.GetBinContent(igen+1))
        return res

        
    def getResponseMatrix(self):  
        responseFiles = self.module("Files").getResponseFiles()
        efficiencyFiles = self.module("Files").getEfficiencyFiles()
        
        genweight = self.module("Utils").getGenWeightStr()
        recoweight = self.module("Utils").getRecoWeightStr()
        cut = self.module("Utils").getMTWCutStr()
        cut += "*"+self.module("Utils").getTriggerCutMCStr()
        cut += "*"+self.module("Utils").getCategoryCutStr(2,1)
        cut += "*"+self.module("Utils").getBDTCutStr()
        
        self._logger.debug("apply gen weight for response matrix: "+genweight)
        self._logger.debug("apply reco weight for response matrix: "+recoweight)
        self._logger.debug("apply cut for response matrix: "+cut)
        
        recoBinning = self.module("ResponseMatrix").getRecoBinning()
        genBinning = self.module("ResponseMatrix").getGenBinning()
        
        responseMatrixSelected = ROOT.TH2D("responseSelected",";gen;reco",len(genBinning)-1,genBinning,len(recoBinning)-1,recoBinning)
        efficiencyHist = ROOT.TH1D("efficiencyHist",";gen;",len(genBinning)-1,genBinning)
        
        for f in responseFiles:
            for processName in self.module("ResponseMatrix").getSignalProcessNames():
                
                self.module("Utils").getHist2D(
                    responseMatrixSelected,
                    f,
                    processName,
                    self.module("ResponseMatrix").getGenUnfoldingVariable(),
                    self.module("ResponseMatrix").getRecoUnfoldingVariable(),
                    recoweight+"*"+cut
                )
                self.module("Utils").getHist1D(
                    efficiencyHist,
                    f,
                    processName,
                    self.module("ResponseMatrix").getGenUnfoldingVariable(),
                    recoweight+"*!("+cut+")"
                )
        
        for f in efficiencyFiles:
            for processName in self.module("ResponseMatrix").getSignalProcessNames():
                self.module("Utils").getHist1D(
                    efficiencyHist,
                    f,
                    processName,
                    self.module("ResponseMatrix").getGenUnfoldingVariable(),
                    genweight
                )

        return self.module("ResponseMatrix").buildResponseMatrix(responseMatrixSelected,efficiencyHist)

