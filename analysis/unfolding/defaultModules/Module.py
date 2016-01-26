import logging
import os
import imp

class Module(object):
    _modules = None
    _classes = None
    
    def __init__(self,options=[]):
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
        if Module._classes == None:
            Module._classes = {}
            for cl in Module.__subclasses__():
                Module._classes[cl.__name__]=cl
                self._logger.info("add default class: "+cl.__name__)
                
            
    def loadModule(self,name,pluginPath):
        filePath = None
        for f in os.listdir(pluginPath):
            if f==name+".py":
                filePath = os.path.join(pluginPath,f)
                break
        
        if filePath==None:
            raise Exception("Module '"+name+"' cannot be located!")
        imp.load_source(name, filePath)

        for clName in Module._classes.keys():
            cl = Module._classes[clName]
            if len(cl.__subclasses__())>0:
                if len(cl.__subclasses__())>1:
                    self._logger.warning("multiple subclasses for plugin '"+clName+"' found")
                Module._classes[clName]=cl.__subclasses__()[-1]
                self._logger.info(clName+": replace '"+cl.__name__+"' -> '"+cl.__subclasses__()[-1].__name__+"'")
                cl.__subclasses__()[-1].baseClass=cl
                
    def initObjects(self):
        if Module._modules == None:
            Module._modules = {}
            for clName in Module._classes.keys():
                Module._modules[clName]=Module._classes[clName]()
                self._logger.info("init module '"+clName+"' with class '"+Module._classes[clName].__name__+"'")
        
    def module(self,name):
        self.initObjects()
        return Module._modules[name]
        
    @staticmethod
    def getClass(name):
        return Module._classes[name]
        
    
