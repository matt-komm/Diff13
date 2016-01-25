from Module import Module

from ModelClasses import *
import pyTool
import random
import logging
import ROOT
import os

class ThetaModel(Module):
    def __init__(self,options=[]):
        Module.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)

        
    def getUncertaintsDict(self):
        uncertainties = {
            "other":{"type":"gauss","config":{"mean": "1.0", "width":"1.0", "range":"(0.0,\"inf\")"}},
            #"BF":{"type":"gauss","config":{"mean": "1.0", "width":"0.3", "range":"(0.0,\"inf\")"}},
            #"CF":{"type":"gauss","config":{"mean": "1.0", "width":"0.3", "range":"(0.0,\"inf\")"}},
            #"LF":{"type":"gauss","config":{"mean": "1.0", "width":"0.3", "range":"(0.0,\"inf\")"}},
            "topbg":{"type":"gauss","config":{"mean": "1.0", "width":"0.3", "range":"(0.0,\"inf\")"}},
            "tChan":{"type":"gauss","config":{"mean": "1.0", "width":"1.0", "range":"(0.0,\"inf\")"}},
            "qcd_2j1t":{"type":"gauss","config":{"mean": "0.15", "width":"1.0", "range":"(0.0,\"inf\")"}},
            "qcd_3j1t":{"type":"gauss","config":{"mean": "0.15", "width":"1.0", "range":"(0.0,\"inf\")"}},
            "qcd_3j2t":{"type":"gauss","config":{"mean": "0.15", "width":"1.0", "range":"(0.0,\"inf\")"}},
            
            #"lumi":{"type":"gauss","config":{"mean": "1.0", "width":"0.1", "range":"(0.0,\"inf\")"}}
        }
        return uncertainties
        
    def getObservablesDict(self):
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
            "3j1t": {
                "weight":"(Reconstructed_1__nSelectedJet==3)*(Reconstructed_1__nSelectedBJet==1)"
            },
            "3j2t": {
               "weight":"(Reconstructed_1__nSelectedJet==3)*(Reconstructed_1__nSelectedBJet==2)"
            },
        }
        return observables
        
    def getBinning(self):
        return 30
        
    def getRange(self):
        return [0.0,200.0]
        
    def getFitVariableStr(self):
        return "(SingleTop_1__mtw_beforePz<50.0)*SingleTop_1__mtw_beforePz+(SingleTop_1__mtw_beforePz>50.0)*(Reconstructed_1__BDT_adaboost04_minnode001_maxvar3_ntree1000_invboost_binned*75.0+75.0+50.0)"
    
    
    def getComponentsDict(self):
        components={
            "tChan":
            {
                "sets":["tChannel"],
                "uncertainties":["tChan"],
                "weight":"1",
                "color":ROOT.kMagenta+1
            },
            
            "topbg":
            {
                "sets":["tWChannel","TTJets"],
                "uncertainties":["topbg"],
                "weight":"1",
                "color":ROOT.kOrange+1
            },
            
            "other":
            {
                "sets":["WJetsMG","DY"],
                "uncertainties":["other"],
                "weight":"1",
                "color":ROOT.kGreen+1
            },
            
            "QCD_2j1t":
            {
                "sets":["data1_antiiso","data2_antiiso","MC_antiiso"],
                "uncertainties":["qcd_2j1t"],
                "weight":"(Reconstructed_1__nSelectedJet==2)*(Reconstructed_1__nSelectedBJet==1)",
                "color":ROOT.kGray+1
            },

            "QCD_3j1t":
            {
                "sets":["data1_antiiso","data2_antiiso","MC_antiiso"],
                "uncertainties":["qcd_3j1t"],
                "weight":"(Reconstructed_1__nSelectedJet==3)*(Reconstructed_1__nSelectedBJet==1)",
                "color":ROOT.kGray+1
            },
 
            "QCD_3j2t":
            {
                "sets":["data1_antiiso","data2_antiiso","MC_antiiso"],
                "uncertainties":["qcd_3j2t"],
                "weight":"(Reconstructed_1__nSelectedJet==3)*(Reconstructed_1__nSelectedBJet==2)",
                "color":ROOT.kGray+1
            }
        }
    
        return components
        
    def getDataDict(self):
        data = {
            "data":
            {
                "sets":["data1","data2"],
                "weight":"1"
            }
        }
        return data
    
    def makeModel(self,name="fit",pseudo=False):
        self._logger.info("Creating model: "+name)
        histograms={}
    
        file = open(name+".cfg","w")
        
        model=Model(name, {"bb_uncertainties":"true"})
        
        
        uncertainties = self.module("ThetaModel").getUncertaintsDict()
        observables = self.module("ThetaModel").getObservablesDict()
        components = self.module("ThetaModel").getComponentsDict()
        
        varName = self.module("ThetaModel").getFitVariableStr()
        binning = self.module("ThetaModel").getBinning()
        ranges = self.module("ThetaModel").getRange()
        
        
        rootFiles = self.module("Files").getMCFiles()
        rootFiles+=self.module("Files").getDataFiles()
        
        for uncertaintyName in uncertainties.keys():
            uncertainties[uncertaintyName]["dist"]=Distribution(uncertaintyName, uncertainties[uncertaintyName]["type"], uncertainties[uncertaintyName]["config"])
            file.write(uncertainties[uncertaintyName]["dist"].toConfigString())
    

        
        for observableName in observables.keys():
            observable = Observable(observableName, binning, ranges)
            observableWeight = observables[observableName]["weight"]
            
            histograms[observableName]={}
            
            
            for componentName in components.keys():
                componentWeight = components[componentName]["weight"]
                componentUncertainties = components[componentName]["uncertainties"]
                
                componentHist = ROOT.TH1F("hist_"+observableName+"_"+componentName+"_"+str(random.random()),";"+varName+";Events",binning,ranges[0],ranges[1])
                componentHist.SetDirectory(0)
                componentHist.Sumw2()
                componentHist.SetFillColor(components[componentName]["color"])
                componentHist.SetLineColor(components[componentName]["color"])
                histograms[observableName][componentName] = componentHist

                self._logger.debug("Creating model: "+observableName+" "+componentName)
                
                for componentSetName in components[componentName]["sets"]:
                    sampleDict = self.module("Samples").getSample(componentSetName)
                    for processName in sampleDict["processes"]:
                        processWeight = sampleDict["weight"]

                        for i,f in enumerate(rootFiles):
                            rootFile = ROOT.TFile(f)
                            tree = rootFile.Get(processName)
                            
                            if (tree):
                                component=ObservableComponent(observableName+"__"+componentName+"__"+componentSetName+"__"+processName+"__"+str(i))
                                coeff=CoefficientMultiplyFunction()
                                for uncertaintyName in componentUncertainties:
                                    coeff.addDistribution(uncertainties[uncertaintyName]["dist"],uncertainties[uncertaintyName]["dist"].getParameterName())
                                component.setCoefficientFunction(coeff)
                                
                                hist=RootProjectedHistogram(observableName+"__"+componentName+"__"+componentSetName+"__"+processName+"__"+str(i),{"use_errors":"true"})
                                hist.setFileName(f)
                                hist.setVariableString(varName)
                                hist.setWeightString(observableWeight+"*"+componentWeight+"*"+processWeight)
                                hist.setTreeName(processName)
                                hist.setBinning(binning)
                                hist.setRange(ranges)
                                file.write(hist.toConfigString())
                                component.setNominalHistogram(hist)
                                
                                observable.addComponent(component)
                                
                                #self.module("Utils").getHist1D(componentHist,f,processName,varName,observableWeight+"*"+componentWeight+"*"+processWeight)
                        
                                
                                break
                            rootFile.Close()


            model.addObservable(observable)


            if not pseudo:
                histoadd = HistoAdd(observableName+"__data")
                data = self.module("ThetaModel").getDataDict()
                for componentName in data.keys():
                    componentWeight=data[componentName]["weight"]
                    
                    componentHist = ROOT.TH1F("hist_"+observableName+"_"+componentName+"_"+str(random.random()),";"+varName+";Events",binning,ranges[0],ranges[1])
                    componentHist.SetDirectory(0)
                    componentHist.Sumw2()
                    componentHist.SetMarkerStyle(20)
                    componentHist.SetMarkerSize(0.8)
                    histograms[observableName][componentName] = componentHist
                    
                    self._logger.debug("Creating model: "+observableName+" "+componentName)
                    
                    for componentSetName in data[componentName]["sets"]:
                        sampleDict = self.module("Samples").getSample(componentSetName)
                        for processName in sampleDict["processes"]:
                            processWeight = sampleDict["weight"]
                            
                            #print observableName,componentName,componentSetName#,observableWeight+"*"+componentWeight+"*"+processWeight
                            
                            for i,f in enumerate(rootFiles):
                                rootFile = ROOT.TFile(f)
                                tree = rootFile.Get(processName)
                                if (tree):
                                    hist=RootProjectedHistogram(observableName+"__"+componentName+"__"+componentSetName+"__"+processName+"__"+str(i),{"use_errors":"true"})
                                    hist.setFileName(f)
                                    hist.setVariableString(varName)
                                    hist.setWeightString(observableWeight+"*"+componentWeight+"*"+processWeight)
                                    hist.setTreeName(processName)
                                    hist.setBinning(binning)
                                    hist.setRange(ranges)
                                    file.write(hist.toConfigString())
                                    histoadd.addHisto(hist.getVarname())
                                    
                                    #self.module("Utils").getHist1D(componentHist,f,processName,varName,observableWeight+"*"+componentWeight+"*"+processWeight)
                                    
                                    break
                                    
                                rootFile.Close()

                file.write(histoadd.toConfigString())
            
            
            

                            
        file.write(model.toConfigString())

        file.write("\n")
        file.write("\n")

        file.write("myminimizer = {\n")

        file.write("type = \"newton_minimizer\";\n")
        file.write("//par_eps = 1e-6; // optional; default is 1e-4'\n")
        file.write("//maxit = 100000; // optional; default is 10,000'\n")
        file.write("//improve_cov = true; // optional; default is false'\n")
        file.write("//force_cov_positive = true; // optional, default is false'\n")
        file.write("//step_cov = 0.01; // optional; default is 0.1'\n")
        '''
        file.write("type = \"root_minuit\";\n")
        file.write("method = \"migrad\"; //optional. Default is 'migrad'\n")
        file.write("tolerance_factor = 0.1; //optional. Default is 1\n")
        file.write("max_iterations = 10000; // optional. Default as in ROOT::Minuit2\n")
        file.write("max_function_calls = 100000; //optional. Default as in ROOT::Minuit2\n")
        file.write("n_retries = 20; // optional; the default is 2\n")

        file.write("type = \"mcmc_minimizer\";\n")
        file.write("name = \"min0\";\n")
        file.write("iterations = 10000;\n")
        file.write("burn-in = 500; //optional. default is iterations / 10\n")
        file.write("stepsize_factor = 0.1; //optional, default is 1.0\n")
        '''
        file.write("};\n")

        file.write('pd = {\n')
        file.write('    name= "fit";\n')
        file.write('    type = "mle";\n')
        file.write('    parameters = ('+model.getParameterNames()+');\n')
        file.write('    minimizer = \"@myminimizer\";\n')
        file.write('    write_covariance = true;\n')
        file.write('};\n')

        file.write('main={\n')
        
        if pseudo:
            file.write('    data_source = {\n')
            file.write('        type = "model_source";\n')
            file.write('        name="data";\n')
            file.write('        model = "@'+model.getVarname()+'";\n')
            file.write('        dice_poisson = false; // optional; default is true\n')
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
        
            file.write('    data_source={\n')
            file.write('        type="histo_source";\n')
            file.write('        name="data";\n')

            #file.write('        obs_1j="@histoadd_1j__data";\n')
            #file.write('        obs_2j0t="@histoadd_2j0t__data";\n')
            file.write('        obs_2j1t="@histoadd_2j1t__data";\n')
            #file.write('        obs_3j0t="@histoadd_3j0t__data";\n')
            file.write('        obs_3j1t="@histoadd_3j1t__data";\n')
            file.write('        obs_3j2t="@histoadd_3j2t__data";\n')
            file.write('    };\n')

            

        file.write('    n-events=1;\n')
        file.write('    model="@'+model.getVarname()+'";\n')
        file.write('    output_database={\n')
        file.write('        type="rootfile_database";\n')
        file.write('        filename="'+name+'.root";\n')
        file.write('    };\n')
        file.write('    producers=("@pd"\n')
        file.write('    );\n')
        file.write('};\n')

        file.write('options = {\n')
        file.write('    plugin_files = ("$THETA_DIR/lib/libplugins.so", "$THETA_DIR/lib/libroot-plugin.so", "$THETA_DIR/lib/liblibtheta.so");\n')
        file.write('};\n')
            
        file.close()

        
        
        
