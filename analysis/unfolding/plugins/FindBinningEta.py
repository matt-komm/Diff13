import ROOT
import math
import numpy
import random
import os

from defaultModules import Module

import logging

class ResponseMatrixYFineBinned(Module.getClass("ResponseMatrixY")):
    def __init__(self,options=[]):
        ResponseMatrixYFineBinned.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getGenBinning(self):
        return numpy.linspace(0.0,2.4,5000)

class FindBinningY(Module.getClass("Program")):
    def __init__(self,options=[]):
        FindBinningY.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def execute(self):
        responseMatrix = self.module("ResponseMatrixY").loadResponseMatrix()
        if responseMatrix==None:
            responseMatrix = self.module("ResponseMatrixY").getResponseMatrix()
            self.module("ResponseMatrixY").saveResponseMatrix(responseMatrix)
        #self.module("Drawing").drawResponseMatrix(responseMatrix,"top quark pT","responsePt")
        #responseMatrix.Scale(0.7)


        genHist = responseMatrix.ProjectionX()
        genBinning = self.module("ResponseMatrixY").getGenBinning()
        '''
        for i in range(genHist.GetNbinsX()):
            genHist.SetBinContent(i+1,1.0)
        '''
        genHist.SetBinContent(0,0)
        genHist.SetBinContent(genHist.GetNbinsX()+1,0)
        
        N=5
        r=genHist.GetXaxis().GetXmax()
        mean = genHist.Integral()/(1.0*N)
        
        f = 0.0
       
        ranges = [0.0]
        
        
        xvalues = []
        yvalues = []
        s = 0.0
        
        def fct(x):
            return mean+mean*f-x/r*mean*f*2
        
        for ibin in range(genHist.GetNbinsX()):
            s+=genHist.GetBinContent(ibin+1)
            center = genHist.GetBinCenter(ibin+1)
            x = 0.5*(ranges[-1]+center)
            if s>=fct(x):
                ranges.append(center)
                xvalues.append(0.5*(ranges[-1]+ranges[-2]))
                yvalues.append(s)
                s=s-fct(x)
                #s=0
        if len(xvalues)<N:
            ranges.append(2.4)
            xvalues.append(0.5*(ranges[-1]+ranges[-2]))
            yvalues.append(s)

        
        cv = ROOT.TCanvas("cv","",800,700)
        g = ROOT.TGraph(len(xvalues),numpy.array(xvalues),numpy.array(yvalues))
        g.SetMarkerStyle(20)
        g.SetMarkerSize(1)
        g.Draw("APL")
        #tf = ROOT.TF1("tf",str(mean)+"+"+str(mean*f)+"-x/"+str(r)+"*"+str(2*mean*f),0,300)
        #tf.Draw("SameL")
        cv.Print("binning_y.pdf")
            
        print ranges
