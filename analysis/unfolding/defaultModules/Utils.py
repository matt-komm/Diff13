import re
import os
import ROOT
import random
import shutil
from Module import Module

import logging

class Utils(Module):
    def __init__(self,options=[]):
        Module.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getOutputFolder(self):
        return os.path.join(os.getcwd(),"result/nominal")
        
    def createOutputFolder(self,force=False):
        try:
            destination = os.path.join("/home/fynu/mkomm/Diff13/analysis/unfolding/result",self.module("Utils").getOutputFolder().split("result/")[1])
            if destination!=self.module("Utils").getOutputFolder():
                self._logger.info("Checking results in '"+destination+"'")
                if os.path.exists(destination):
                    self._logger.info("Copy results from '"+destination+"' to '"+self.module("Utils").getOutputFolder()+"'")
                    shutil.copytree(destination,self.module("Utils").getOutputFolder())
                else:
                    self._logger.info("No results found in '"+destination+"'")
        
        
            if os.path.exists(self.module("Utils").getOutputFolder()) and not force:
                self._logger.warning("Output folder already exists!: "+self.module("Utils").getOutputFolder())
            elif os.path.exists(self.module("Utils").getOutputFolder()) and force:
                self._logger.info("delete existing output folder: "+self.module("Utils").getOutputFolder())
                shutil.rmtree(self.module("Utils").getOutputFolder())
            if not os.path.exists(self.module("Utils").getOutputFolder()):
                self._logger.info("creating output folder: "+self.module("Utils").getOutputFolder())
                os.makedirs(self.module("Utils").getOutputFolder())
                
            
                
        except Exception,e:
            self._logger.error(str(e))
        
    def getBDTCutStr(self):
        return "(TMath::TanH((Reconstructed_1__BDT_adaboost04_minnode001_maxvar3_ntree1000_invboost_binned)*3.0)>0.65)"
        
    def getLumi(self):
        return 2290.0
        
    def getRecoWeightStr(self):
        return self.module("Utils").getGenWeightStr()+"*(testing==1)/splitweight*(Reconstructed_1__PU69000_weight*Reconstructed_1__btagging_nominal*SingleTop_1__TightMuon_1__id_SF_nominal*SingleTop_1__TightMuon_1__iso_SF_nominal*SingleTop_1__TightMuon_1__trigger_SF_nominal)"
    
    def getGenWeightStr(self):
        return str(self.module("Utils").getLumi())+"*mc_weight*((Generated_1__genweight<0)*(-1)+(Generated_1__genweight>0)*1)"
    
    def getRecoWeightStrPseudo(self):
        return self.module("Utils").getGenWeightStrPseudo()+"*(testing==1)/splitweight*(Reconstructed_1__PU69000_weight*Reconstructed_1__btagging_nominal*SingleTop_1__TightMuon_1__id_SF_nominal*SingleTop_1__TightMuon_1__iso_SF_nominal*SingleTop_1__TightMuon_1__trigger_SF_nominal)"
    
    def getGenWeightStrPseudo(self):
        return str(self.module("Utils").getLumi())+"*mc_weight*((Generated_1__genweight<0)*(-1)+(Generated_1__genweight>0)*1)"
    
    def getTriggerCutMCStr(self):
        return "(Reconstructed_1__HLT_IsoMu20_vALL==1)"
        
    def getTriggerCutDataStr(self):
        return "(Reconstructed_1__HLT_IsoMu20_vALL==1)"

    def getCategoryCutStr(self,njets,nbtags):
        return "(Reconstructed_1__nSelectedJet=="+str(int(njets))+")*(Reconstructed_1__nSelectedBJet=="+str(int(nbtags))+")"
        
    def getMTWCutValue(self):
        return 50.0
        
    def getMTWCutStr(self):
        return "(SingleTop_1__mtw_beforePz>"+str(self.module("Utils").getMTWCutValue())+")"
        
    def getRecoSamplePrefix(self):
        return ""
        
    def getRecoSamplePrefixPseudo(self):
        return ""
            
    def getHist1D(self,hist,fileName,processName,variableName,weight):
        hist.SetDirectory(0)
        rootFile = ROOT.TFile(fileName)
        tree = rootFile.Get(processName)
        tempHist=hist.Clone()
        tempHist.Scale(0)
        tempHist.SetEntries(0)
        tempHist.SetName("hist_"+processName+str(random.random()))
        if (tree):
            tree.Project(tempHist.GetName(),variableName,weight)
            tempHist.SetDirectory(0)
            tempHist.SetBinContent(0,0)
            tempHist.SetBinContent(tempHist.GetNbinsX()+1,0)
            hist.Add(tempHist)
        rootFile.Close()
        
    def getHist2D(self,hist,fileName,processName,variableNameX,variableNameY,weight):
        hist.SetDirectory(0)
        rootFile = ROOT.TFile(fileName)
        tree = rootFile.Get(processName)
        tempHist=hist.Clone()
        tempHist.Scale(0)
        tempHist.SetEntries(0)
        if (tree):
            tree.Project(tempHist.GetName(),variableNameY+":"+variableNameX,weight)
            tempHist.SetDirectory(0)
            for ibin in range(tempHist.GetNbinsX()+2):
                tempHist.SetBinContent(ibin,0,0)
                tempHist.SetBinContent(ibin,tempHist.GetNbinsY()+1,0)
            for jbin in range(tempHist.GetNbinsY()+2):
                tempHist.SetBinContent(0,jbin,0)
                tempHist.SetBinContent(tempHist.GetNbinsX()+1,jbin,0)
            hist.Add(tempHist)
        rootFile.Close()
        
    def normalizeByBinWidth(self,hist):
        hist.Scale(1./hist.Integral())
        
        for ibin in range(hist.GetNbinsX()):
            hist.SetBinContent(ibin+1,hist.GetBinContent(ibin+1)/hist.GetBinWidth(ibin+1))
            hist.SetBinError(ibin+1,hist.GetBinError(ibin+1)/hist.GetBinWidth(ibin+1))
        
            
    def normalizeByTransistionProbability(self,responseMatrix):
        responseMatrixSelectedNormalized = responseMatrix.Clone(responseMatrix.GetName()+"Norm")

        for genBin in range(responseMatrixSelectedNormalized.GetNbinsX()):
            s = 0.0
            for recoBin in range(responseMatrixSelectedNormalized.GetNbinsY()):
                s+=responseMatrixSelectedNormalized.GetBinContent(genBin+1,recoBin+1)
            for recoBin in range(responseMatrixSelectedNormalized.GetNbinsY()):
                c = responseMatrixSelectedNormalized.GetBinContent(genBin+1,recoBin+1)
                responseMatrixSelectedNormalized.SetBinContent(genBin+1,recoBin+1,c/s)
        return responseMatrixSelectedNormalized
            
