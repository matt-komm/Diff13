from Module import Module

import logging
import ROOT
import subprocess
import math
import os
import csv

class UnfoldingSetup(Module):
    def __init__(self,options=[]):
        Module.__init__(self,options)
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
    
        
    
