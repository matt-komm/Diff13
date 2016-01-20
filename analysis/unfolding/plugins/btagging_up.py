from defaultModules import ResponseMatrix

import logging
logger = logging.getLogger(__file__)

class ResponseMatrix_btagging_up(ResponseMatrix):
    def __init__(self,defaultModules,options=[]):
        ResponseMatrix.__init__(self,defaultModule,options)
        self._defaultModules = defaultModules
        
    @staticmethod
    def get():
        print "derived class",__file__
