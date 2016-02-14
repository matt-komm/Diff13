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

        
    def getUncertaintsDict(self):
        uncertainties = {
            "WZjets":{"type":"log_normal","config":{"mu": "0.0", "sigma":str(math.sqrt(math.log(1.0+1)))}},
            #"BF":{"type":"gauss","config":{"mean": "1.0", "width":"0.3", "range":"(0.0,\"inf\")"}},
            #"CF":{"type":"gauss","config":{"mean": "1.0", "width":"0.3", "range":"(0.0,\"inf\")"}},
            #"LF":{"type":"gauss","config":{"mean": "1.0", "width":"0.3", "range":"(0.0,\"inf\")"}},
            "TopBkg":{"type":"log_normal","config":{"mu": "0.0", "sigma":str(math.sqrt(math.log(0.3+1)))}},
            "tChannel":{"type":"log_normal","config":{"mu": "0.0", "sigma":str(math.sqrt(math.log(1.0+1)))}},
            "QCD_2j1t":{"type":"log_normal","config":{"mu": "0.0", "sigma":str(math.sqrt(math.log(1.0+1)))}},
            "QCD_3j1t":{"type":"log_normal","config":{"mu": "0.0", "sigma":str(math.sqrt(math.log(1.0+1)))}},
            "QCD_3j2t":{"type":"log_normal","config":{"mu": "0.0", "sigma":str(math.sqrt(math.log(1.0+1)))}},
            
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
        return 30
        
    def getRange(self):
        return [0.0,200.0]
        
    def getFitVariableStr(self):
        return "(SingleTop_1__mtw_beforePz<50.0)*SingleTop_1__mtw_beforePz+(SingleTop_1__mtw_beforePz>50.0)*(Reconstructed_1__BDT_adaboost04_minnode001_maxvar3_ntree1000_invboost_binned*75.0+75.0+50.0)"
    
    
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
                "sets":["WJetsMG"],#,"DY"],
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
    
    def makeModel(self,name="fit",pseudo=False,addCut="1"):
        self._logger.info("Creating model: "+name)
        
        histograms={}
    
        file = open(os.path.join(self.module("Utils").getOutputFolder(),name+".cfg"),"w")
        
        model=self.module("ThetaModel").getModel(name)
        
        
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

        for iobs,observableName in enumerate(observables.keys()):
            observable = Observable(observableName, binning, ranges)
            observableWeight = observables[observableName]["weight"]+"*"+addCut

            for componentName in components.keys():
                componentWeight = components[componentName]["weight"]
                componentUncertainties = components[componentName]["uncertainties"]
                
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
                            rootFile.Close()
                            
                
            model.addObservable(observable)


            if not pseudo:
                histoadd = HistoAdd(observableName+"__data")
                data = self.module("ThetaModel").getDataDict()
                for componentName in data.keys():
                    componentWeight=data[componentName]["weight"]
                      
                    self._logger.debug("Creating model: "+observableName+" "+componentName)
                    
                    for componentSetName in data[componentName]["sets"]:
                        sampleDict = self.module("Samples").getSample(componentSetName)
                        for processName in sampleDict["processes"]:
                            processWeight = sampleDict["weight"]
                            
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

                                    #break
                                rootFile.Close()

                file.write(histoadd.toConfigString())
                
            else:
                histoadd = HistoAdd(observableName+"__data",{"dice_stat":"true","rnd":7*(iobs*3+11)})
                data = self.module("ThetaModel").getDataDict()
                for componentName in components.keys():
                    componentWeight = components[componentName]["weight"]
                    componentUncertainties = components[componentName]["uncertainties"]
   
                    self._logger.debug("Creating pseudo data: "+observableName+" "+componentName)
                    
                    for componentSetName in components[componentName]["sets"]:
                        sampleDict = self.module("Samples").getSamplePseudo(componentSetName)
                        for processName in sampleDict["processes"]:
                            processWeight = sampleDict["weight"]
                            
                            for i,f in enumerate(rootFiles):
                                rootFile = ROOT.TFile(f)
                                tree = rootFile.Get(processName)
                                if (tree):
                                    hist=RootProjectedHistogram("pseudodata__"+observableName+"__"+componentName+"__"+componentSetName+"__"+processName+"__"+str(i),
                                        {
                                            "use_errors":"true"
                                        }
                                    )
                                    hist.setFileName(f)
                                    hist.setVariableString(varName)
                                    hist.setWeightString(observableWeight+"*"+componentWeight+"*"+processWeight)
                                    hist.setTreeName(processName)
                                    hist.setBinning(binning)
                                    hist.setRange(ranges)
                                    file.write(hist.toConfigString())
                                    histoadd.addHisto(hist.getVarname())

                                rootFile.Close()

                file.write(histoadd.toConfigString())
            
        '''  
        csvFileName = os.path.join(self.module("Utils").getOutputFolder(),"prefitYields.csv")
        self._logger.info("write prefit yield csv: "+csvFileName)
        outputFile = open(csvFileName, 'wb')
        writer = csv.DictWriter(
            outputFile, 
            ["category","component","sample","cut","entries","integral","unc"], 
            restval='NAN', 
            extrasaction='raise', 
            dialect='excel', 
            quoting=csv.QUOTE_NONNUMERIC
        )
        writer.writeheader()
        for cat in histograms.keys():
            for comp in histograms[cat].keys():
                for sample in histograms[cat][comp].keys():
                    for cut in histograms[cat][comp][sample].keys():
                        if histograms[cat][comp][sample][cut].GetEntries()>0.0:
                            writer.writerow({
                                "category":cat,
                                "component":comp,
                                "sample":sample,
                                "cut":cut,
                                "entries":histograms[cat][comp][sample][cut].GetEntries(),
                                "integral":round(histograms[cat][comp][sample][cut].Integral(),1),
                                "unc":round(histograms[cat][comp][sample][cut].Integral()/math.sqrt(histograms[cat][comp][sample][cut].GetEntries()),1),
                            })
                        else:
                            writer.writerow({
                                "category":cat,
                                "component":comp,
                                "sample":sample,
                                "cut":cut,
                                "entries":histograms[cat][comp][sample][cut].GetEntries(),
                                "integral":0.0,
                                "unc":0.0,
                            })
        outputFile.close()
        '''
                            
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
            file.write('        obs_'+obs+'="@histoadd_'+obs+'__data";\n')
        file.write('    };\n')

            

        file.write('    n-events=1;\n')
        file.write('    model="@'+model.getVarname()+'";\n')
        file.write('    output_database={\n')
        file.write('        type="rootfile_database";\n')
        file.write('        filename="'+os.path.join(self.module("Utils").getOutputFolder(),name+'.root')+'";\n')
        file.write('    };\n')
        file.write('    producers=("@pd"\n')
        file.write('    );\n')
        file.write('};\n')

        file.write('options = {\n')
        file.write('    plugin_files = ("$THETA_DIR/lib/libplugins.so", "$THETA_DIR/lib/libroot-plugin.so", "$THETA_DIR/lib/liblibtheta.so");\n')
        file.write('};\n')
            
        file.close()

        
        
        
