import logging

class Module(object):
    _modules = None
    def __init__(self,options=[]):
        self._logger = logging.getLogger(__file__)
        self._logger.setLevel(logging.DEBUG)
        
        if Module._modules == None:
            Module._modules = {}
            for cl in Module.__subclasses__():
                if len(cl.__subclasses__())>0:
                    if len(cl.__subclasses__())>1:
                        self._logger.warning("multiple subclasses for plugin '"+cl.__name__+"' found")
                    Module._modules[cl.__name__]=cl.__subclasses__()[-1](options)
                    self._logger.info("replace '"+cl.__name__+"' -> '"+cl.__subclasses__()[-1].__name__+"'")
                else:
                    Module._modules[cl.__name__]=cl(options)
        
    def module(self,name):
        return Module._modules[name]
        
