from Module import Module

import logging
import ROOT

class Samples(Module):
    def __init__(self,options=[]):
        Module.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getQCDIsoCutStr(self):
        return "(SingleTop_1__TightMuon_1__relIso>0.20)*(SingleTop_1__TightMuon_1__relIso<10000.0)"
        
        
    def getSample(self,name):
        syst = self.module("Utils").getRecoSamplePrefix()
        mcweight = self.module("Utils").getRecoWeightStr()+"*"+self.module("Utils").getTriggerCutMCStr()
        dataweight = "1"
        sampleDict = {
            "tChannel":
            {
                "processes":[
                    #"ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_iso"+syst,
                    "ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_ext_iso"+syst,
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kRed),
                "title":"t-channel",
                "weight":mcweight
            },

            "tWChannel":
            {
                "processes":[
                    "ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_iso"+syst,
                    "ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_iso"+syst
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kOrange),
                "title":"tW-channel",
                "weight":mcweight
            },

            "TTJets":
            {
                "processes":[
                    "TT_TuneCUETP8M1_13TeV-powheg-pythia8_ext_iso"+syst
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kOrange-3),
                "title":"t#bar{t}",
                "weight":mcweight+"*Generated_1__top_pt_rew"
            },
            
            "WJetsAMC":
            {
                "processes":[
                    "WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_ext_iso"+syst
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kGreen-2),
                "title":"W+jets",
                "addtitle":"(aMC@NLO)",
                "weight":mcweight
            },
            
            "WJetsMG":
            {
                "processes":[
                    "WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_iso"+syst
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kGreen-2),
                "title":"W+jets",
                "addtitle":"(MadGraph)",
                "weight":mcweight
            },
            
            "DY":
            {
                "processes":[
                    "DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_iso"+syst
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kBlue-1),
                "title":"Drell-Yan",
                "weight":mcweight
            },

            "QCD_MC":
            {
                "processes":[
                    "QCD_Pt-20toInf_MuEnrichedPt15_TuneCUETP8M1_13TeV_pythia8_iso"+syst,
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kGray),
                "title":"QCD (MC)",# #lower[-0.06]{#scale[0.85]{#times#frac{1}{5}}}",
                "weight":mcweight
            },
            
            "MC_antiiso":
            {
                "processes":[
                    "ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_ext_antiiso",
                    "ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_antiiso",
                    "ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_antiiso",
                    "TT_TuneCUETP8M1_13TeV-powheg-pythia8_ext_antiiso",
                    "WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_ext_antiiso",
                    #"WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_antiiso",
                    "DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_antiiso"
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kBlue-1),
                "title":"QCD (DD)",
                "weight":"(-1.0)*"+mcweight+"*"+self.module("Samples").getQCDIsoCutStr()
            },
            

            
            "data76_antiiso":
            {
                "processes":[
                    "SingleMuon_Run2015D-16Dec2015-v1_antiiso",
                ],
                
                "weight":self.module("Utils").getTriggerCutDataStr()+"*"+dataweight+"*"+self.module("Samples").getQCDIsoCutStr()
            },
            

            
            "data76":
            {
                "processes":[
                    "SingleMuon_Run2015D-16Dec2015-v1_iso",
                ],
                
                "weight":self.module("Utils").getTriggerCutDataStr()+"*"+dataweight
               
            }
        }
        
        return sampleDict[name]
        
    def getSamplePseudo(self,name):
        syst = self.module("Utils").getRecoSamplePrefixPseudo()
        mcweight = self.module("Utils").getRecoWeightStrPseudo()+"*"+self.module("Utils").getTriggerCutMCStr()
        dataweight = "1"
        sampleDict = {
            "tChannel":
            {
                "processes":[
                    #"ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_iso"+syst,
                    "ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_ext_iso"+syst,
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kRed),
                "title":"t-channel",
                "weight":mcweight
            },

            "tWChannel":
            {
                "processes":[
                    "ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_iso"+syst,
                    "ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_iso"+syst
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kOrange),
                "title":"tW-channel",
                "weight":mcweight
            },

            "TTJets":
            {
                "processes":[
                    "TT_TuneCUETP8M1_13TeV-powheg-pythia8_ext_iso"+syst
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kOrange-3),
                "title":"t#bar{t}",
                "weight":mcweight
            },
            
            "WJetsAMC":
            {
                "processes":[
                    "WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_ext_iso"+syst
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kGreen-2),
                "title":"W+jets",
                "addtitle":"(aMC@NLO)",
                "weight":mcweight
            },
            
            "WJetsMG":
            {
                "processes":[
                    "WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_iso"+syst
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kGreen-2),
                "title":"W+jets",
                "addtitle":"(MadGraph)",
                "weight":mcweight
            },
            
            "DY":
            {
                "processes":[
                    "DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_iso"+syst
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kBlue-1),
                "title":"Drell-Yan",
                "weight":mcweight
            },

            "QCD_MC":
            {
                "processes":[
                    "QCD_Pt-20toInf_MuEnrichedPt15_TuneCUETP8M1_13TeV_pythia8_iso"+syst,
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kGray),
                "title":"QCD (MC)",# #lower[-0.06]{#scale[0.85]{#times#frac{1}{5}}}",
                "weight":mcweight
            },
            
            #take iso MC-QCD as antiiso data to generate pseudo data 
            "data76_antiiso":
            {
                "processes":[
                    "QCD_Pt-20toInf_MuEnrichedPt15_TuneCUETP8M1_13TeV_pythia8_iso"+syst,
                ],
                
                "weight":mcweight
            },
            
            #do not subtract any other process
            "MC_antiiso":
            {
                "processes":[
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kBlue-1),
                "title":"QCD (DD)",
                "weight":"1"
            },
        }
        return sampleDict[name]
        
        
