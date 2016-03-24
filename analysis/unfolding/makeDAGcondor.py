
import os

def makeCondorCfg(name,jobOptions=[]):
    f = open(name+".condor","w")
    f.write('''
executable = run.sh
universe = vanilla
output = log/log_'''+name+'''$(Process).out
error = log/log_'''+name+'''$(Process).error
log = /dev/null
should_transfer_files = YES
transfer_output_files = result
when_to_transfer_output = ON_EXIT
requirements   = (CMSFARM =?= TRUE)
    ''')
    
    for jobOption in jobOptions:
        f.write('''
arguments = \"'/home/fynu/mkomm/Diff13/analysis/unfolding/driver.py' '''+jobOption+'''\"
queue
        ''')
    f.close()

sysModules = [
    "'-m Herwig'",
    "'-m Powheg'",
    "'-m AMC5FS'",
    "'-m NoTopPt'",
    
    "'-m NoBB'",
    
    "'-m TopMassDown'",
    "'-m TopMassUp'",
    "'-m qscaleTChannelDown'",
    "'-m qscaleTChannelUp'",
    "'-m qscaleTTbarDown'",
    "'-m qscaleTTbarUp'",
    "'-m qscaleTWDown'",
    "'-m qscaleTWUp'",
    "'-m qscaleWjetsDown'",
    "'-m qscaleWjetsUp'",
    "'-m btagDown'",
    "'-m btagUp'",
    "'-m ltagDown'",
    "'-m ltagUp'",
    "'-m EnDown'",
    "'-m EnUp'",
    "'-m ResDown'",
    "'-m ResUp'",
    "'-m puDown'",
    "'-m puUp'",
    "'-m isoDown'",
    "'-m isoUp'",
    "'-m pdfDown'",
    "'-m pdfUp'",
    "'-m muonDown'",
    "'-m muonUp'",
    "'-m DYUp'",
    "'-m DYDown'",
    "'-m TWUp'",
    "'-m TWDown'",    


    #"'-m YieldUp' '-c scalefactor:QCD_2j1t'",
    #"'-m YieldDown' '-c scalefactor:QCD_2j1t'",
    
    #"'-m YieldUp' '-c scalefactor:TopBkg'",
    #"'-m YieldDown' '-c scalefactor:TopBkg'",
    
    #"'-m YieldUp' '-c scalefactor:WZjets'",
    #"'-m YieldDown' '-c scalefactor:WZjets'",
    
    #"'-m YieldUp' '-c scalefactor:tChannel'",
    #"'-m YieldDown' '-c scalefactor:tChannel'",
]

jobOptions=[]


jobOptions.append("'-m FitOnly' '-c fit:incl'")


for ipt in range(8):
    jobOptions.append("'-m FitOnly' '-c fit:pt"+str(ipt)+"'")
for iy in range(8):
    jobOptions.append("'-m FitOnly' '-c fit:y"+str(iy)+"'")



for sysModule in sysModules:
    jobOptions.append(sysModule+" '-m FitOnly' '-c fit:incl'")


    for ipt in range(8):
        jobOptions.append(sysModule+"  '-m FitOnly' '-c fit:pt"+str(ipt)+"'")
    for iy in range(8):
        jobOptions.append(sysModule+"  '-m FitOnly' '-c fit:y"+str(iy)+"'")

    
makeCondorCfg("all_fits",jobOptions)


jobOptionsUnfolding = [""]
for sysModule in sysModules:
    jobOptionsUnfolding.append(sysModule)
makeCondorCfg("unfolding",jobOptionsUnfolding)



