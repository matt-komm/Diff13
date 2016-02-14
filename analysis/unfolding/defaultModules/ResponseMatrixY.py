import numpy
import ROOT
import os
import random

from Module import Module

import logging


class ResponseMatrixY(Module):
    def __init__(self,options=[]):
        Module.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
       
    def getRecoUnfoldingVariable(self):
        return "fabs(SingleTop_1__Top_1__y)"
        
    def getGenUnfoldingVariable(self):
        return "fabs(Generated_1__top_y)"
        
    def getRecoBinning(self):
        genBinning = self.module("ResponseMatrixY").getGenBinning()
        b = numpy.zeros(len(genBinning)*2-1)
        for i in range(len(genBinning)-1):
            b[2*i]=genBinning[i]
            b[2*i+1]=0.5*(genBinning[i]+genBinning[i+1])
        b[-1]=genBinning[-1]
        return b
        #return numpy.array([0.0,0.15,0.3,0.45,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.1,2.4])
        
    def getGenBinning(self):
        #return numpy.array([0.0,0.4,0.8,1.2,1.6,2.0,2.4])
        #return numpy.linspace(0,2.4,7)
        #return numpy.array([0.0,0.3,0.6,1.0,1.5,2.0,2.4])
        return numpy.array([0.0, 0.35, 0.7, 1.1, 1.6, 2.4])
        
    def getSignalProcessNames(self):
        return ["ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_ext"]
        
    def buildResponseMatrix(self,responseMatrix, efficiencyHist):
        res = responseMatrix.Clone(responseMatrix.GetName()+"Clone")
        for igen in range(responseMatrix.GetNbinsX()):
            res.SetBinContent(igen+1,0,efficiencyHist.GetBinContent(igen+1))
        return res
        
    def saveResponseMatrix(self,responseMatrix):
        fullPath = os.path.join(self.module("Utils").getOutputFolder(),"responseY.root")
        rootFile = ROOT.TFile(fullPath,"RECREATE")
        responseMatrixSave = responseMatrix.Clone("responseMatrixY")
        responseMatrixSave.SetDirectory(rootFile)
        responseMatrixSave.Write()
        rootFile.Close()
        
    def loadResponseMatrix(self):
        fullPath = os.path.join(self.module("Utils").getOutputFolder(),"responseY.root")
        if not os.path.exists(fullPath):
            return None
        rootFile = ROOT.TFile(fullPath)
        responseMatrix = rootFile.Get("responseMatrixY").Clone("responseMatrix"+str(random.random()))
        responseMatrix.SetDirectory(0)
        rootFile.Close()
        return responseMatrix
        
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
        
        recoBinning = self.module("ResponseMatrixY").getRecoBinning()
        genBinning = self.module("ResponseMatrixY").getGenBinning()
        
        responseMatrixSelected = ROOT.TH2D("responseSelected"+str(random.random()),";gen;reco",len(genBinning)-1,genBinning,len(recoBinning)-1,recoBinning)
        responseMatrixUnselected = ROOT.TH1D("responseUnselected"+str(random.random()),";gen;",len(genBinning)-1,genBinning)
        efficiencyHist = ROOT.TH1D("efficiencyHist"+str(random.random()),";gen;",len(genBinning)-1,genBinning)
        
        for f in responseFiles:
            for processName in self.module("ResponseMatrixY").getSignalProcessNames():
                
                self.module("Utils").getHist2D(
                    responseMatrixSelected,
                    f,
                    processName+"_iso"+self.module("Utils").getRecoSamplePrefix(),
                    self.module("ResponseMatrixY").getGenUnfoldingVariable(),
                    self.module("ResponseMatrixY").getRecoUnfoldingVariable(),
                    recoweight+"*"+cut
                )
                self.module("Utils").getHist1D(
                    responseMatrixUnselected,
                    f,
                    processName+"_iso"+self.module("Utils").getRecoSamplePrefix(),
                    self.module("ResponseMatrixY").getGenUnfoldingVariable(),
                    recoweight+"*!("+cut+")"
                )
        
        for f in efficiencyFiles:
            for processName in self.module("ResponseMatrixY").getSignalProcessNames():
                for prefix in ["","_iso","_midiso","_antiiso"]:
                    self.module("Utils").getHist1D(
                        efficiencyHist,
                        f,
                        processName+prefix,
                        self.module("ResponseMatrixY").getGenUnfoldingVariable(),
                        genweight
                    )
        self._logger.debug("response matrix selected: "+str(responseMatrixSelected.Integral())+" events")
        self._logger.debug("response matrix unselected: "+str(responseMatrixUnselected.Integral())+" events")
        self._logger.debug("efficiency: "+str(efficiencyHist.Integral())+" events")
        
        efficiencyHist.Add(responseMatrixUnselected)

        return self.module("ResponseMatrixY").buildResponseMatrix(responseMatrixSelected,efficiencyHist)

