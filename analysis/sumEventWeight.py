import os

sampleDict={}

for f in os.listdir(os.path.join(os.getcwd(),"collectNumberOfEvents")):
    if f.startswith("events"):
        t=open(os.path.join(os.getcwd(),"collectNumberOfEvents",f))
        for line in t:
            s=line.replace("\n","").replace("\r","").split(",")
            sampleName=s[0]
            positive=int(s[1])
            negative=int(s[2])
            total=int(s[3])
            if (positive+negative)!=total:
                print "Error: ",f
            if not sampleDict.has_key(sampleName):
                sampleDict[sampleName]={"t":0,"n":0,"p":0}
            sampleDict[sampleName]["t"]+=total
            sampleDict[sampleName]["n"]+=negative
            sampleDict[sampleName]["p"]+=positive
            
for key in sorted(sampleDict.keys()):
    print key,"total="+str(sampleDict[key]["t"]),"eff="+str(sampleDict[key]["p"])+"-"+str(sampleDict[key]["n"])+"="+str(sampleDict[key]["p"]-sampleDict[key]["n"])
