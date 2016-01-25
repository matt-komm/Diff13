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
        basedir = "/nfs/user/mkomm/ST13/evaluate25ns"
        match = re.compile("mc[0-9]+.root")
        for f in os.listdir(basedir):
            if match.match(f):
                rootFiles.append(os.path.join(basedir,f))
        self._logger.debug("mc files ("+str(len(rootFiles))+") found in "+basedir)
        return rootFiles
        
    def getDataFiles(self):
        rootFiles = []
        basedir = "/nfs/user/mkomm/ST13/evaluate25nsData"
        match = re.compile("data[0-9]+.root")
        for f in os.listdir(basedir):
            if match.match(f):
                rootFiles.append(os.path.join(basedir,f))
        self._logger.debug("data files ("+str(len(rootFiles))+") found in "+basedir)
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
        
