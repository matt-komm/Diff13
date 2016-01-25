from Module import Module

import logging
import ROOT
import subprocess
import math

class ThetaFit(Module):
    def __init__(self,options=[]):
        Module.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def run(self,cfgFile="fit.cfg"):
        self._logger.info("run fit model: "+cfgFile)
        p = subprocess.Popen(["theta", cfgFile],
            #shell=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        )
        p.wait()

        for l in p.stdout:
            if l.find("errors:")!=-1:
                self._logger.info("run fit: "+l.replace("\n","").replace(" ",""))
            if l.find("warnings:")!=-1:
                self._logger.info("run fit: "+l.replace("\n","").replace(" ",""))

        
    def readFitResult(self,rootFile="fit.root",modelName="fit"):
        self._logger.info("read fit result: "+rootFile)
        f = ROOT.TFile(rootFile)
        tree = f.Get("products")
        tree.GetEntry(0)
        
        uncertainties = self.module("ThetaModel").getUncertaintsDict()
        
        result = {}
        for sysName in uncertainties.keys():
            result[sysName]={
                "mean":getattr(tree,modelName+"__"+sysName),
                "unc":getattr(tree,modelName+"__"+sysName+"_error")
            }
            
            self._logger.info("Fit: "+sysName+": "+str(round(result[sysName]["mean"],2))+"+-"+str(round(result[sysName]["unc"],2)))
        result["cov"]= getattr(tree,modelName+"__covariance").Clone("covariance")
        result["cov"].SetDirectory(0)
        f.Close()
        return result
        
        
    def plotCorrelations(self,covariance):
        uncertainties = self.module("ThetaModel").getUncertaintsDict()
        
        corr = ROOT.TH2D("correlations","",len(uncertainties.keys()),0,len(uncertainties.keys()),len(uncertainties.keys()),0,len(uncertainties.keys()))
        corr.SetDirectory(0)
        print covariance.GetNbinsX(),covariance.GetNbinsY()
        for ibin in range(len(uncertainties.keys())):
            for jbin in range(len(uncertainties.keys())):
                ijthetaBin = ibin*len(uncertainties.keys())+jbin
                iithetaBin = ibin*len(uncertainties.keys())+ibin
                jjthetaBin = jbin*len(uncertainties.keys())+jbin
                covij = covariance.GetBinContent(ijthetaBin+1,1)
                covii = covariance.GetBinContent(iithetaBin+1,1)
                covjj = covariance.GetBinContent(jjthetaBin+1,1)
                corr.SetBinContent(ibin+1,jbin+1,covij/math.sqrt(covii*covjj))
        
        for ibin in range(len(uncertainties)):
            corr.GetXaxis().SetBinLabel(ibin+1,uncertainties.keys()[ibin])
            corr.GetYaxis().SetBinLabel(ibin+1,uncertainties.keys()[ibin])
        
        corr.SetMarkerSize(1)
        cv = ROOT.TCanvas("cvCorrelations","",800,700)
        corr.Draw("colz text")
        
        cv.Update()
        cv.WaitPrimitive()
        
        
        
        
        
