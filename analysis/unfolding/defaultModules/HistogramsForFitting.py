from Module import Module

import pyTool
import random
import logging
import ROOT

class HistogramsForFitting(Module):
    def __init__(self,options=[]):
        Module.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
    
    def getFitVariableStr(self):
        return "(SingleTop_1__mtw_beforePz<50.0)*SingleTop_1__mtw_beforePz+(SingleTop_1__mtw_beforePz>50.0)*(Reconstructed_1__BDT_boost02_minnode005_maxvar2_ntree1000_invboost*50.0+100.0)"
        
    def getBinning(self):
        return 30
        
    def getRange(self):
        return [0.0,150.0]
       
    def getComponentHistogram(self,name,addWeightStr="1"):
        componentHist = ROOT.TH1D(
            "compHist"+name+str(random.random()),
            "",
            self.module("HistogramsForFitting").getBinning(),
            self.module("HistogramsForFitting").getRange()[0],
            self.module("HistogramsForFitting").getRange()[1]
        )
        variableName = self.module("HistogramsForFitting").getFitVariableStr()
        
        component = self.module("Samples").getComponent(name)
        componentSamples = component["sets"]
        componentWeight = component["weight"]
        for fileName in self.module("Files").getMCFiles():
            for sampleName in componentSamples:
                sampleDict = self.module("Samples").getSample(samplesName)
                processes = sampleDict["processes"]
                sampleWeight = sampleDict["weight"]
                for processName in processes:
                    self.module("Utils").getHist1D(
                        componentHist,
                        fileName,
                        processName,
                        variableName,
                        weightStr+"*"+sampleWeight+"*"+componentWeight+"*"+addWeightStr
                    )
        return componentHist
                    
    def buildFitModel(self):
        uncertainties = {
            "other":ROOT.PyFit.Parameter("other"),
            "topbg":ROOT.PyFit.Parameter("topbg"),
            "tChan":ROOT.PyFit.Parameter("tChan"),
            "qcd_2j1t":ROOT.PyFit.Parameter("qcd_2j1t"),
            #"qcd_3j1t":ROOT.PyFit.Parameter("qcd_3j1t"),
            #"qcd_3j2t":ROOT.PyFit.Parameter("qcd_3j2t"),
            
            #"lumi":{"type":"gauss","config":{"mean": "1.0", "width":"0.1", "range":"(0.0,\"inf\")"}}
        }
        uncertainties["topbg"].setPrior(ROOT.TF1("top","TMath::LogNormal(x,0.01397)"))


        observables = {
            #"1j": {
            #    "weight":"(Reconstructed_1__nSelectedJet==1)"
            #},
            #"2j0t": {
            #    "weight":"(Reconstructed_1__nSelectedJet==2)*(Reconstructed_1__nSelectedBJet==0)"
            #},
            "2j1t": {
                "weight":"(Reconstructed_1__nSelectedJet==2)*(Reconstructed_1__nSelectedBJet==1)"
            },
            #"3j0t": {
            #    "weight":"(Reconstructed_1__nSelectedJet==3)*(Reconstructed_1__nSelectedBJet==0)"
            #},
            #"3j1t": {
            #    "weight":"(Reconstructed_1__nSelectedJet==3)*(Reconstructed_1__nSelectedBJet==1)"
            #},
            #"3j2t": {
            #   "weight":"(Reconstructed_1__nSelectedJet==3)*(Reconstructed_1__nSelectedBJet==2)"
            #},
        }
        fit = ROOT.PyFit.MLFit()
        
        for observableName in observables.keys():
            observable = ROOT.PyFit.Observable()
            observableWeight = observables[observableName]["weight"]
            for componentName in ["tChan","topbg","other","QCD"]:
                hist = self.module("HistogramsForFitting").getComponentHistogram(componentName,observableWeight)
                cv = ROOT.TCanvas("cv","",800,700)
                hist.Draw()
                cv.Update()
                cv.WaitPrimitive()
            
            '''
            for componentName in components.keys():
                componentWeight = components[componentName]["weight"]
                componentUncertainties = components[componentName]["uncertainties"]
                
                componentHist = ROOT.TH1F("hist_"+observableName+"_"+componentName+"_"+str(random.random()),";"+varName+";Events",binning,ranges[0],ranges[1])
                componentHist.Sumw2()
                componentHist.SetFillColor(components[componentName]["color"])
                componentHist.SetLineColor(components[componentName]["color"])
                
                if observableName=="2j1t":
                    samplesForUnfolding[componentName]=ROOT.TH1D("pt"+componentName+str(random.random()),";reconstructed top p_{T}",len(recoBins)-1,recoBins)
                    samplesForUnfolding[componentName].Sumw2()
                    samplesForUnfolding[componentName].SetFillColor(components[componentName]["color"])
                    samplesForUnfolding[componentName].SetLineColor(components[componentName]["color"])
                    
                for componentSetName in components[componentName]["sets"]:
                    for processName in sampleDict[componentSetName]["processes"]:
                        processWeight = sampleDict[componentSetName]["weight"]
                        weight = observableWeight+"*"+componentWeight+"*"+processWeight
                        print observableName,processName
                        for i,f in enumerate(rootFiles):
                            rootFile = ROOT.TFile(f)
                            tree = rootFile.Get(processName)
                            if (tree):                       
                                getHist1D(componentHist,f,processName,varName,weight) 
                                if observableName=="2j1t":
                                    getHist1D(samplesForUnfolding[componentName],f,processName,"SingleTop_1__Top_1__Pt",weight+"*(Reconstructed_1__BDT_boost04_minnode005_maxvar2_ntree1000_invboost>0.2)") 
                            
                            rootFile.Close()
                
                component = ROOT.PyFit.ConstShapeComponent(componentHist)
                
                for uncertaintyName in componentUncertainties:
                    component.addSFParameter(uncertainties[uncertaintyName])
                
                observable.addComponent(component)
            

            
            dataHist = ROOT.TH1F("hist_"+observableName+"_"+str(random.random()),";"+varName+";Events",binning,ranges[0],ranges[1])
            dataHist.Sumw2()

            if observableName=="2j1t":
                samplesForUnfolding["data"]=ROOT.TH1D("pt"+componentName+str(random.random()),";reconstructed top p_{T}",len(recoBins)-1,recoBins)
                samplesForUnfolding["data"].Sumw2()
                samplesForUnfolding["data"].SetMarkerStyle(20)
                samplesForUnfolding["data"].SetMarkerSize(0.9)
                
            componentWeight=data["weight"]
            
            for componentSetName in data["sets"]:
                for processName in sampleDict[componentSetName]["processes"]:
                    processWeight = sampleDict[componentSetName]["weight"]
                    weight=observableWeight+"*"+componentWeight+"*"+processWeight
                    for i,f in enumerate(rootFiles):
                        rootFile = ROOT.TFile(f)
                        tree = rootFile.Get(processName)
                        if (tree):
                            getHist1D(dataHist,f,processName,varName,weight)
                            if observableName=="2j1t":
                                getHist1D(samplesForUnfolding["data"],f,processName,"SingleTop_1__Top_1__Pt",weight+"*(Reconstructed_1__BDT_boost04_minnode005_maxvar2_ntree1000_invboost>0.2)") 
                        rootFile.Close()

            prediction = ROOT.PyFit.Prediction(binning)
            observable.getPrediction(prediction)
            
            predictionHist = ROOT.TH1F("histPrediction_"+observableName+"_"+str(random.random()),";"+varName+";Events",binning,ranges[0],ranges[1])
            prediction.toRootHistogram(predictionHist)
            
            cv = ROOT.TCanvas("cv"+observableName,"",800,600)
            axis=ROOT.TH2F("axis"+observableName,";"+varName+";Events",binning,ranges[0],ranges[1],50,0,1.2*max(predictionHist.GetMaximum(),dataHist.GetMaximum()))
            axis.Draw("AXIS")
            predictionHist.SetFillColor(ROOT.kGray)
            predictionHist.SetFillStyle(1001)
            predictionHist.Draw("HISTSame")
            dataHist.SetMarkerStyle(21)
            dataHist.SetMarkerSize(0.9)
            dataHist.Draw("PESame")
            cv.Update()
            cv.WaitPrimitive()
            
            fit.addObservable(observable,dataHist)  

        for uncertainty in uncertainties.values():
            fit.addParameter(uncertainty)

        fit.minimize()
        '''
