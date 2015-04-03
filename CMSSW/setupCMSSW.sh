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

#export SCRAM_ARCH=slc6_amd64_gcc491
export SCRAM_ARCH=slc6_amd64_gcc481
addVar SCRAM_ARCH $SCRAM_ARCH

CMSSWVERSION="CMSSW_7_2_4"

execute "scramv1 project CMSSW $CMSSWVERSION"
cd $CMSSWVERSION/src

execute "eval cmsenv"
execute "git cms-init"

#execute "wget -q -O - https://github.com/matt-komm/Pxl/archive/3.5.1.tar.gz | tar xvzf -"
#execute "mv Pxl-3.5.1 Pxl"

execute "git cms-merge-topic ikrav:egm_id_phys14"

execute "git submodule add -b plugin-based https://github.com/matt-komm/EDM2PXLIO.git"
execute "git submodule add https://github.com/matt-komm/Pxl.git"

#execute git cms-merge-topic matt-komm:ST_EA
#execute "echo .gitignore > .git/info/sparse-checkout"
#execute "echo .gitmodules >> .git/info/sparse-checkout"
#execute "echo EDM2PXLIO >> .git/info/sparse-checkout"
#execute "echo Pxl >> .git/info/sparse-checkout"
#execute git read-tree -mu HEAD

execute "scram b -j10"

cd $BASEDIR

