from Module import Module

from ModelClasses import *
import pyTool
import random
import logging
import ROOT
import os
import csv

class ThetaModel(Module):
    def __init__(self,options=[]):
        Module.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)

        
    def makeLogNormal(self,mean,unc):
        sigma2 = math.log(1+unc**2/mean**2)
        mu = math.log(mean)-0.5*sigma2
        #mu = 0.0
        return {"type":"log_normal","config":{"mu": "%4.3f"%(mu), "sigma":"%4.3f"%(math.sqrt(sigma2))}}
        #return {"type":"gauss","config":{"mean": "%4.3f"%(mean), "width":"%4.3f"%(unc), "range":"(0.0,\"inf\")"}}
       
    def makeGaus(self,mean,unc):
        return {"type":"gauss","config":{"mean": "%4.3f"%(mean), "width":"%4.3f"%(unc), "range":"(0.02,\"inf\")"}}
        
    def getUncertaintsDict(self):
        uncertainties = {
            "WZjets":self.module("ThetaModel").makeLogNormal(1.0,0.3),
            #"BF":{"type":"gauss","config":{"mean": "1.0", "width":"0.3", "range":"(0.0,\"inf\")"}},
            #"CF":{"type":"gauss","config":{"mean": "1.0", "width":"0.3", "range":"(0.0,\"inf\")"}},
            #"LF":{"type":"gauss","config":{"mean": "1.0", "width":"0.3", "range":"(0.0,\"inf\")"}},
            "TopBkg":self.module("ThetaModel").makeLogNormal(1.0,0.1),
            "tChannel":self.module("ThetaModel").makeGaus(1.0,10.0),
            "QCD_2j1t":self.module("ThetaModel").makeGaus(0.2,0.5),
            "QCD_3j1t":self.module("ThetaModel").makeGaus(0.2,0.5),
            "QCD_3j2t":self.module("ThetaModel").makeGaus(0.2,0.5),
            
            #"lumi":{"type":"gauss","config":{"mean": "1.0", "width":"0.1", "range":"(0.0,\"inf\")"}}
        }
        return uncertainties
        
    def getObservablesDict(self):
        observables = {
            "2j1t": {
                "weight":self.module("Utils").getCategoryCutStr(2,1)
            },
            "3j1t": {
                "weight":self.module("Utils").getCategoryCutStr(3,1)
            },
            "3j2t": {
               "weight":self.module("Utils").getCategoryCutStr(3,2)
            },
        }
        
        return observables
        
    def getBinning(self):
        return 15
        
    def getRange(self):
        return [0.0,150.0]
        
    def getFitVariableStr(self):
        return "(SingleTop_1__mtw_beforePz<50.0)*SingleTop_1__mtw_beforePz+(SingleTop_1__mtw_beforePz>50.0)*(TMath::TanH((Reconstructed_1__BDT_adaboost04_minnode001_maxvar3_ntree1000_invboost_binned-0.12)*3.2)*50.0+50.0+50.0)"
        #return "(SingleTop_1__mtw_beforePz<50.0)*SingleTop_1__mtw_beforePz+(SingleTop_1__mtw_beforePz>50.0)*(TMath::TanH((Reconstructed_1__BDT_adaboost04_minnode001_maxvar3_ntree1000_invboost_binned-0.17)*2.3)*50.0+50.0+50.0)"
        #return "(SingleTop_1__mtw_beforePz<50.0)*SingleTop_1__mtw_beforePz+(SingleTop_1__mtw_beforePz>50.0)*(Reconstructed_1__BDT_gradboost04_minnode001_maxvar3_ntree1000_pray_binned*50.0+50.0+50.0)"
        #return "(SingleTop_1__mtw_beforePz<50.0)*SingleTop_1__mtw_beforePz+(SingleTop_1__mtw_beforePz>50.0)*(fabs(SingleTop_1__absLEta_binned)/5.0*100.0+50.0)"
        #return "(SingleTop_1__mtw_beforePz<50.0)*SingleTop_1__mtw_beforePz+(SingleTop_1__mtw_beforePz>50.0)*(Reconstructed_1__C*150.0+50.0)"
    
    
    def getComponentsDict(self):
        components={
            "tChannel":
            {
                "sets":["tChannel"],
                "uncertainties":["tChannel"],
                "weight":"1",
                "color":ROOT.kMagenta+1
            },
            
            "TopBkg":
            {
                "sets":["tWChannel","TTJets"],
                "uncertainties":["TopBkg"],
                "weight":"1",
                "color":ROOT.kOrange+1
            },
            
            "WZjets":
            {
                "sets":["WJetsAMC","DY"],
                "uncertainties":["WZjets"],
                "weight":"1",
                "color":ROOT.kGreen+1
            },
            
            "QCD_2j1t":
            {
                "sets":["data76_antiiso","MC_antiiso"],
                "uncertainties":["QCD_2j1t"],
                "weight":"(Reconstructed_1__nSelectedJet==2)*(Reconstructed_1__nSelectedBJet==1)",
                "color":ROOT.kGray
            },

            "QCD_3j1t":
            {
                "sets":["data76_antiiso","MC_antiiso"],
                "uncertainties":["QCD_3j1t"],
                "weight":"(Reconstructed_1__nSelectedJet==3)*(Reconstructed_1__nSelectedBJet==1)",
                "color":ROOT.kGray
            },
 
            "QCD_3j2t":
            {
                "sets":["data76_antiiso","MC_antiiso"],
                "uncertainties":["QCD_3j2t"],
                "weight":"(Reconstructed_1__nSelectedJet==3)*(Reconstructed_1__nSelectedBJet==2)",
                "color":ROOT.kGray
            }
        }
            
        return components
        
    def getDataDict(self):
        data = {
            "data":
            {
                "sets":["data76"],
                "weight":"1"
            }
        }
        return data
        
    def getModel(self,name):
        return Model(name, {"bb_uncertainties":"true"})
        
    def makeMCDicedHistograms(self,histFileInput,histFileOutput):
        histFileNameInput = os.path.join(self.module("Utils").getOutputFolder(),histFileInput+"_fitHists.root")
        histFileNameOutput = os.path.join(self.module("Utils").getOutputFolder(),histFileOutput+"_fitHists.root")
        rootFileIn = ROOT.TFile(histFileNameInput)
        histograms = set()
        for k in rootFileIn.GetListOfKeys():
            histograms.add(k.GetName())
        rootFileOut = ROOT.TFile(histFileNameOutput,"RECREATE")
        for histName in histograms:
            newHist = rootFileIn.Get(histName).Clone()
            if histName.find("data")<0:
                for ibin in range(newHist.GetNbinsX()):
                    newHist.SetBinContent(ibin+1,
                        max(0.0001,newHist.GetBinContent(ibin+1)+
                        ROOT.gRandom.Gaus(0.0,newHist.GetBinError(ibin+1)))
                    )
            newHist.SetDirectory(rootFileOut)
            newHist.Write()
        rootFileOut.Close()
        rootFileIn.Close()
    
    def makeModel(self,modelName="fit",histFile="fit",outputFile="fit",pseudo=False):
        self._logger.info("Creating model: "+modelName)
        
        histFileName = os.path.join(self.module("Utils").getOutputFolder(),histFile+"_fitHists.root")
        outputFileName = os.path.join(self.module("Utils").getOutputFolder(),outputFile+".root")
        #20MB buffer
        file = open(os.path.join(self.module("Utils").getOutputFolder(),modelName+".cfg"),"w",20971520)
        
        model=self.module("ThetaModel").getModel(modelName)
        
        
        uncertainties = self.module("ThetaModel").getUncertaintsDict()
        observables = self.module("ThetaModel").getObservablesDict()
        components = self.module("ThetaModel").getComponentsDict()
        
        varName = self.module("ThetaModel").getFitVariableStr()
        binning = self.module("ThetaModel").getBinning()
        ranges = self.module("ThetaModel").getRange()
        
        for uncertaintyName in uncertainties.keys():
            uncertainties[uncertaintyName]["dist"]=Distribution(uncertaintyName, uncertainties[uncertaintyName]["type"], uncertainties[uncertaintyName]["config"])
            file.write(uncertainties[uncertaintyName]["dist"].toConfigString())
            
        for iobs,observableName in enumerate(observables.keys()):
            observable = Observable(observableName, binning, ranges)

            for icomp,componentName in enumerate(components.keys()):
                componentUncertainties = components[componentName]["uncertainties"]
                
                if componentName.startswith("QCD"):
                    if componentName.find(observableName)<0:
                        continue
                
                self._logger.debug("Creating model component: "+observableName+" "+componentName)

                componentHist = RootHistogram(observableName+"__"+componentName,{
                    "zerobin_fillfactor":0.001,
                    "use_errors":"true"
                })
                componentHist.setFileName(histFileName)
                componentHist.setHistoName(observableName+"__"+componentName+"__total")
                    
                component=ObservableComponent(observableName+"__"+componentName+"__"+str(icomp))
                coeff=CoefficientMultiplyFunction()
                for uncertaintyName in componentUncertainties:
                    coeff.addDistribution(uncertainties[uncertaintyName]["dist"],uncertainties[uncertaintyName]["dist"].getParameterName())
                component.setCoefficientFunction(coeff)
                component.setNominalHistogram(componentHist)
                observable.addComponent(component)

                file.write(componentHist.toConfigString())
                
                        
            model.addObservable(observable)


            if not pseudo:
                dataHist = RootHistogram(observableName+"__data",{
                    "zerobin_fillfactor":0.001,
                    "use_errors":"true"
                })
                dataHist.setFileName(histFileName)
                dataHist.setHistoName(observableName+"__data__total")

                file.write(dataHist.toConfigString())
                
            else:
                pass

                            
        file.write(model.toConfigString())

        file.write("\n")
        file.write("\n")

        file.write("myminimizer = {\n")
        
        file.write("type = \"newton_minimizer\";\n")
        file.write("par_eps = 1e-5; // optional; default is 1e-4'\n")
        file.write("maxit = 200000; // optional; default is 10,000'\n")
        file.write("improve_cov = true; // optional; default is false'\n")
        file.write("force_cov_positive = true; // optional, default is false'\n")
        file.write("step_cov = 0.025; // optional; default is 0.1'\n")
        file.write("};\n")
        
        
        '''
        file.write("\ttype = \"mcmc_minimizer\";\n")
        file.write("\tname = \"min0\";\n")
        file.write("\titerations = 10000;\n")
        file.write("\tburn-in = 2000; //optional. default is iterations / 10\n")
        file.write("\tstepsize_factor = 0.1; //optional, default is 1.0\n")
        
        file.write("\tafter_minimizer = {\n")
        file.write("\t\ttype = \"newton_minimizer\";\n")
        file.write("\t\tpar_eps = 1e-4; // optional; default is 1e-4'\n")
        file.write("\t\tmaxit = 200000; // optional; default is 10,000'\n")
        file.write("\t\timprove_cov = false; // optional; default is false'\n")
        file.write("\t\tforce_cov_positive = false; // optional, default is false'\n")
        file.write("\t\tstep_cov = 0.1; // optional; default is 0.1'\n")
        file.write("\t};\n")
        
        file.write("};\n")
        '''
        
        file.write('pd = {\n')
        file.write('    name= "'+modelName+'";\n')
        file.write('    type = "mle";\n')
        file.write('    parameters = ('+model.getParameterNames()+');\n')
        file.write('    minimizer = \"@myminimizer\";\n')
        file.write('    write_covariance = true;\n')
        file.write('};\n')

        file.write('main={\n')
        '''
        if pseudo:
            file.write('    data_source = {\n')
            file.write('        type = "model_source";\n')
            file.write('        name="data";\n')
            file.write('        model = "@'+model.getVarname()+'";\n')
            file.write('        dice_poisson = true; // optional; default is true\n')
            file.write('        dice_template_uncertainties = false; // optional; default is true\n')
            file.write('        dice_rvobs = false; // optional; default is true\n')
            file.write('        parameters-for-nll = {\n') 
            for uncName in uncertainties.keys():
                mean = uncertainties[uncName]["config"]["mean"]
                file.write('            '+uncName+' = '+str(mean)+';\n') 
            file.write('        }; //optional; assuming p1, p2, p3 are parameters\n')
            file.write('        rnd_gen = { seed = 123; }; // optional\n')
            file.write('        };\n')
        else:
        '''
        file.write('    data_source={\n')
        file.write('        type="histo_source";\n')
        file.write('        name="data";\n')
        for obs in self.module("ThetaModel").getObservablesDict().keys():
            file.write('        obs_'+obs+'="@hist_'+obs+'__data";\n')
        file.write('    };\n')

            

        file.write('    n-events=1;\n')
        file.write('    model="@'+model.getVarname()+'";\n')
        file.write('    output_database={\n')
        file.write('        type="rootfile_database";\n')
        file.write('        filename="'+outputFileName+'";\n')
        file.write('    };\n')
        file.write('    producers=("@pd"\n')
        file.write('    );\n')
        file.write('};\n')

        file.write('options = {\n')
        file.write('    plugin_files = ("$THETA_DIR/lib/libplugins.so", "$THETA_DIR/lib/libroot-plugin.so", "$THETA_DIR/lib/liblibtheta.so");\n')
        file.write('};\n')
            
        file.close()

        

    def makeFitHists(self,histFile="fit",pseudo=False,addCut="1"):
        self._logger.info("Creating fit hists: "+histFile)
        
        histFileName = os.path.join(self.module("Utils").getOutputFolder(),histFile+"_fitHists.root")
        csvFileName = os.path.join(self.module("Utils").getOutputFolder(),histFile+"_prefitYields.csv")
                
        histograms={}
        #20MB buffer

        observables = self.module("ThetaModel").getObservablesDict()
        components = self.module("ThetaModel").getComponentsDict()
        
        varName = self.module("ThetaModel").getFitVariableStr()
        binning = self.module("ThetaModel").getBinning()
        ranges = self.module("ThetaModel").getRange()
        
        
        rootFiles = self.module("Files").getMCFiles()
        rootFiles+=self.module("Files").getDataFiles()
        

            
        histograms = {}

        for iobs,observableName in enumerate(observables.keys()):
            observable = Observable(observableName, binning, ranges)
            observableWeight = observables[observableName]["weight"]
            
            if not histograms.has_key(observableName):
                histograms[observableName] = {}
            

            for icomp,componentName in enumerate(components.keys()):
                componentWeight = components[componentName]["weight"]
                componentUncertainties = components[componentName]["uncertainties"]
                
                if componentName.startswith("QCD"):
                    if componentName.find(observableName)<0:
                        continue
                
                if not histograms[observableName].has_key(componentName):
                    histograms[observableName][componentName] = {}
                
                self._logger.debug("Creating fit hists: "+observableName+" "+componentName)
                    
               
                
                totalEntries = 0.0
                totalIntegral = 0.0
                
                for componentSetName in components[componentName]["sets"]:
                    sampleDict = self.module("Samples").getSample(componentSetName)

                    for processName in sampleDict["processes"]:
                        processWeight = sampleDict["weight"]
                        
                        if not histograms[observableName][componentName].has_key(processName):
                            histograms[observableName][componentName][processName] = ROOT.TH1F(observableName+"_"+componentName+"_"+processName+"__"+str(random.random()),"",binning,ranges[0],ranges[1])
                            histograms[observableName][componentName][processName].Sumw2()

                        for i,f in enumerate(rootFiles):
                            rootFile = ROOT.TFile(f)
                            tree = rootFile.Get(processName)
                            if (tree):
                                self.module("Utils").getHist1D(
                                    histograms[observableName][componentName][processName],
                                    f,
                                    processName,
                                    varName,
                                    observableWeight+"*"+componentWeight+"*"+processWeight+"*"+addCut
                                )
                                #break
                            rootFile.Close()
                        
                        for ibin in range(binning):
                            content = histograms[observableName][componentName][processName].GetBinContent(ibin+1)
                            error = histograms[observableName][componentName][processName].GetBinError(ibin+1)
                            integral = histograms[observableName][componentName][processName].Integral()
                            entries = histograms[observableName][componentName][processName].GetEntries()
                            if entries>0.0 and math.fabs(integral)>0.0:
                                weight = math.fabs(integral/entries)
                                binEntries = content/weight
                                if math.fabs(binEntries)<1.0:
                                    histograms[observableName][componentName][processName].SetBinError(
                                        ibin+1,
                                        ROOT.TEfficiency.ClopperPearson(entries,0.0,0.683,True)*weight
                                    )
                                    self._logger.debug("\t\tassign uncertainty of "+str(0.3812*weight)+" for bin "+str(ibin+1))
                            
                        self._logger.debug("\t-> "+processName+" with events: "+str(round(histograms[observableName][componentName][processName].Integral(),1)))
                        totalEntries += histograms[observableName][componentName][processName].GetEntries()
                        totalIntegral += histograms[observableName][componentName][processName].Integral()
                
                self._logger.debug("-> total events: "+str(round(totalIntegral,1)))

            if not pseudo:
                
                if not histograms[observableName].has_key("data"):
                    histograms[observableName]["data"] = {"data":ROOT.TH1F(observableName+"_data__"+str(random.random()),"",binning,ranges[0],ranges[1])}
                    histograms[observableName]["data"]["data"].Sumw2()
                
                data = self.module("ThetaModel").getDataDict()
                for componentName in data.keys():
                    componentWeight=data[componentName]["weight"]
                    
                    if not histograms[observableName].has_key(componentName):
                        histograms[observableName][componentName] = {}
                      
                    self._logger.debug("Creating fit hists: "+observableName+" "+componentName)
                    
                    
                    for componentSetName in data[componentName]["sets"]:
                        sampleDict = self.module("Samples").getSample(componentSetName)
                        
                        
                        for processName in sampleDict["processes"]:
                            processWeight = sampleDict["weight"]
                            
                            for i,f in enumerate(rootFiles):
                                rootFile = ROOT.TFile(f)
                                tree = rootFile.Get(processName)
                                if (tree):
                                    self.module("Utils").getHist1D(
                                        histograms[observableName]["data"]["data"],
                                        f,
                                        processName,
                                        varName,
                                        observableWeight+"*"+componentWeight+"*"+processWeight+"*"+addCut
                                    )
                                    #break
                                rootFile.Close()
                                
                self._logger.debug("\t-> data with events: "+str(round(histograms[observableName]["data"]["data"].Integral(),1)))
                
            else:
                pass
                

        histFile = ROOT.TFile(histFileName,"RECREATE")
        for cat in histograms.keys():
            for comp in histograms[cat].keys():
                totalHist = None
                for process in histograms[cat][comp].keys():
                    hist = histograms[cat][comp][process]
                    if totalHist==None:
                        totalHist=hist.Clone(cat+"__"+comp+"__total")
                        totalHist.SetDirectory(histFile)
                    else:
                        totalHist.Add(hist)
                    histToWrite = hist.Clone(cat+"__"+comp+"__"+process)
                    histToWrite.SetDirectory(histFile)
                    histToWrite.Write()
                totalHist.Write()
        histFile.Close()
        

        self._logger.info("write prefit yield csv: "+csvFileName)
        outputFile = open(csvFileName, 'wb')
        writer = csv.DictWriter(
            outputFile, 
            ["category","component","process","entries","integral","unc"], 
            restval='NAN', 
            extrasaction='raise', 
            dialect='excel', 
            quoting=csv.QUOTE_NONNUMERIC
        )
        writer.writeheader()
        for cat in histograms.keys():
            for comp in histograms[cat].keys():
                totalEntries = 0.0
                totalIntegral = 0.0
                for process in histograms[cat][comp].keys():
                    hist = histograms[cat][comp][process]
                    writer.writerow({
                        "category":cat,
                        "component":comp,
                        "process":process,
                        "entries":round(hist.GetEntries(),1),
                        "integral":round(hist.Integral(),1),
                        "unc":round(hist.Integral()/math.sqrt(hist.GetEntries()+0.00001),1),
                    })
                    totalEntries+=hist.GetEntries()
                    totalIntegral+=hist.Integral()
                writer.writerow({
                        "category":cat,
                        "component":comp,
                        "process":"total",
                        "entries":round(totalEntries,1),
                        "integral":round(totalIntegral,1),
                        "unc":round(totalIntegral/math.sqrt(totalEntries+0.00001),1),
                    })


