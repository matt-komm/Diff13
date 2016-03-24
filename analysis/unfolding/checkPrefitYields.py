#!/usr/bin/python

import ROOT
import os
import numpy
import math
import csv



def readPrefitYield(f):
    #"category","component","process","entries","integral","unc"
    inFile = open(f,"rb")
    csvFile = csv.DictReader(inFile, dialect='excel', quoting=csv.QUOTE_NONNUMERIC)
    result = {}
    for l in csvFile:
        if l["process"]=="total" and l["category"]=="2j1t":
            result[l["component"]]=l["integral"]
    inFile.close()
    return result

uncertainties = [
    ["Btag","b-tagging efficiency"],
    ["Ltag","mis-tagging efficiency"],
    ["PU","pileup reweighting"],
    ["Iso","multijet isolation region"],
    ["En","jet energy scale"],
    ["Res","jet resolution"],
    #["UnclEn","unclustered energy"],
    ["QScaleTChannel","\\tch Q scale"], 
    ["QScaleTTbar","\\ttbar Q scale"],
    ["QScaleTW","\\tw Q scale"],
    ["TopMass","top quark mass"],
    ["Muon","muon selection"],
    ["PDF","PDF"]
]

nominal = readPrefitYield("result/nominal/fit_prefitYields.csv")
for comp in ["tChannel","TopBkg","WZjets","QCD_2j1t"]:
    print comp
    print "--------------------"
    for unc in uncertainties:
        sysUp = readPrefitYield("result/"+unc[0]+"Up/fit_prefitYields.csv")
        sysDown = readPrefitYield("result/"+unc[0]+"Down/fit_prefitYields.csv")
        
        upShift = 100.*sysUp[comp]/nominal[comp]-100.
        downShift = 100.*sysDown[comp]/nominal[comp]-100.
        
        print "%17s: %+6.2f %+6.2f" % (unc[0],upShift,downShift),
        
        if upShift*downShift>0.1:
            print " <= shift in same direction"
        elif downShift!=0 and math.fabs((math.fabs(upShift/downShift)-1.0))>0.2:
            print " <= asymmetric shifts"
        else:
            print
    print
