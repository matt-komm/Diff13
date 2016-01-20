import logging
logger = logging.getLogger(__file__)

class Utils(object):
    def __init_(self,defaultModules,options=[]):
        self._defaultModules = defaultModules
        
    @staticmethod
    def getMCFiles():
        return []
        
    @staticmethod
    def getDataFiles():
        return []
        
    @staticmethod
    def getResponseFiles():
        return []
        
    @staticmethod
    def getEfficiencyFiles():
        return []
        
            
    @staticmethod
    def getHist1D(hist,fileName,processName,variableName,weight):
        hist.SetDirectory(0)
        rootFile = ROOT.TFile(f)
        tree = rootFile.Get(processName)
        tempHist=hist.Clone()
        tempHist.Scale(0)
        tempHist.SetEntries(0)
        tempHist.SetName("hist_"+processName+str(random.random()))
        if (tree):
            tree.Project(tempHist.GetName(),variableName,weight)
            tempHist.SetDirectory(0)
            tempHist.SetBinContent(0,0)
            tempHist.SetBinContent(tempHist.GetNbinsX()+1,0)
            hist.Add(tempHist)
        rootFile.Close()
        
    @staticmethod
    def getHist2D(hist,fileName,processName,variableNameX,variableNameY,weight):
        hist.SetDirectory(0)
        rootFile = ROOT.TFile(f)
        tree = rootFile.Get(processName)
        tempHist=hist.Clone()
        tempHist.Scale(0)
        tempHist.SetEntries(0)
        if (tree):
            tree.Project(tempHist.GetName(),variableNameY+":"+variableNameX,weight)
            tempHist.SetDirectory(0)
            for ibin in range(tempHist.GetNbinsX()+2):
                tempHist.SetBinContent(ibin,0,0)
                tempHist.SetBinContent(ibin,tempHist.GetNbinsY()+1,0)
            for jbin in range(tempHist.GetNbinsY()+2):
                tempHist.SetBinContent(0,jbin,0)
                tempHist.SetBinContent(tempHist.GetNbinsX()+1,jbin,0)
            hist.Add(tempHist)
        rootFile.Close()
        
    @staticmethod
    def normalizeByBinWidth(hist):
        #hist.Scale(1./(hist.GetXaxis().GetXmax()-hist.GetXaxis().GetXmin())/hist.Integral())
        hist.Scale(1./hist.Integral())
        for ibin in range(hist.GetNbinsX()):
            hist.SetBinError(ibin+1,hist.GetBinError(ibin+1)/hist.GetBinWidth(ibin+1))
            hist.SetBinContent(ibin+1,hist.GetBinContent(ibin+1)/hist.GetBinWidth(ibin+1))
