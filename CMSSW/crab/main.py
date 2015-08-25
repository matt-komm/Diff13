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
        crabCommand('status', task,'--verboseErrors')
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
    config.General.transferOutputs = True
    config.General.transferLogs = False

    config.JobType.pluginName = 'Analysis'
    config.JobType.psetName = 'EDM2PXLIO/stea.py'
    config.JobType.pyCfgParams = []
    config.JobType.outputFiles = ["output.pxlio"]
    config.JobType.inputFiles=[
        "Summer15_50nsV4_DATA.db",
        "Summer15_50nsV4_MC.db",
        #"Summer15_50nsV4_UncertaintySources_AK4PFchs.txt"
    ]

    config.Data.inputDBS = 'global'
    config.Data.splitting = 'FileBased'
    #config.Data.splitting = 'LumiBased'
    #config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/DCSOnly/json_DCSONLY_Run2015B.txt'
    #config.Data.runRange='251162-251883'
    config.Data.unitsPerJob = 3
    config.Data.publication = False



    config.Site.storageSite = "T2_BE_UCL"
    config.Site.whitelist = ['T2_CH_CERN','T2_DE_DESY','T2_BE_IIHE','T2_BE_UCL','T2_IT_Legnaro','T2_IT_Rome', 'T2_US_UCSD']
    
    datasetsPHYS14 = [
        "/TToLeptons_t-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM",
        "/TBarToLeptons_t-channel_Tune4C_CSA14_13TeV-aMCatNLO-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM",
        
        "/T_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM",
        "/Tbar_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM",
        
        "/TToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM",
        "/TBarToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM",
        
        "/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM",
        
        "/WJetsToLNu_13TeV-madgraph-pythia8-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM",
        
        #"/WJetsToLNu_HT-100to200_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM",
        #"/WJetsToLNu_HT-200to400_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM",
        #"/WJetsToLNu_HT-400to600_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM",
        #"/WJetsToLNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM",
        
        "/DYJetsToLL_M-50_13TeV-madgraph-pythia8/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM",
        
        "/QCD_Pt-20toInf_MuEnrichedPt15_PionKaonDecay_Tune4C_13TeV_pythia8/Phys14DR-PU20bx25_PHYS14_25_V1-v3/MINIAODSIM",
        "/QCD_Pt_20to30_bcToE_Tune4C_13TeV_pythia8/Phys14DR-PU20bx25_PHYS14_25_V1-v2/MINIAODSIM",
        "/QCD_Pt_30to80_bcToE_Tune4C_13TeV_pythia8/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM",
        "/QCD_Pt_80to170_bcToE_Tune4C_13TeV_pythia8/Phys14DR-PU20bx25_PHYS14_25_V1-v2/MINIAODSIM",
        "/QCD_Pt_170toInf_bcToE_Tune4C_13TeV_pythia8/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM",
        "/QCD_Pt-20to30_EMEnriched_Tune4C_13TeV_pythia8/Phys14DR-PU20bx25_castor_PHYS14_25_V1-v2/MINIAODSIM",
        "/QCD_Pt-30to80_EMEnriched_Tune4C_13TeV_pythia8/Phys14DR-PU20bx25_castor_PHYS14_25_V1-v1/MINIAODSIM",
        "/QCD_Pt-80to170_EMEnriched_Tune4C_13TeV_pythia8/Phys14DR-PU20bx25_castor_PHYS14_25_V1-v1/MINIAODSIM",
    ]
    
    datasets15DR74 = [
        "/ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM",
        "/ST_t-channel_4f_mtop1695_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v3/MINIAODSIM",
        "/ST_t-channel_4f_mtop1755_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v3/MINIAODSIM",
        "/ST_t-channel_4f_scaleup_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM",
        "/ST_t-channel_4f_scaledown_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM",
        
        "/ST_t-channel_top_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM",
        "/ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM",
        
        "/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM",
        "/ST_tW_top_5f_mtop1695_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM",
        "/ST_tW_top_5f_mtop1755_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM",
        "/ST_tW_top_5f_scaleup_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM",
        "/ST_tW_top_5f_scaledown_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM",
        
        
        "/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/MINIAODSIM",
        "/ST_tW_antitop_5f_mtop1695_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM",
        "/ST_tW_antitop_5f_mtop1755_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v3/MINIAODSIM",
        "/ST_tW_antitop_5f_scaleup_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM",
        "/ST_tW_antitop_5f_scaledown_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v3/MINIAODSIM",
        
        "/ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",
        
        "/TT_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v4/MINIAODSIM",
        "/TT_TuneCUETP8M1_mtop1695_13TeV-powheg-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM",
        "/TT_TuneCUETP8M1_mtop1755_13TeV-powheg-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM",
        
        
        '/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM',
        '/TTJets_mtop1695_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v3/MINIAODSIM',
        '/TTJets_mtop1755_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM',
        '/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-scaleup-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/MINIAODSIM',
        '/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-scaledown-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/MINIAODSIM',
        

        "/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM",
        "/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/MINIAODSIM",
        "/QCD_Pt-20toInf_MuEnrichedPt15_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/MINIAODSIM",
        
        '/WW_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM',
        '/WZ_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/MINIAODSIM',
        '/ZZ_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/MINIAODSIM',
        
        
    ]
    
    datasetData=[
        '/SingleMuon/Run2015B-PromptReco-v1/MINIAOD',
        '/SingleMuon/Run2015B-17Jul2015-v1/MINIAOD'
    ]
    
    '''
    for dataset in datasetData:
        #for dataset in datasets15DR74:
        #processName = dataset.split("/")[1]+"_ALL"
        processName = dataset.split("/")[1]+"_"+dataset.split("/")[2]
    	jobName = processName+'_v150818'
    	print "status... ",jobName
    	#status("crab/"+jobName+"/crab_"+config.General.requestName)
    '''
    '''
    dataset=datasets15DR74[int(args[0])]
    #dataset=datasetData[int(args[0])]
    processName = dataset.split("/")[1]
    #processName = dataset.split("/")[1]+"_"+dataset.split("/")[2]+"_ALL"
    jobName = processName+'_v150818'
    print "submitting... ",jobName
    config.General.workArea = "crab/"+jobName
    config.Data.inputDataset=dataset
    config.JobType.pyCfgParams=['processName='+processName]
    #config.JobType.pyCfgParams=['processName='+processName,'isData=True','onlyFiltered=True']
    config.Data.outLFNDirBase='/store/user/mkomm/'+config.General.requestName+"/"+jobName
    submit(config)
    '''

