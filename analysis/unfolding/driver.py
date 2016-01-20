#!/usr/bin/python

import imp
import os
import os.path
import sys
import inspect
import logging
import logging.config
import defaultModules
from optparse import OptionParser

parser = OptionParser()
parser.add_option("-m", "--module", dest="modules",
                  action="append", type="string", default=[])

(options, args) = parser.parse_args()

logging.config.dictConfig({
    "version":1,
    'formatters': { 
        'standard': { 
            #'format': '%(asctime)s [%(levelname)s] %(name)s: %(message)s'
            'format': '%(levelname)-10s %(message)s'# [%(name)s:%(lineno)i]'
        },
    },
    'handlers': { 
        'default': { 
            'level': 'INFO',
            'class': 'logging.StreamHandler',
            'formatter': 'standard'
        },
    },
    'loggers': { 
        '': { 
            'handlers': ['default'],
            'level': 'INFO',
            'propagate': True
        },
    }
    
})
logger = logging.getLogger(__file__)

defaultModules = {
    "responseMatrix": defaultModules.ResponseMatrix
}

pluginPath = os.path.join(os.getcwd(),"plugins")

def loadModule(name):
    filePath = None
    for f in os.listdir(pluginPath):
        if f==name+".py":
            filePath = os.path.join(pluginPath,f)
            break
    
    if filePath==None:
        raise Exception("Module '"+name+"' cannot be located!")
    module = imp.load_source(name, filePath)
    return module
    
for moduleName in options.modules:
    try:
        m = loadModule(moduleName)
        #allow to pass options as modulename:options1:option2:...
        logger.info("loaded module '"+str(m.__name__)+"'")
    except Exception, e:
        logging.error(e)
    
for m in defaultModules.keys():
    if len(defaultModules[m].__subclasses__())>1:
        logger.warning("multiple subclasses loaded for "+m)
    if len(defaultModules[m].__subclasses__())>0:
        logger.info("replacing "+str(defaultModules[m].__name__)+" -> "+str(defaultModules[m].__subclasses__()[-1].__name__))
        defaultModules[m]=defaultModules[m].__subclasses__()[-1]
    
responseMatrix = defaultModules["responseMatrix"].get()
    
