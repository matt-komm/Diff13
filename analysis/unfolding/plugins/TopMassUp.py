import ROOT
import math
import numpy
import random
import os

from defaultModules import Module

import logging

class ThetaModelTopMassUp(Module.getClass("ThetaModel")):
    def __init__(self,options=[]):
        ThetaModelTopMassUp.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
    
    #mixin 2/3*nominal+1/3*variation for 1GeV mass uncertainty
    def getComponentsDict(self):
        components=ThetaModelTopMassUp.baseClass.getComponentsDict(self)
        components["tChannel"]["sets"].append("tChannelMass")
        components["TopBkg"]["sets"].append("tWChannelMass")
        components["TopBkg"]["sets"].append("TTJetsMass")
        return components
        
        

class SamplesTopMassUp(Module.getClass("Samples")):
    def __init__(self,options=[]):
        SamplesTopMassUp.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getSample(self,name):
        syst = self.module("Utils").getRecoSamplePrefix()
        mcweight = self.module("Utils").getRecoWeightStr()+"*"+self.module("Utils").getTriggerCutMCStr()
        dataweight = "1"
        
        altDict = {
            "tChannel":
            {
                "processes":[
                    #"ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_iso"+syst,
                    "ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_ext_iso"+syst,
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kRed),
                "title":"t-channel",
                "weight":mcweight+"*0.666"
            },

            "tWChannel":
            {
                "processes":[
                    "ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_iso"+syst,
                    "ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_iso"+syst
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kOrange),
                "title":"tW-channel",
                "weight":mcweight+"*0.666"
            },

            "TTJets":
            {
                "processes":[
                    "TT_TuneCUETP8M1_13TeV-powheg-pythia8_ext_iso"+syst
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kOrange-3),
                "title":"t#bar{t}",
                "weight":mcweight+"*Generated_1__top_pt_rew"+"*0.666"
            },
        
        
            "tChannelMass":
            {
                "processes":[
                    #"ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_iso"+syst,
                    "ST_t-channel_4f_mtop1755_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_iso"+syst,
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kRed),
                "title":"t-channel",
                "weight":mcweight+"*0.333"
            },

            "tWChannelMass":
            {
                "processes":[
                    "ST_tW_top_5f_mtop1755_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_iso"+syst,
                    "ST_tW_antitop_5f_mtop1755_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_iso"+syst
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kOrange),
                "title":"tW-channel",
                "weight":mcweight+"*0.333"
            },

            "TTJetsMass":
            {
                "processes":[
                    "TT_TuneCUETP8M1_mtop1755_13TeV-powheg-pythia8_iso"+syst
                ],
                "color":ROOT.gROOT.GetColor(ROOT.kOrange-3),
                "title":"t#bar{t}",
                "weight":mcweight+"*Generated_1__top_pt_rew"+"*0.333"
            }
        }
        
        if name not in altDict.keys():
            return SamplesTopMassUp.baseClass.getSample(self,name)
        else:
            return altDict[name]
            
        
class ResponseMatrixPtTopMassUp(Module.getClass("ResponseMatrixPt")):
    def __init__(self,options=[]):
        ResponseMatrixPtTopMassUp.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getSignalProcessNames(self):
        return [
            ["ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_ext","0.666"],
            ["ST_t-channel_4f_mtop1755_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1","0.333"]
        ]
        
class ResponseMatrixYTopMassUp(Module.getClass("ResponseMatrixY")):
    def __init__(self,options=[]):
        ResponseMatrixYTopMassUp.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getSignalProcessNames(self):
        return [
            ["ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_ext","0.666"],
            ["ST_t-channel_4f_mtop1755_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1","0.333"]
        ]
        
class UtilsTopMassUp(Module.getClass("Utils")):
    def __init__(self,options=[]):
        UtilsTopMassUp.baseClass.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getOutputFolder(self):
        return os.path.join(os.getcwd(),"result/TopMassUp")
        
        
