#!/usr/bin/python

from optparse import OptionParser
import json
from itertools import groupby
from operator import itemgetter

parser = OptionParser()
parser.add_option("-i", dest="files",action="append",default=[],
                  help="input files")
parser.add_option("-o", 
                  dest="outfile", default="overlap.json",
                  help="output file name")

(options, args) = parser.parse_args()

runlumidict = {}

def expandLumi(d):
    ret = {}
    for run in d:
        for lumiRange in d[run]:
            begin = lumiRange[0]
            end = lumiRange[1]
            for lumi in range(begin,end+1):
                if not ret.has_key(run):
                    ret[run]=[]
                if lumi not in ret[run]:
                    ret[run].append(lumi)
    for run in ret:
        ret[run].sort()
    return ret
    
def countLumis(d):
    n = 0
    for run in d:
        n+=len(d[run])
    return n

def parseJSON(file):
    f = open(file)
    content = ""
    for l in f:
        content+=l.replace("\r","").replace("\n","")
    return json.loads(content)
        
for i,f in enumerate(options.files):
    print "parsing ",f,"...",
    d = expandLumi(parseJSON(f))
    print "lumis=",countLumis(d)
    
    if i==0:
        runlumidict=d
    else:
        runoverlap = set(runlumidict.keys()).intersection(d.keys())
        for run in runlumidict.keys():
            if run not in runoverlap:
                del runlumidict[run]
                
        for run in runlumidict.keys():
            runlumidict[run]=list(set(runlumidict[run]).intersection(d[run]))
                
        
        
        
        
expandedLumis = countLumis(runlumidict)

result={}

for run in runlumidict:
    result[run]=[]
    runlumidict[run].sort()
    
    for k, g in groupby(enumerate(runlumidict[run]), lambda (i, x): i-x):
        m= map(itemgetter(1), g)
        result[run].append([m[0],m[-1]])
    #print run,result[run]
            
reexpandedLumis = countLumis(expandLumi(result))
if expandedLumis!=reexpandedLumis:
    print "Error during slimming lumi blocks"
    
print "final lumis = ",expandedLumis

f = open(options.outfile,"w")
f.write(json.dumps(result))
f.close()

    
    
    
    
