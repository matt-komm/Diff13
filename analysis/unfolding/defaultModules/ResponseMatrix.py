import numpy
import ROOT

import logging
logger = logging.getLogger(__file__)

class ResponseMatrix(object):
    def __init__(self,defaultModules,options=[]):
        self._defaultModules = defaultModules
        
    def getRecoWeightStr(self):  
        return self._defaultModules["ResponseMatrix"](self._defaultModules).getGenWeightStr()+"*(Reconstructed_1__PU69000_weight*Reconstructed_1__btagging_nominal)"
    
    def getGenWeightStr(self):  
        return str(self._defaultModules["Utils"].getLumi())+"*mc_weight*((Generated_1__genweight<0)*(-1)+(Generated_1__genweight>0)*1)"
    
    def getCutStr(self):  
        cut = "(Reconstructed_1__HLT_IsoMu20_v1==1)*(Reconstructed_1__nSelectedJet==2)*(Reconstructed_1__nSelectedBJet==1)"
        cut +="*"+self._defaultModules["Utils"].getBDTCutStr()
        return cut
        
    @staticmethod
    def getRecoUnfoldingVariable():
        return "SingleTop_1__Top_1__Pt"
        
    @staticmethod
    def getGenUnfoldingVariable():
        return "Generated_1__top_pt"
        
    @staticmethod
    def getRecoBinning():
        #return numpy.array([0, 20., 30., 37.5, 45., 55., 65., 75., 85., 105., 130, 180.,300])
        return numpy.array([0.,20.,40.,60.,80.,100.,120.,140.,160.,180.,200.,250.,300.])
        
    @staticmethod
    def getGenBinning():
        #return numpy.array([0,      30.,       45.,      65.,      85.,       130,      300])
        return numpy.array([0.,    40.,    80.,     120.,     160.,     200.,     300.])
        
    @staticmethod
    def getSignalProcessNames():
        return ["ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_ext_iso"]
        
    @staticmethod
    def buildResponseMatrix(responseMatrix, efficiencyHist):
        res = responseMatrix.Clone(responseMatrix.GetName()+"Clone")
        for igen in range(responseMatrix.GetNbinsX()):
            res.SetBinContent(igen+1,0,efficiencyHist.GetBinContent(igen+1))
        return res

        
    def getResponseMatrix(self):  
        responseFiles = self._defaultModules["Utils"].getResponseFiles()
        efficiencyFiles = self._defaultModules["Utils"].getEfficiencyFiles()
        
        genweight = self._defaultModules["ResponseMatrix"](self._defaultModules).getGenWeightStr()
        recoweight = self._defaultModules["ResponseMatrix"](self._defaultModules).getRecoWeightStr()
        cut = self._defaultModules["ResponseMatrix"](self._defaultModules).getCutStr()
        
        recoBinning = self._defaultModules["ResponseMatrix"].getRecoBinning()
        genBinning = self._defaultModules["ResponseMatrix"].getGenBinning()
        
        responseMatrixSelected = ROOT.TH2D("responseSelected",";gen;reco",len(genBinning)-1,genBinning,len(recoBinning)-1,recoBinning)
        efficiencyHist = ROOT.TH1D("efficiencyHist",";gen;",len(genBinning)-1,genBinning)
        
        for f in responseFiles:
            for processName in self._defaultModules["ResponseMatrix"].getSignalProcessNames():
                
                self._defaultModules["Utils"].getHist2D(
                    responseMatrixSelected,
                    f,
                    processName,
                    self._defaultModules["ResponseMatrix"].getGenUnfoldingVariable(),
                    self._defaultModules["ResponseMatrix"].getRecoUnfoldingVariable(),
                    recoweight+"*"+cut
                )
                self._defaultModules["Utils"].getHist1D(
                    efficiencyHist,
                    f,
                    processName,
                    self._defaultModules["ResponseMatrix"].getGenUnfoldingVariable(),
                    recoweight+"*!("+cut+")"
                )
        
        for f in efficiencyFiles:
            for processName in self._defaultModules["ResponseMatrix"].getSignalProcessNames():
                self._defaultModules["Utils"].getHist1D(
                    efficiencyHist,
                    f,
                    processName,
                    self._defaultModules["ResponseMatrix"].getGenUnfoldingVariable(),
                    genweight
                )

        return self._defaultModules["ResponseMatrix"].buildResponseMatrix(responseMatrixSelected,efficiencyHist)

