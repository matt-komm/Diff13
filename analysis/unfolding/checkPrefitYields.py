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
        if not result.has_key(l["component"]):
            result[l["component"]]={}
        if l["process"]=="total":
            result[l["component"]][l["category"]]={"mean":l["integral"],"unc":l["unc"]}
    inFile.close()
    return result

uncertainties = [
    ["DY","\\zjets yield"],
    ["TW","\\tw yield"],
    ["Btag","b-tagging efficiency"],
    ["Ltag","mis-tagging efficiency"],
    ["PU","pileup reweighting"],
    ["Iso","multijet isolation region"],
    ["En","jet energy scale"],
    ["Res","jet resolution"],
    ["Muon","muon selection"],
    ["PDF","PDF"],
    #["UnclEn","unclustered energy"],
    ["QScaleTChannel","\\tch Q scale"], 
    ["QScaleTTbar","\\ttbar Q scale"],
    ["QScaleTW","\\tw Q scale"],
    ["QScaleWjets","\\wjets Q scale"],
    ["TopMass","top quark mass"]
]

uncertaintiesSpecial = [
    ["NoTopPt","top quark \\pt reweighting"],
    ["Powheg","signal modeling"],
    ["Herwig","hadronization modeling"]
]

nominal = readPrefitYield("result/nominal/fit_prefitYields.csv")

sysUp={}
sysDown={}
for unc in uncertainties:
    sysUp[unc[0]] = readPrefitYield("result/"+unc[0]+"Up/fit_prefitYields.csv")
    sysDown[unc[0]] = readPrefitYield("result/"+unc[0]+"Down/fit_prefitYields.csv")

for unc in uncertaintiesSpecial:
    sysUp[unc[0]] = readPrefitYield("result/"+unc[0]+"/fit_prefitYields.csv")
    sysDown[unc[0]] = nominal


for comp in ["tChannel","TopBkg","WZjets","QCD"]:
    print comp
    print "%17s: " % ("nominal"),
    for cat in ["2j1t","3j1t","3j2t"]:
        if comp=="QCD":
            if nominal[comp+"_"+cat].has_key(cat):
                print "%6.0f+-%5.0f   "%(nominal[comp+"_"+cat][cat]["mean"],nominal[comp+"_"+cat][cat]["unc"]),
            else:
                print "%6s %6s   "%("",""),
        else:
            if nominal[comp].has_key(cat):
                print "%6.0f+-%5.0f   "%(nominal[comp][cat]["mean"],nominal[comp][cat]["unc"]),
            else:
                print "%6s %6s   "%("",""),
    print
    print "--------------------"
    for unc in uncertainties+uncertaintiesSpecial:
        print "%17s: "%(unc[0]),
        for cat in ["2j1t","3j1t","3j2t"]:
            if comp=="QCD":
                upShift = sysUp[unc[0]][comp+"_"+cat]
                downShift = sysDown[unc[0]][comp+"_"+cat]
            
                if upShift.has_key(cat):
                    print "%+6.2f %+6.2f   "%(upShift[cat]["mean"]/nominal[comp+"_"+cat][cat]["mean"]*100.-100.,downShift[cat]["mean"]/nominal[comp+"_"+cat][cat]["mean"]*100.-100.),
                else:
                    print "%6s %6s   "%("",""),
            else:
                upShift = sysUp[unc[0]][comp]
                downShift = sysDown[unc[0]][comp]
            
                if upShift.has_key(cat):
                    print "%+6.2f %+6.2f   "%(upShift[cat]["mean"]/nominal[comp][cat]["mean"]*100.-100.,downShift[cat]["mean"]/nominal[comp][cat]["mean"]*100.-100.),
                else:
                    print "%6s %6s   "%("",""),
        print
        '''
        if upShift*downShift>0.1:
            print " <= shift in same direction"
        elif downShift!=0 and math.fabs((math.fabs(upShift/downShift)-1.0))>0.2:
            print " <= asymmetric shifts"
        else:
            print
        '''
        
    print
