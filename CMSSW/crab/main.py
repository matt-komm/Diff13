from CRABAPI.RawCommand import crabCommand
from httplib import HTTPException
from CRABClient.UserUtilities import config

from optparse import OptionParser


def submit(config,*args,**kwargs):
    try:
        crabCommand('submit', config = config)
    except HTTPException, hte:
        print hte.headers
        
def status(task,*args,**kwargs):
    try:
        crabCommand('status', task)
    except HTTPException, hte:
        print hte.headers
        
def kill(task,*args,**kwargs):
    try:
        crabCommand('kill', task)
    except HTTPException, hte:
        print hte.headers

def getlog(task,*args,**kwargs):
    try:
        crabCommand('getlog', task)
    except HTTPException, hte:
        print hte.headers        

if __name__=="__main__":

    parser = OptionParser()
    parser.add_option("--datasetID", dest="datasetID",
                      help="dataset")

    (options, args) = parser.parse_args()

    config = config()

    config.General.requestName = 'ST_EA_13'
    #config.General.workArea = 'crab/TToLeptons_t-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola'
    config.General.transferOutputs = True
    config.General.transferLogs = False

    config.JobType.pluginName = 'Analysis'
    config.JobType.psetName = 'EDM2PXLIO/phys14.py'
    config.JobType.pyCfgParams = []
    config.JobType.outputFiles = ["output.pxlio"]

    #config.Data.inputDataset = '/TToLeptons_t-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM'
    config.Data.inputDBS = 'global'
    config.Data.splitting = 'FileBased'
    config.Data.unitsPerJob = 5
    #config.Data.outLFN = '/store/user/mkomm'
    config.Data.publication = False


    config.Site.storageSite = "T2_BE_UCL"
    config.Site.whitelist = ['T2_CH_CERN','T2_DE_DESY','T2_BE_IIHE','T2_BE_UCL','T2_IT_Legnaro','T2_IT_Rome']
    
    datasets = [
        #"/TToLeptons_t-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM",
        #"/TBarToLeptons_t-channel_Tune4C_CSA14_13TeV-aMCatNLO-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM",
        
        #"/T_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM",
        #"/Tbar_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM",
        
        #"/TToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM",
        #"/TBarToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM",
        
        "/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM",
        
        #"/WJetsToLNu_13TeV-madgraph-pythia8-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM",
        
        #"/WJetsToLNu_HT-100to200_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM",
        #"/WJetsToLNu_HT-200to400_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM",
        #"/WJetsToLNu_HT-400to600_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM",
        #"/WJetsToLNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM",
        
        #"/DYJetsToLL_M-50_13TeV-madgraph-pythia8/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM",
        
        #"/QCD_Pt-20toInf_MuEnrichedPt15_PionKaonDecay_Tune4C_13TeV_pythia8/Phys14DR-PU20bx25_PHYS14_25_V1-v3/MINIAODSIM",
        #"/QCD_Pt_20to30_bcToE_Tune4C_13TeV_pythia8/Phys14DR-PU20bx25_PHYS14_25_V1-v2/MINIAODSIM",
        #"/QCD_Pt_30to80_bcToE_Tune4C_13TeV_pythia8/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM",
        #"/QCD_Pt_80to170_bcToE_Tune4C_13TeV_pythia8/Phys14DR-PU20bx25_PHYS14_25_V1-v2/MINIAODSIM",
        #"/QCD_Pt_170toInf_bcToE_Tune4C_13TeV_pythia8/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM",
        #"/QCD_Pt-20to30_EMEnriched_Tune4C_13TeV_pythia8/Phys14DR-PU20bx25_castor_PHYS14_25_V1-v2/MINIAODSIM",
        #"/QCD_Pt-30to80_EMEnriched_Tune4C_13TeV_pythia8/Phys14DR-PU20bx25_castor_PHYS14_25_V1-v1/MINIAODSIM",
        #"/QCD_Pt-80to170_EMEnriched_Tune4C_13TeV_pythia8/Phys14DR-PU20bx25_castor_PHYS14_25_V1-v1/MINIAODSIM",
    ]
    for dataset in datasets:
    	jobName = dataset.split("/")[1]+'_v4'
   	print "crab/"+jobName+"/crab_"+config.General.requestName
    	status("crab/"+jobName+"/crab_"+config.General.requestName)
    #print "submitting... ",jobName
    #config.General.workArea = "crab/"+jobName
    #config.Data.inputDataset=dataset
    #config.JobType.pyCfgParams=['processName='+jobName]
    #config.Data.outLFN='/store/user/mkomm/'+config.General.requestName+"/"+jobName
    #submit(config)

