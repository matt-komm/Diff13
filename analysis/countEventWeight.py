### Skeleton for countEventWeight.py
# created by VISPA
# Sun Jul 19 21:55:29 2015
# -*- coding: utf-8 -*-

from pxl import core, modules
import csv

class Example(modules.PythonModule):

    def __init__(self):
        ''' Initialize private variables '''
        modules.PythonModule.__init__(self)
        # self._exampleVariable = startValue

    def initialize(self, module):
        ''' Initialize module sinks, sources and options '''
        self.__module = module
        self._logger = core.Logger(self.__module.getName())

        module.addSink("in", "Input port")
        module.addSource("out", "Output port")
        module.addOption("output", "", "events.txt",modules.OptionDescription.USAGE_FILE_SAVE)
        module.addOption("step", "", "")

    def beginJob(self, parameters=None):
        ''' Executed before the first object comes in '''
        self._logger.log(core.LOG_LEVEL_INFO, "Begin job")
        self._outputFile=self.__module.getOption("output")
        self._step = self.__module.getOption("step")
        self._nEvents={}

    def beginRun(self):
        ''' Executed before each run '''
        pass

    def process(self, object, sink):
        ''' Executed on every object '''
        event = core.toEvent(object)
        if not event:
            return "out"
        process = event.getUserRecord("ProcessName")
        region = "none"
        if event.hasUserRecord("isolation"):
            region = event.getUserRecord("isolation")
            
        weight=1.0
        for eventView in event.getEventViews():
            if eventView.getName()=="Generated":
                weight=eventView.getUserRecord("genweight")
                break
        if not self._nEvents.has_key(process):
            self._nEvents[process]={}
        if not self._nEvents[process].has_key(region):
            self._nEvents[process][region]={"total":0,"positive":0,"negative":0}
            
        self._nEvents[process][region]["total"]+=1
        if weight>0.0:
            self._nEvents[process][region]["positive"]+=1
        else:
            self._nEvents[process][region]["negative"]+=1
        

        # return the name of the source
        return "out"

    def endRun(self):
        ''' Executed after each run '''
        pass

    def endJob(self):
        ''' Executed after the last object '''
        self._logger.log(core.LOG_LEVEL_INFO, "End job")
        f=open(self._outputFile,'wb')
        writer = csv.DictWriter(
            f, 
            ["process","isolation","step","MC","MCpositive","MCnegative"], 
            restval='NAN', 
            extrasaction='raise', 
            dialect='excel', 
            quoting=csv.QUOTE_NONNUMERIC
        )
        writer.writeheader()
        
        for process in sorted(self._nEvents.keys()):
            for region in sorted(self._nEvents[process].keys()):
                writer.writerow({
                    "process":process,
                    "isolation":region,
                    "step":self._step,
                    "MC":self._nEvents[process][region]["total"],
                    "MCpositive":self._nEvents[process][region]["positive"],
                    "MCnegative":self._nEvents[process][region]["negative"]
                })

        
        f.close()
        
        
        
        
        
        
        
        
        
        
        
        
        
