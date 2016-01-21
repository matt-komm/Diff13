import re
import os
import ROOT
import random

from Module import Module

import logging

class Utils(Module):
    def __init__(self,options=[]):
        Module.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getBDTCutStr(self):
        return "(Reconstructed_1__BDT_adaboost04_minnode001_maxvar2_ntree600_invboost>0.2)"
        
    def getLumi(self):
        return 2100.0
        
    def getRecoWeightStr(self):
        return self.module("Utils").getGenWeightStr()+"*(Reconstructed_1__PU69000_weight*Reconstructed_1__btagging_nominal)"
    
    def getGenWeightStr(self):
        return str(self.module("Utils").getLumi())+"*mc_weight*((Generated_1__genweight<0)*(-1)+(Generated_1__genweight>0)*1)"
    
    def getTriggerCutMCStr(self):
        return "(Reconstructed_1__HLT_IsoMu20_v1==1)"
    
    def getTriggerCutDataStr(self):
        return "((Reconstructed_1__HLT_IsoMu20_v2==1)+(Reconstructed_1__HLT_IsoMu20_v3==1))"

    def getCategoryCutStr(self,njets,nbtags):
        return "(Reconstructed_1__nSelectedJet=="+str(njets)+")*(Reconstructed_1__nSelectedBJet=="+str(nbtags)+")"
        
    def getMTWCutStr(self):
        return "(SingleTop_1__mtw_beforePz>50.0)"
        
            
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
        #hist.Scale(1./(hist.GetXaxis().GetXmax()-hist.GetXaxis().GetXmin())/hist.Integral())
        hist.Scale(1./hist.Integral())
        for ibin in range(hist.GetNbinsX()):
            hist.SetBinError(ibin+1,hist.GetBinError(ibin+1)/hist.GetBinWidth(ibin+1))
            hist.SetBinContent(ibin+1,hist.GetBinContent(ibin+1)/hist.GetBinWidth(ibin+1))
            
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
            
