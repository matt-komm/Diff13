import ROOT
import os
import numpy
import math
import csv

components = [
"tChannel",
"TopBkg",
"WZjets",
"QCD_2j1t"
]

def getTotalUnc(uncList):
    NTOYS=5000
    toys = numpy.zeros(NTOYS)
    for itoy in range(NTOYS):
        for unc in uncList:
            up = unc[0]
            down = unc[1]

            diced = numpy.random.normal()
            #print up,down
            if diced<0:
                diced=math.fabs(diced)*down
            else:
                diced=math.fabs(diced)*up
            toys[itoy]+=diced
    return numpy.percentile(toys,[15.865,50.0,84.135])
            

def parseFitResult(fileName,sys):
    '''
    inFile = open(fileName,"rb")
    csvFile = csv.DictReader(inFile, dialect='excel', quoting=csv.QUOTE_NONNUMERIC)
    result = {}
    for l in csvFile:
        result[l["sys"]]={"mean":l["mean"],"unc":l["unc"]}
    inFile.close()
    return result
    '''
    f = ROOT.TFile(fileName)
    tree = f.Get("products")
    tree.GetEntry(0)
    result = {}
    for comp in components:
        result[comp]={"mean":getattr(tree,"fit__"+comp),"unc":getattr(tree,"fit__"+comp+"_error")}
    f.Close()
    return result
    
nominalFit = parseFitResult("result/nominal/fit.root",sys="nominal")
nominalMean = nominalFit["tChannel"]["mean"]
nominalUnc = nominalFit["tChannel"]["unc"]


noBBFit = parseFitResult("result/NoBB/fit.root","noBB")
noBBMean = noBBFit["tChannel"]["mean"]
noBBUnc = noBBFit["tChannel"]["unc"]


systematics = [
    #["QCD_2j1tYield","multijet normalization"],
    #["TopBkgYield","\\tw, \\ttbar background normalization"],
    #["WZjetsYield","\\wjets, \\zjets background normalization"],
    ["Btag","b-tagging efficiency"],
    ["Ltag","mis-tagging efficiency"],
    ["PU","pileup reweighting"],
    ["Iso","multijet isolation region"],
    ["En","jet energy scale"],
    ["Res","jet resolution"],
    #["UnclEn","unclustered energy"],
    ["QScaleTChannel","\\tch Q scale"], 
    ["QScaleTTbar","\\ttbar Q scale"],
    #["TopMass","top quark mass"]
]

sysFits={}

for systematic in systematics:
    sysFits[systematic[0]]={}
    for shift in ["Up","Down"]:
        fitResult = parseFitResult("result/"+systematic[0]+shift+"/fit.root",sys=systematic[0])
        sysFits[systematic[0]][shift]=fitResult
        
        for par in fitResult.keys():
            sysFits[systematic[0]][shift][par]["shift"]=fitResult[par]["mean"]-nominalMean



### print yield table ###
totalSysList = []

print "%30s & %7s & %7s \\\\\\hline" % ("systematic source","up shift","down shift")
print "%30s & $%+6.1f\\%%$ & $%+6.1f\\%%$ \\\\" % ("statistical",100.*noBBUnc,-100.*noBBUnc)
print "%30s & $%+6.1f\\%%$ & $%+6.1f\\%%$ \\\\" % ("limited MC statistics",100.*math.sqrt(nominalUnc**2-noBBUnc**2),-100.*math.sqrt(nominalUnc**2-noBBUnc**2))

for systematic in systematics:
    upShift = sysFits[systematic[0]]["Up"]["tChannel"]["shift"]
    downShift = sysFits[systematic[0]]["Down"]["tChannel"]["shift"]
    totalSysList.append([upShift,downShift])
    print "%30s & $%+6.1f\\%%$ & $%+6.1f\\%%$ \\\\" % (systematic[1],100.*upShift,100.*downShift)
totalSysList.append([0.027,-0.027])
print "%30s & $%+6.1f\\%%$ & $%+6.1f\\%%$ \\\\" % ("luminosity",2.7,-2.7)
totalSysUnc = getTotalUnc(totalSysList)
print "%30s & $%+6.1f\\%%$ & $%+6.1f\\%%$ \\\\" % ("total systematic",100.*totalSysUnc[2],100.*totalSysUnc[0])
totalSysList.append([nominalUnc,-nominalUnc])
print "\\hline"
totalUnc = getTotalUnc(totalSysList)
print "%30s & $%+6.1f\\%%$ & $%+6.1f\\%%$ \\\\" % ("total uncertainty",100.*totalSysUnc[2],100.*totalSysUnc[0])

print
print

### print fit table ###   
print "%39s"%("nominal"),
for par in ["tChannel","TopBkg","WZjets","QCD_2j1t"]:
    mean = nominalFit[par]["mean"]
    unc = math.fabs(nominalFit[par]["unc"])
    print "&","$%3.2f\\pm%3.2f$"%(mean,unc),
print "\\\\"

print "%39s"%("nominal (w/o limited MC)"),
for par in ["tChannel","TopBkg","WZjets","QCD_2j1t"]:
    mean = noBBFit[par]["mean"]
    unc = math.fabs(noBBFit[par]["unc"])
    print "&","$%3.2f\\pm%3.2f$"%(mean,unc),
print "\\\\"

print "\\hline"

for systematic in systematics:
    for shift in ["Up","Down"]:
        print "%30s %8s"%(systematic[1],shift),
        for par in ["tChannel","TopBkg","WZjets","QCD_2j1t"]:
            mean = sysFits[systematic[0]][shift][par]["mean"]
            unc = math.fabs(sysFits[systematic[0]][shift][par]["unc"])
            print "&","$%3.2f\\pm%3.2f$"%(mean,unc),

        print "\\\\"
    print "\\hline"

