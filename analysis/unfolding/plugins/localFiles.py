import re
import os
from defaultModules import Utils

import logging
logger = logging.getLogger(__file__)

class LocalFiles(Utils):
    def __init_(self,defaultModules,options=[]):
        Utils.__init__(self,defaultModules,options)
        self._defaultModules = defaultModules
        
    @staticmethod
    def getMCFiles():
        rootFiles = []
        basedir = "/home/mkomm/Analysis/ST13/Diff13/analysis/evaluate25ns"
        match = re.compile("mc[0-9]+.root")
        for f in os.listdir(basedir):
            if match.match(f):
                rootFiles.append(os.path.join(basedirMC,f))
        return rootFiles
        
    @staticmethod
    def getDataFiles():
        rootFiles = []
        basedir = "/home/mkomm/Analysis/ST13/Diff13/analysis/evaluate25nsData"
        match = re.compile("data[0-9]+.root")
        for f in os.listdir(basedir):
            if match.match(f):
                rootFiles.append(os.path.join(basedir,f))
        return rootFiles
        
    @staticmethod
    def getResponseFiles():
        rootFiles = []
        basedir = "/home/mkomm/Analysis/ST13/Diff13/analysis/response25ns"
        match = re.compile("selected[0-9]+.root")
        for f in os.listdir(basedir):
            if match.match(f):
                rootFiles.append(os.path.join(basedir,f))
        return rootFiles
        
    @staticmethod
    def getEfficiencyFiles():
        rootFiles = []
        basedir = "/home/mkomm/Analysis/ST13/Diff13/analysis/response25ns"
        match = re.compile("efficiency[0-9]+.root")
        for f in os.listdir(basedir):
            if match.match(f):
                rootFiles.append(os.path.join(basedir,f))
        return rootFiles
