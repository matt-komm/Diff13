from Module import Module

import logging
import ROOT

class Samples(Module):
    def __init__(self,options=[]):
        Module.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getQCDIsoCutStr(self):
        return "(Reconstructed_1__TightMuon_1__relIso>0.12)*(Reconstructed_1__TightMuon_1__relIso<10000.0)"
        
    def getComponent(self,name):
        components={
            "other":
            {
                "sets":["WJetsMG","DY"],
                "weight":"1",
                "color":ROOT.kGreen+1,
            },
            "topbg":
            {
                "sets":["tWChannel","TTJets"],
                "weight":"1",
                "color":ROOT.kOrange,
            },
            "tChan":
            {
                "sets":["tChannel"],
                "weight":"1",
                "color":ROOT.kMagenta,
            },
            "QCD_MC":
            {
                "sets":["QCD_MC"],
                "weight":"1",
                "color":ROOT.kGray+1,
            },
            "QCD_DD":
            {
                "sets":["data1_antiiso","data2_antiiso","MC_antiiso"],
                "weight":"1",
                "color":ROOT.kGray+1,
            },
            "otherBF":
            {
                "sets":["WJetsMG","DY"],
                "weight":"(Reconstructed_1__nBFlavorSelectedJet>0)",
                "color":ROOT.kGreen+2
            },
            "otherCF":
            {
                "sets":["WJetsMG","DY"],
                "weight":"(Reconstructed_1__nBFlavorSelectedJet==0)*(Reconstructed_1__nCFlavorSelectedJet>0)",
                "color":ROOT.kGreen-3
            },
            "otherLF":
            {
                "sets":["WJetsMG","DY"],
                "weight":"(Reconstructed_1__nBFlavorSelectedJet==0)*(Reconstructed_1__nCFlavorSelectedJet==0)",
                "color":ROOT.kGreen-7
            }
        }
        return components[name]
        
    def getSample(self,name):
        syst = self.module("Utils").getRecoSamplePrefix()
        mcweight = self.module("Utils").getRecoWeightStr()
        dataweight = self.module("Utils").getTriggerCutDataStr()
        sampleDict = {
            "tChannel":
            {
                "processes":[
                    "ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_iso"+syst,
                    "ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_ext_iso"+syst,
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kRed),
                "title":"t-channel",
                "weight":"0.5*"+mcweight
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
                    "WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_iso"+syst
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
                    "ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_antiiso",
                    "ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_antiiso",
                    "ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_antiiso",
                    "TT_TuneCUETP8M1_13TeV-powheg-pythia8_ext_antiiso",
                    #"WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_antiiso",
                    "WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_antiiso",
                    "DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_antiiso"
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kBlue-1),
                "title":"QCD (DD)",
                "weight":"(-1.0)*"+mcweight+"*"+self.module("Samples").getQCDIsoCutStr()
            },
            
            "data1_antiiso":
            {
                "processes":[
                    "SingleMuon_Run2015D-05Oct2015-v1_antiiso",
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kBlack),
                "title":"Data",
                "weight":dataweight+"*"+self.module("Samples").getQCDIsoCutStr()
            },
            
            "data2_antiiso":
            {
                "processes":[
                    "SingleMuon_Run2015D-PromptReco-v4_antiiso",
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kBlack),
                "title":"Data",
                "weight":dataweight+"*"+self.module("Samples").getQCDIsoCutStr()
            },
            
            "data1":
            {
                "processes":[
                    "SingleMuon_Run2015D-05Oct2015-v1_iso",
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kBlack),
                "title":"Data",
                "weight":dataweight
            },
            
            "data2":
            {
                "processes":[
                    "SingleMuon_Run2015D-PromptReco-v4_iso",
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kBlack),
                "title":"Data",
                "weight":dataweight
            }
        }
        return sampleDict[name]
        
        
