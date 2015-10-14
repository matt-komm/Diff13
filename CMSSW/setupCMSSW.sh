#!/bin/bash

STOP=""

VARS=""

function addVar
{
    VARS=$VARS" "$1"="$2
}

function execute 
{
    if [ -z "$STOP" ]; then
        echo -n $@ " ... "
        out=$(eval $@ 2>&1)
        ret=$?

        if [ $ret -eq 0 ]; then
            echo "ok"
        else
            echo "error"
            STOP=$out
        fi
    fi
}

cd $BASEDIR/CMSSW

export SCRAM_ARCH=slc6_amd64_gcc491
addVar SCRAM_ARCH $SCRAM_ARCH

CMSSWVERSION="CMSSW_7_4_15"

execute "scramv1 project CMSSW $CMSSWVERSION"
cd $CMSSWVERSION/src

eval `scramv1 runtime -sh`
execute "git cms-init"

execute "git cms-merge-topic ikrav:egm_id_7.4.12_v1"

execute "git clone -b CMSSW_7415 https://github.com/matt-komm/EDM2PXLIO.git"
execute "git reset EDM2PXLIO"
execute "git clone https://github.com/matt-komm/Pxl.git"
execute "git reset Pxl"

ln -s ../../UserCode
ln -s ../../crab

execute "scram b -j10"

cd $BASEDIR

