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
            #'level': 'INFO',
            'level': 'DEBUG',
            'class': 'logging.StreamHandler',
            'formatter': 'standard'
        },
    },
    'loggers': { 
        '': { 
            'handlers': ['default'],
            'level': 'DEBUG',
            'propagate': True
        },
    }
    
})
logger = logging.getLogger(__file__)
logger.setLevel(logging.DEBUG)

pluginPath = os.path.join(os.getcwd(),"plugins")

module = defaultModules.Module()
    
for moduleName in options.modules:
    try:
        logger.info("loading plugin '"+str(moduleName)+"'")
        module.loadModule(moduleName,pluginPath)
        #allow to pass options as modulename:options1:option2:...

    except Exception, e:
        logging.error(e)


program = module.module("Program").execute()

    
