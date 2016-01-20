import logging
logger = logging.getLogger(__file__)

class Program(object):
    def __init__(self,defaultModules,options=[]):
        self._defaultModules=defaultModules
        
    def execute(self):
        utils = self._defaultModules["Utils"]
        responseMatrix = self._defaultModules["ResponseMatrix"].get()
