import numpy
import ROOT

from Module import Module

import logging


class ResponseMatrixPt(Module):
    def __init__(self,options=[]):
        Module.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
       
    def getRecoUnfoldingVariable(self):
        return "SingleTop_1__Top_1__Pt"
        
    def getGenUnfoldingVariable(self):
        return "Generated_1__top_pt"
        
    def getRecoBinning(self):
        genBinning = self.module("ResponseMatrixPt").getGenBinning()
        b = numpy.zeros(len(genBinning)*2-1)
        for i in range(len(genBinning)-1):
            b[2*i]=genBinning[i]
            b[2*i+1]=0.5*(genBinning[i]+genBinning[i+1])
        b[-1]=genBinning[-1]
        return b
        
    def getGenBinning(self):
        #return numpy.array([0,   30., 47.5,  70., 100.,  150., 300.])
        #return numpy.array([0,      30.,       45.,      65.,      85.,       130,      300])
        return numpy.array([0.,    45.,    75.,     110.,     160.,     200.,     300.])
        
    def getSignalProcessNames(self):
        return ["ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_ext"]
        
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
        
        recoBinning = self.module("ResponseMatrixPt").getRecoBinning()
        genBinning = self.module("ResponseMatrixPt").getGenBinning()
        
        responseMatrixSelected = ROOT.TH2D("responseSelected",";gen;reco",len(genBinning)-1,genBinning,len(recoBinning)-1,recoBinning)
        efficiencyHist = ROOT.TH1D("efficiencyHist",";gen;",len(genBinning)-1,genBinning)
        
        for f in responseFiles:
            for processName in self.module("ResponseMatrixPt").getSignalProcessNames():
                
                self.module("Utils").getHist2D(
                    responseMatrixSelected,
                    f,
                    processName+"_iso",
                    self.module("ResponseMatrixPt").getGenUnfoldingVariable(),
                    self.module("ResponseMatrixPt").getRecoUnfoldingVariable(),
                    recoweight+"*"+cut
                )
                self.module("Utils").getHist1D(
                    efficiencyHist,
                    f,
                    processName,
                    self.module("ResponseMatrixPt").getGenUnfoldingVariable(),
                    recoweight+"*!("+cut+")"
                )
        
        for f in efficiencyFiles:
            for processName in self.module("ResponseMatrixPt").getSignalProcessNames():
                self.module("Utils").getHist1D(
                    efficiencyHist,
                    f,
                    processName,
                    self.module("ResponseMatrixPt").getGenUnfoldingVariable(),
                    genweight
                )

        return self.module("ResponseMatrixPt").buildResponseMatrix(responseMatrixSelected,efficiencyHist)
