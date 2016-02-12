#!/usr/bin/python
import os
import subprocess
from optparse import OptionParser
import re
import stat
import shutil

import pxl
import pxl.core
import pxl.modules
import pxl.xml
import pxl.hep


def sum(list):
	ret = 0.0
	for elem in list:
		ret+=elem[0]
	return ret

def getValue(elem1,elem2):
	if elem1["size"]<=elem2["size"]:
		return 1
	else:
		return -1

def shift(list1,list2):
	sum1=sum(list1)
	sum2=sum(list2)
	bestShift = sum1-sum2
	shiftPair = [-1,-1]
	for cnt1 in range(len(list1)):
		for cnt2 in range(len(list2)):
			if (math.fabs(bestShift)>math.fabs((sum1+list2[cnt2]["size"])-(sum2-list2[cnt2]["size"]))):
				shiftPair=[-1,cnt2]
				bestShift=(sum1+list2[cnt2]["size"])-(sum2-list2[cnt2]["size"])
			if (math.fabs(bestShift)>math.fabs((sum1-list1[cnt1]["size"])-(sum2+list1[cnt1]["size"]))):
				shiftPair=[cnt1,-1]
				bestShift=(sum1-list1[cnt1]["size"])-(sum2+list1[cnt1]["size"])
	if (math.fabs(bestShift)<math.fabs(sum1-sum2)):
		if shiftPair[0]==-1:
			elem2=list2.pop(shiftPair[1])
			list1.append(elem2)
		elif shiftPair[1]==-1:
			elem1=list1.pop(shiftPair[0])
			list2.append(elem1)
	else:
		#print "no improvement"
		pass

def swap(list1,list2):
	sum1=sum(list1)
	sum2=sum(list2)
	
	bestSwap = sum1-sum2
	swapPair = [0,0]
	#print "bestSwap",bestSwap
	#print
	for cnt1 in range(len(list1)):
		for cnt2 in range(len(list2)):
			#print bestSwap,(sum1-list1[cnt1][0]+list2[cnt2][0])-(sum2-list2[cnt2][0]+list1[cnt1][0])
			if (math.fabs(bestSwap)>math.fabs((sum1-list1[cnt1]["size"]+list2[cnt2]["size"])-(sum2-list2[cnt2]["size"]+list1[cnt1]["size"]))):
				#print "swap"
				swapPair=[cnt1,cnt2]
				bestSwap=(sum1-list1[cnt1]["size"]+list2[cnt2]["size"])-(sum2-list2[cnt2]["size"]+list1[cnt1]["size"])
	if (math.fabs(bestSwap)<math.fabs(sum1-sum2)):
		#print
		#print "pair",swapPair
		#print "old:",sum1-sum2
		#print "new:",(sum1-list1[swapPair[0]][0]+list2[swapPair[1]][0])-(sum2-list2[swapPair[1]][0]+list1[swapPair[0]][0])
		elem1=list1[swapPair[0]]
		elem2=list2[swapPair[1]]
		
		list2[swapPair[1]]=elem1
		list1[swapPair[0]]=elem2
	else:
		#print "no improvement"
		pass
		
def generatePartions(fileList,N):
    sizedFileList=[]
    for f in fileList:
        sizedFileList.append({"file":f,"size":os.path.getsize(f)})

    sizedFileList.sort(getValue)

    divLists=[]
    retLists=[]
    for target in range(N):
        divLists.append([])
        retLists.append([])

    #fill list in zigzag
    for cnt in range(len(sizedFileList)):
        for target in range(N):
	        if ((cnt/N)%2==0):
		        if ((cnt%N)==target):
			        #print cnt,"-> ",target
			        divLists[target].append(sizedFileList[cnt])
			        break
	        else:
		        if ((cnt%N)==target):
			        #print cnt,"-> ",N-target -1
			        divLists[N-target-1].append(sizedFileList[cnt])
			        break
    
    
    
    for i in range(len(divLists)):
        divList=divLists[i]
        for elem in divList:
            retLists[i].append(elem["file"])
    return retLists
    
def createPartionedFileName(name,N):
    splitted=name.rsplit(".",1)
    filename=splitted[0]+str(N)+"."+splitted[1]
    return filename
    
def findMatches(path,regex):
    dirlevel=path.count(os.path.sep)
    pieces_regex=regex.split(os.path.sep)
    matches=[]
    if len(pieces_regex)==(dirlevel):
        matches.append(path)
    elif not pieces_regex[dirlevel]=="[walk]":
        
        matcher=re.compile(pieces_regex[dirlevel])#.replace("*","[0-9A-Za-z_\-]*"))
        for f in os.listdir(path):
            matchObj = matcher.match(f)
            if matchObj:
                if matchObj.end()!=len(f):
                    continue
                for i in range(dirlevel):
                    print " + ", 
                print f
                matches.extend(findMatches(path+os.path.sep+f,regex))

    else:
        matcher=re.compile(pieces_regex[dirlevel+1])#.replace("*","[0-9A-Za-z_\-]*"))
        for root, dirs, files in os.walk(path,topdown=True):
            for f in files:
                if matcher.match(f):
                    
                    #for i in range(dirlevel):
                    #   print " + ",
                    #print f
                    matches.append(os.path.join(root,f))
        for i in range(dirlevel-1):
            print " + ", 
        print len(matches)
    return matches
                    
    
        

if __name__=="__main__":
    parser = OptionParser()
    parser.add_option("-n", dest="N", default="10")
    parser.add_option("-f", action="store_true", default=False, dest="force")
    parser.add_option("-o", action="store_true", default=False, dest="override")
    parser.add_option("--out", default="", dest="out")
    parser.add_option("--condor", action="store_true", default=False, dest="condor")
    parser.add_option("--addoption", default=None, dest="addoption")
    (options, args) = parser.parse_args()
    N=int(options.N)
    if options.out!="":
        if (os.path.exists(options.out)):
            if options.force and not options.override:
                shutil.rmtree(options.out)
        if not options.override:
            os.mkdir(options.out)
        elif len(os.listdir(options.out))>0:
            print "WARNING: output folder '",options.out,"' contains already ",len(os.listdir(options.out))," files/folders that wont be deleted because override (-o) was specified!"
    outputFolder=options.out
    pluginManager=pxl.core.PluginManager()
    pluginManager.loadPluginsFromDirectory(os.path.join(os.environ['HOME'],".pxl-3.5","plugins"))
    analysisImporter=pxl.xml.AnalysisXmlImport()
    analysisImporter.open(args[0])
    analysis = pxl.modules.Analysis()
    analysisImporter.parseInto(analysis)
    analysisImporter.close()
    moduleList = analysis.getModules()
    fileInputModules=[]
    fileOutputModules=[]
    for m in moduleList:
        opts = m.getOptionDescriptions()
        #print m.getName(), m.getType()
        for op in opts:
            #print option.name
            if op.usage==op.USAGE_FILE_OPEN:
                if m.getType()=="File Input":
                    fileInputModules.append({"module":m,"option":op.name,"files":m.getOption(op.name)})
                else:
                    if type(m.getOption(op.name))==type(str()):
                        m.setOption(op.name,os.path.abspath(m.getOption(op.name)))
                    elif type(m.getOption(op.name))==type(tuple()):
                        ftlist = []
                        for f in m.getOption(op.name):
                            ftlist.append(os.path.abspath(f))
                        m.setOption(op.name,tuple(ftlist))
            elif op.usage==op.USAGE_FILE_SAVE:
                if (m.getType()=="RootTreeWriter") or (m.getType()=="File Output") or op.name=="output":
                    fileOutputModules.append({"module":m,"option":op.name,"file":m.getOption(op.name)})
                else:
                    m.setOption(op.name,os.path.abspath(m.getOption(op.name)))
            elif op.name=="filename" and (m.getType()=="PyModule" or m.getType()=="PyAnalyse"):
                m.setOption(op.name,os.path.abspath(m.getOption(op.name)))
    
    for i,fileInput in enumerate(fileInputModules):
        evaluatedList=[]
        for f in fileInput["files"]:
            flist=[]
            if f.startswith("/"):
                flist = findMatches('/',f)
            else:
                #print os.path.join(os.getcwd(),f)
                flist = findMatches('/',os.path.join(os.getcwd(),f))
            print "found "+str(len(flist))+" files for '"+f+"' in module '"+fileInput["module"].getName()+"'"
            evaluatedList.extend(flist)
        fileInputModules[i]["files"]=evaluatedList
        fileInputModules[i]["files_partioned"]=generatePartions(evaluatedList,N)

    analysisFiles=[]
    for n in range(N):
        for fileInput in fileInputModules:
            fileInput["module"].setOption(fileInput["option"],fileInput["files_partioned"][n])
        for fileOutput in fileOutputModules:
            fileOutput["module"].setOption(fileOutput["option"],createPartionedFileName(fileOutput["file"],n))
        exporters=pxl.xml.AnalysisXmlExport()
        analysisFileName=os.path.join(outputFolder,createPartionedFileName(args[0],n))
        exporters.open(analysisFileName)
        exporters.writeObject(analysis)
        #print "h1"
        exporters.close()
        #print "h2"
        analysisFiles.append(os.path.abspath(analysisFileName))
        #print "h3"
    if (options.condor):
        
            
        preconfig='''executable = run.sh
universe = vanilla
output = log/log$(Process).out
error = log/log$(Process).error
log = /dev/null
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
requirements   = (CMSFARM =?= TRUE)
'''
        if options.addoption!=None:
            f = open(options.addoption)
            for l in f:
                preconfig+=l
            f.close()
            
        shellscript='''#!/bin/bash
number=$RANDOM
let "number %= 100"
echo "sleeping for ... "$number
sleep $number
source /home/fynu/mkomm/.bashrc
cd /home/fynu/mkomm/Diff13
source setVars.sh
export HOME=/home/fynu/mkomm
cd $_CONDOR_SCRATCH_DIR
env
pwd
echo "ls: `ls -l`"
echo "ls stageout: `ls -l /storage/data/cms/store/user/mkomm/DX_13`"
echo "ls storage: `ls -l /nfs/user/mkomm/ST13/selection25ns`"
which pxlrun
echo "executing: "$@
pxlrun -v $@
echo "done executing: "$@
'''
        try:
            os.mkdir(os.path.join(outputFolder,"log"))
        except Exception,e:
            pass
        shellFile=open(os.path.join(outputFolder,"run.sh"),"w")
        shellFile.write(shellscript)
        shellFile.close()
        os.chmod(os.path.join(os.getcwd(),outputFolder,"run.sh"),0744)
        condorConfig=open(os.path.join(outputFolder,"pxlrun.condor"),"w")
        condorConfig.write(preconfig)
        for analysisFile in analysisFiles:
            #condorConfig.write("arguments = --addSearchPath="+os.path.dirname(os.path.abspath(args[0]))+" "+analysisFile+"\n")
            condorConfig.write("arguments = "+analysisFile+"\n")
            condorConfig.write("queue\n")
        condorConfig.close()
