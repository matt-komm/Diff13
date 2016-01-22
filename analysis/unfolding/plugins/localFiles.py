import re
import os
from defaultModules import Module

import logging

class LocalFiles(Module.getClass("Files")):
    def __init_(self,defaultModules,options=[]):
        Module.getClass("Files").__init__(self,defaultModules,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    def getMCFiles(self):
        rootFiles = []
        basedir = "/home/mkomm/Analysis/ST13/Diff13/analysis/evaluate25ns"
        match = re.compile("mc[0-9]+.root")
        for f in os.listdir(basedir):
            if match.match(f):
                rootFiles.append(os.path.join(basedirMC,f))
        self._logger.debug("mc files ("+str(len(rootFiles))+") found in "+basedir)
        return rootFiles
        
    def getDataFiles(self):
        rootFiles = []
        basedir = "/home/mkomm/Analysis/ST13/Diff13/analysis/evaluate25nsData"
        match = re.compile("data[0-9]+.root")
        for f in os.listdir(basedir):
            if match.match(f):
                rootFiles.append(os.path.join(basedir,f))
        self._logger.debug("data files ("+str(len(rootFiles))+") found in "+basedir)
        return rootFiles
        
    def getResponseFiles(self):
        rootFiles = []
        basedir = "/home/mkomm/Analysis/ST13/Diff13/analysis/response25ns"
        match = re.compile("selected[0-9]+.root")
        for f in os.listdir(basedir):
            if match.match(f):
                rootFiles.append(os.path.join(basedir,f))
        self._logger.debug("response files ("+str(len(rootFiles))+") found in "+basedir)
        return rootFiles
        
    def getEfficiencyFiles(self):
        rootFiles = []
        basedir = "/home/mkomm/Analysis/ST13/Diff13/analysis/response25ns"
        match = re.compile("efficiency[0-9]+.root")
        for f in os.listdir(basedir):
            if match.match(f):
                rootFiles.append(os.path.join(basedir,f))
        self._logger.debug("efficiency files ("+str(len(rootFiles))+") found in "+basedir)
        return rootFiles


