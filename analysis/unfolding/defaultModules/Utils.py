import re
import os
import ROOT
import random

import logging
logger = logging.getLogger(__file__)

class Utils(object):
    def __init__(self,defaultModules,options=[]):
        self._defaultModules = defaultModules
        
    @staticmethod
    def getBDTCutStr():
        return "(Reconstructed_1__BDT_adaboost04_minnode001_maxvar2_ntree600_invboost>0.2)"
        
    @staticmethod
    def getLumi():
        return 2100.0
        
    def getRecoWeightStr(self):
        return self._defaultModules["Utils"](self._defaultModules).getGenWeightStr()+"*(Reconstructed_1__PU69000_weight*Reconstructed_1__btagging_nominal)"
    
    def getGenWeightStr(self):
        return str(self._defaultModules["Utils"].getLumi())+"*mc_weight*((Generated_1__genweight<0)*(-1)+(Generated_1__genweight>0)*1)"
    
    @staticmethod
    def getTriggerCutMCStr():
        return "(Reconstructed_1__HLT_IsoMu20_v1==1)"
    
    @staticmethod
    def getTriggerCutDataStr():
        return "((Reconstructed_1__HLT_IsoMu20_v2==1)+(Reconstructed_1__HLT_IsoMu20_v3==1))"
        
    @staticmethod
    def getCategoryCutStr(njets,nbtags):
        return "(Reconstructed_1__nSelectedJet=="+str(njets)+")*(Reconstructed_1__nSelectedBJet=="+str(nbtags)+")"
        
    @staticmethod
    def getMTWCutStr():
        return "(SingleTop_1__mtw_beforePz>50.0)"
        
    @staticmethod
    def getMCFiles():
        rootFiles = []
        basedir = "/nfs/user/mkomm/ST13/evaluate25ns"
        match = re.compile("mc[0-9]+.root")
        for f in os.listdir(basedir):
            if match.match(f):
                rootFiles.append(os.path.join(basedirMC,f))
        return rootFiles
        
    @staticmethod
    def getDataFiles():
        rootFiles = []
        basedir = "/nfs/user/mkomm/ST13/evaluate25nsData"
        match = re.compile("data[0-9]+.root")
        for f in os.listdir(basedir):
            if match.match(f):
                rootFiles.append(os.path.join(basedir,f))
        return rootFiles
        
    @staticmethod
    def getResponseFiles():
        rootFiles = []
        basedir = "/nfs/user/mkomm/ST13/response25ns"
        match = re.compile("selected[0-9]+.root")
        for f in os.listdir(basedir):
            if match.match(f):
                rootFiles.append(os.path.join(basedir,f))
        return rootFiles
        
    @staticmethod
    def getEfficiencyFiles():
        rootFiles = []
        basedir = "/nfs/user/mkomm/ST13/response25ns"
        match = re.compile("efficiency[0-9]+.root")
        for f in os.listdir(basedir):
            if match.match(f):
                rootFiles.append(os.path.join(basedir,f))
        return rootFiles
        
            
    @staticmethod
    def getHist1D(hist,fileName,processName,variableName,weight):
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
        
    @staticmethod
    def getHist2D(hist,fileName,processName,variableNameX,variableNameY,weight):
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
        
    @staticmethod
    def normalizeByBinWidth(hist):
        #hist.Scale(1./(hist.GetXaxis().GetXmax()-hist.GetXaxis().GetXmin())/hist.Integral())
        hist.Scale(1./hist.Integral())
        for ibin in range(hist.GetNbinsX()):
            hist.SetBinError(ibin+1,hist.GetBinError(ibin+1)/hist.GetBinWidth(ibin+1))
            hist.SetBinContent(ibin+1,hist.GetBinContent(ibin+1)/hist.GetBinWidth(ibin+1))
            
    @staticmethod
    def normalizeByTransistionProbability(responseMatrix):
        responseMatrixSelectedNormalized = responseMatrix.Clone(responseMatrix.GetName()+"Norm")

        for genBin in range(responseMatrixSelectedNormalized.GetNbinsX()):
            s = 0.0
            for recoBin in range(responseMatrixSelectedNormalized.GetNbinsY()):
                s+=responseMatrixSelectedNormalized.GetBinContent(genBin+1,recoBin+1)
            for recoBin in range(responseMatrixSelectedNormalized.GetNbinsY()):
                c = responseMatrixSelectedNormalized.GetBinContent(genBin+1,recoBin+1)
                responseMatrixSelectedNormalized.SetBinContent(genBin+1,recoBin+1,c/s)
        return responseMatrixSelectedNormalized
            
