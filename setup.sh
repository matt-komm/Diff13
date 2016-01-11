#!/bin/bash

VARS=""

function addVar
{
    VARS=$VARS" "$1"="$2
}

BASEDIR=`pwd`
addVar BASEDIR $BASEDIR
addVar PATH $BASEDIR/scripts:"$"PATH
addVar PYTHONPATH $BASEDIR/scripts:"$"PYTHONPATH
addVar LD_LIBRARY_PATH $BASEDIR/scripts:"$"LD_LIBRARY_PATH

echo $VARS > global-vars.txt

#source CMSSW/setupCMSSW.sh  

#source external/setupCMake.sh
#source external/setupSWIG.sh
#source external/setupPXL.sh

#source external/setupSqlite.sh
#source external/setupBoost.sh
#source external/setupTheta.sh

#source external/setupTUnfold.sh






