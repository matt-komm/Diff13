#!/bin/bash

VARS=""

function addVar
{
    VARS=$VARS" "$1"="$2
}

BASEDIR=`pwd`
addVar BASEDIR $BASEDIR
echo $VARS > global-vars.txt

#source CMSSW/setupCMSSW.sh && \
source external/setupSWIG.sh && \
source external/setupPXL.sh



