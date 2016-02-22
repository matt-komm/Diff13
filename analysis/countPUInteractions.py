### Skeleton for countEventWeight.py
# created by VISPA
# Sun Jul 19 21:55:29 2015
# -*- coding: utf-8 -*-

from pxl import core, modules
import ROOT


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
        module.addOption("output", "", "pu.root",modules.OptionDescription.USAGE_FILE_SAVE)

    def beginJob(self, parameters=None):
        ''' Executed before the first object comes in '''
        self._logger.log(core.LOG_LEVEL_INFO, "Begin job")
        self._outputFile=self.__module.getOption("output")
        self._nEvents={}
        
        self._nVertices=ROOT.TH1F("nVertices",";N vertices",52,0,52)
        self._nInteractions1D=ROOT.TH1F("nInteractions1D",";N interactions",52,0,52)
        self._nInteractions3D=ROOT.TH3F("nInteractions3D",";N interactions",52,0,52,52,0,52,52,0,52)
        self._nTrueInteractions1D=ROOT.TH1F("nTrueInteractions1D",";N true interactions",52,0,52)
        self._nTrueInteractions3D=ROOT.TH3F("nTrueInteractions3D",";N true interactions",52,0,52,52,0,52,52,0,52)

    def beginRun(self):
        ''' Executed before each run '''
        pass

    def process(self, object, sink):
        ''' Executed on every object '''
        event = core.toEvent(object)
        if not event:
            return "out"
            
        weight=event.getUserRecord("mc_weight")
        nVertices=0
        nInteractions=[0,0,0]
        nTrueInteractions=[0,0,0]
        
        for eventView in event.getEventViews():
            if eventView.getName()=="Generated":
                weight*=eventView.getUserRecord("genweight")
            if eventView.getName()=="Reconstructed":
                for particle in eventView.getParticles():
                    if particle.getName()=="PU":
                        nVertices = particle.getUserRecord("nVertices")
                        nInteractions = particle.getUserRecord("nInteractions0")
                        nTrueInteractions = particle.getUserRecord("nTrueInteractions0")
        
 
        self._nVertices.Fill(nVertices,weight)
        self._nInteractions1D.Fill(nInteractions[1],weight)
        self._nInteractions3D.Fill(nInteractions[0],nInteractions[1],nInteractions[2],weight)
        self._nTrueInteractions1D.Fill(nTrueInteractions[1],weight)
        self._nTrueInteractions3D.Fill(nTrueInteractions[0],nTrueInteractions[1],nTrueInteractions[2],weight)

        return "out"

    def endRun(self):
        ''' Executed after each run '''
        pass

    def endJob(self):
        ''' Executed after the last object '''
        self._logger.log(core.LOG_LEVEL_INFO, "End job")
        f=ROOT.TFile(self._outputFile,"RECREATE")
        self._nVertices.SetDirectory(f)
        self._nVertices.Scale(1.0/self._nVertices.Integral())
        self._nVertices.Write()
        self._nInteractions1D.SetDirectory(f)
        self._nInteractions1D.Scale(1.0/self._nVertices.Integral())
        self._nInteractions1D.Write()
        self._nInteractions3D.SetDirectory(f)
        self._nInteractions3D.Scale(1.0/self._nVertices.Integral())
        self._nInteractions3D.Write()
        self._nTrueInteractions1D.SetDirectory(f)
        self._nTrueInteractions1D.Scale(1.0/self._nVertices.Integral())
        self._nTrueInteractions1D.Write()
        self._nTrueInteractions3D.SetDirectory(f)
        self._nTrueInteractions3D.Scale(1.0/self._nVertices.Integral())
        self._nTrueInteractions3D.Write()
        f.Close()
        
