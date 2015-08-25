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
#export SCRAM_ARCH=slc6_amd64_gcc481
addVar SCRAM_ARCH $SCRAM_ARCH

CMSSWVERSION="CMSSW_7_4_8"

execute "scramv1 project CMSSW $CMSSWVERSION"
cd $CMSSWVERSION/src

eval `scramv1 runtime -sh`
execute "git cms-init"

#execute "wget -q -O - https://github.com/matt-komm/Pxl/archive/3.5.1.tar.gz | tar xvzf -"
#execute "mv Pxl-3.5.1 Pxl"

#execute "git cms-merge-topic ikrav:egm_id_74X_v0"
#execute "git read-tree -mu HEAD"

execute "git cms-merge-topic -u cms-met:METCorUnc74X"

execute "git clone -b CMSSW_744_patch2 https://github.com/matt-komm/EDM2PXLIO.git"
execute "git reset EDM2PXLIO"
execute "git clone https://github.com/matt-komm/Pxl.git"
execute "git reset Pxl"


#execute "git checkout official-cmssw/CMSSW_7_5_X -- CommonTools/PileupAlgos"
#execute "rm CommonTools/PileupAlgos/plugins/SoftKillerProducer.cc"
#execute "git checkout official-cmssw/CMSSW_7_5_X -- DataFormats/Math"


ln -s ../../UserCode
ln -s ../../crab

execute "scram b -j10"

cd $BASEDIR

