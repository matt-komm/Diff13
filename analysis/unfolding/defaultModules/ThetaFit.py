from Module import Module

import logging
import ROOT
import subprocess
import math
import os
import csv
import sys

class ThetaFit(Module):
    def __init__(self,options=[]):
        Module.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        self.fitResult = None
        
    def run(self,modelName="fit"):
        fullPath = os.path.join(self.module("Utils").getOutputFolder(),modelName+".cfg")
        self._logger.info("run fit model: "+fullPath)
        p = subprocess.Popen(["theta", fullPath],
            #shell=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        )
        while True:
            nextline = p.stdout.readline()
            if nextline == '' and p.poll() != None:
                break
            #self._logger.debug(nextline.replace("\n","").replace("\r",""))
            '''
            if nextline.find("errors:")!=-1:
                self._logger.info("run fit: "+nextline.replace("\n","").replace("\r","").replace(" ",""))
            if nextline.find("warnings:")!=-1:
                self._logger.info("run fit: "+nextline.replace("\n","").replace("\r","").replace(" ",""))
            '''
    def checkFitResult(self,modelName="fit",fileName="fit"):
        fullPath = os.path.join(self.module("Utils").getOutputFolder(),fileName+".root")
        if os.path.exists(fullPath):
            self._logger.info("fit result already exists: "+fullPath)
            return self.module("ThetaFit").readFitResult(modelName,fileName)
        return None
        
    def printFitResult(self,result):
        
        uncertainties = self.module("ThetaModel").getUncertaintsDict()
        for sysName in uncertainties.keys():
            self._logger.info("Fit: "+sysName+": "+str(round(result[sysName]["mean"],2))+"+-"+str(round(result[sysName]["unc"],2)))
        self._logger.info("Fit: NLL: %+6.4e"%(result["nll"]))
        
    def readFitResult(self,modelName="fit",fileName="fit"):
        fullPath = os.path.join(self.module("Utils").getOutputFolder(),fileName+".root")
        f = ROOT.TFile(fullPath)
        tree = f.Get("products")
        if tree==None or tree.GetEntries()==0:
            raise Exception("No fit result produced")
        tree.GetEntry(0)
        
        uncertainties = self.module("ThetaModel").getUncertaintsDict()
        
        result = {}
        for sysName in uncertainties.keys():
            if (not hasattr(tree,modelName+"__"+sysName)) or (not hasattr(tree,modelName+"__"+sysName+"_error")):
                raise Exception("No fitted values found for '"+modelName+"__"+sysName+"'")
            result[sysName]={
                "mean":getattr(tree,modelName+"__"+sysName),
                "unc":getattr(tree,modelName+"__"+sysName+"_error")
            }
            
        if (not hasattr(tree,modelName+"__covariance")):
            raise Exception("No fitted covariance matrix found for '"+modelName+"__covariance'")

        cov = getattr(tree,modelName+"__covariance")
        
        covariances = ROOT.TH2D("covariance","",len(uncertainties.keys()),0,len(uncertainties.keys()),len(uncertainties.keys()),0,len(uncertainties.keys()))
        covariances.SetDirectory(0)
        correlations = ROOT.TH2D("correlations","",len(uncertainties.keys()),0,len(uncertainties.keys()),len(uncertainties.keys()),0,len(uncertainties.keys()))
        correlations.SetDirectory(0)
        for ibin in range(len(uncertainties.keys())):
            for jbin in range(len(uncertainties.keys())):
                ijthetaBin = ibin*len(uncertainties.keys())+jbin
                iithetaBin = ibin*len(uncertainties.keys())+ibin
                jjthetaBin = jbin*len(uncertainties.keys())+jbin
                covij = cov.GetBinContent(ijthetaBin+1,1)
                covii = cov.GetBinContent(iithetaBin+1,1)
                covjj = cov.GetBinContent(jjthetaBin+1,1)
                if covii>0 and covjj>0:
                    correlations.SetBinContent(ibin+1,jbin+1,covij/math.sqrt(covii*covjj))
                else:
                    correlations.SetBinContent(ibin+1,jbin+1,0.0)
                covariances.SetBinContent(ibin+1,jbin+1,covij)
        for ibin in range(len(uncertainties)):
            covariances.GetXaxis().SetBinLabel(ibin+1,uncertainties.keys()[ibin])
            covariances.GetYaxis().SetBinLabel(ibin+1,uncertainties.keys()[ibin])
            correlations.GetXaxis().SetBinLabel(ibin+1,uncertainties.keys()[ibin])
            correlations.GetYaxis().SetBinLabel(ibin+1,uncertainties.keys()[ibin])
        result["covariances"]=covariances
        result["correlations"]=correlations
        
        if (not hasattr(tree,modelName+"__nll")):
            raise Exception("No NLL value found for '"+modelName+"__nll'")
        result["nll"]=getattr(tree,modelName+"__nll")

        f.Close()
        
        return result
        
   
        
        
        
