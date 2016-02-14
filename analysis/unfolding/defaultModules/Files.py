import re
import os
import ROOT
import random

from Module import Module

import logging

class Files(Module):
    def __init__(self,options=[]):
        Module.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)

    def getMCFiles(self):
        rootFiles = []
        basedirSignalMC = "/nfs/user/mkomm/ST13/evaluate25ns/MC76X_30/signal"
        matchSignalMC = re.compile("mcSignal[0-9]+.root")

        basedirBackgroundMC = "/nfs/user/mkomm/ST13/evaluate25ns/MC76X_30/background"
        matchBackgroundMC = re.compile("mcBackground[0-9]+.root")
        
        for f in os.listdir(basedirSignalMC):
            if matchSignalMC.match(f):
                rootFiles.append(os.path.join(basedirSignalMC,f))

        for f in os.listdir(basedirBackgroundMC):
            if matchBackgroundMC.match(f):
                rootFiles.append(os.path.join(basedirBackgroundMC,f))
        
        self._logger.debug("mc files ("+str(len(rootFiles))+") found")
        return rootFiles
        
    def getDataFiles(self):
        rootFiles = []
        basedirData = "/nfs/user/mkomm/ST13/evaluate25ns/data76X"
        matchData = re.compile("data[0-9]+.root")
        for f in os.listdir(basedirData):
            if matchData.match(f):
                rootFiles.append(os.path.join(basedirData,f))
        self._logger.debug("data files ("+str(len(rootFiles))+") found in "+basedirData)
        return rootFiles
        
    def getResponseFiles(self):
        rootFiles = []
        basedir = "/nfs/user/mkomm/ST13/response25ns"
        match = re.compile("selected[0-9]+.root")
        for f in os.listdir(basedir):
            if match.match(f):
                rootFiles.append(os.path.join(basedir,f))
        self._logger.debug("response files ("+str(len(rootFiles))+") found in "+basedir)
        return rootFiles
        
    def getEfficiencyFiles(self):
        rootFiles = []
        basedir = "/nfs/user/mkomm/ST13/response25ns"
        match = re.compile("efficiency[0-9]+.root")
        for f in os.listdir(basedir):
            if match.match(f):
                rootFiles.append(os.path.join(basedir,f))
        self._logger.debug("efficiency files ("+str(len(rootFiles))+") found in "+basedir)
        return rootFiles
        
