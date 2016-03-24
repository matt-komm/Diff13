import ROOT
import os
import numpy
import math

#f = ROOT.TFile(os.path.join(os.getcwd(),"result/PrefitYields/yields_all.root"))
f = ROOT.TFile(os.path.join(os.getcwd(),"result/nominal/yields_all.root"))
njets = 2
btags = 1
cat = njets*3+btags
yTotal = 0.0
errTotal2 = 0.0
for k in f.GetListOfKeys ():
    ibin = f.Get(k.GetName()).FindBin(cat)
    y = f.Get(k.GetName()).GetBinContent(ibin)
    err = f.Get(k.GetName()).GetBinError(ibin)
    if k.GetName().find("data__data")==-1:
        yTotal+=y
        errTotal2+=err**2
    print k.GetName(),round(y,1),round(err,1)
print "totalMC",round(yTotal,1),round(math.sqrt(errTotal2),1)
