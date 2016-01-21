class Module(object):
    def __init__(self,defaultModules,options=[]):
        self._defaultModules = defaultModules
        
    def module(self,name):
        return self._defaultModules[name](self._defaultModules)
        
