import re
import os
import ROOT
import random

import logging
logger = logging.getLogger(__file__)

class Files(object):
    def __init__(self,defaultModules,options=[]):
        self._defaultModules = defaultModules
        
    @staticmethod
    def getMCFiles():
        rootFiles = []
        basedir = "/nfs/user/mkomm/ST13/evaluate25ns"
        match = re.compile("mc[0-9]+.root")
        for f in os.listdir(basedir):
            if match.match(f):
                rootFiles.append(os.path.join(basedirMC,f))
        return rootFiles
        
    @staticmethod
    def getDataFiles():
        rootFiles = []
        basedir = "/nfs/user/mkomm/ST13/evaluate25nsData"
        match = re.compile("data[0-9]+.root")
        for f in os.listdir(basedir):
            if match.match(f):
                rootFiles.append(os.path.join(basedir,f))
        return rootFiles
        
    @staticmethod
    def getResponseFiles():
        rootFiles = []
        basedir = "/nfs/user/mkomm/ST13/response25ns"
        match = re.compile("selected[0-9]+.root")
        for f in os.listdir(basedir):
            if match.match(f):
                rootFiles.append(os.path.join(basedir,f))
        return rootFiles
        
    @staticmethod
    def getEfficiencyFiles():
        rootFiles = []
        basedir = "/nfs/user/mkomm/ST13/response25ns"
        match = re.compile("efficiency[0-9]+.root")
        for f in os.listdir(basedir):
            if match.match(f):
                rootFiles.append(os.path.join(basedir,f))
        return rootFiles
        
