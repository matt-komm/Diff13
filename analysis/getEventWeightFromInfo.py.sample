#!/usr/bin/python

import os
import math
import re
import ROOT
from optparse import OptionParser


parser = OptionParser()
(options, args) = parser.parse_args()

baseFolder = "/storage/data/cms/store/user/mkomm/DX_13"
print "looking for files: ",baseFolder+"/*"+args[0]+"*"

rootFiles = {}
for folder in os.listdir(baseFolder):
    if folder.find(args[0])!=-1:
        #print "searching in ...",folder
        for dirpath, dirnames, filenames in os.walk(os.path.join(baseFolder,folder)):
            for f in filenames:
                if f.find("info")!=-1:
                    if not rootFiles.has_key(folder):
                        rootFiles[folder]=[]
                    rootFiles[folder].append(os.path.join(dirpath,f))
                    
for folder in rootFiles.keys():
    print "found ",len(rootFiles[folder])," in ",folder



for folder in sorted(rootFiles.keys()):
    print "reading ... ",folder
    
    nEvents = 0
    sumPosWeight = 0
    sumNegWeight = 0
    for f in rootFiles[folder]:
        rootFile = ROOT.TFile(f)
        hist = rootFile.Get("eventAndPuInfo/genweight")
        wSum = math.fabs(hist.GetBinContent(1))+math.fabs(hist.GetBinContent(2))
        eSum = hist.GetEntries()
        w = eSum/wSum
        nEvents+=eSum
        sumPosWeight +=round(math.fabs(hist.GetBinContent(2))*w)
        sumNegWeight +=round(math.fabs(hist.GetBinContent(1))*w)

    print "\tentries = ",nEvents
    print "\tpos = ",sumPosWeight
    print "\tneg = ",sumNegWeight
    print "\teff = ",sumPosWeight-sumNegWeight

