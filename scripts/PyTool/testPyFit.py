#!/usr/bin/python

import ROOT
import math
import random
import numpy

ROOT.gSystem.Load("libpyTool.so")

fit = ROOT.PyFit.Fit()
obs = ROOT.PyFit.Observable()
par = ROOT.PyFit.Parameter("test")

data = ROOT.TH1D("data","",10,-1,1)
for i in range(1000):
    data.Fill(random.gauss(1,0.2))
pred = data.Clone("prediction")

for i in range(2):
    comp = ROOT.PyFit.ConstShapeComponent(pred)
    comp.addSFParameter(par)
    obs.addComponent(comp)


for sf in numpy.linspace(0.1,2,20):
    par.setScaleFactor(sf)
    print sf,obs.nll(data)
    
fit.minimize()

#print obs,par,comp

#obs.getPrediction(parameters).getLikelihood(dataHist)
