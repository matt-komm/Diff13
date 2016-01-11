#!/usr/bin/python

import ROOT
import math
import random
import numpy

ROOT.gSystem.Load("libpyTool.so")

fit = ROOT.PyFit.MLFit()
obs = ROOT.PyFit.Observable()
par = ROOT.PyFit.Parameter("test")

data = ROOT.TH1D("data","",1,-1,1)
data.Fill(0,100)
pred = ROOT.TH1D("pred","",1,-1,1)
pred.Fill(0,1)

comp = ROOT.PyFit.ConstShapeComponent(pred)
comp.addSFParameter(par)
obs.addComponent(comp)
fit.addObservable(obs,data)
fit.addParameter(par)

xvalues = numpy.linspace(50,150,50)
yvalues = numpy.zeros(len(xvalues))
for i,x in enumerate(xvalues):
    par.setScaleFactor(x)
    yvalues[i]=-math.exp(-fit.globalNll())
    #print yvalues[i]
graph = ROOT.TGraph(len(xvalues),xvalues,yvalues)
graph.Draw("APL")
ROOT.gPad.WaitPrimitive()

#par.setScaleFactor(100.0)
fit.minimize()

#print obs,par,comp

#obs.getPrediction(parameters).getLikelihood(dataHist)
