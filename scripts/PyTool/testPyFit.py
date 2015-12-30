#!/usr/bin/python

import ROOT
import math
import random

ROOT.gSystem.Load("libpyTool.so")

fit = ROOT.PyFit()
obs = ROOT.PyFit.Observable()
par = ROOT.PyFit.Parameter("test")
comp = ROOT.PyFit.ConstShapeComponent(ROOT.TH1D())
comp.addSFParameter(par)

print obs,par,comp

#obs.getPrediction(parameters).getLikelihood(dataHist)
