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
        out=$($@ 2>&1)
        ret=$?

        if [ $ret -eq 0 ]; then
            echo "ok"
        else
            echo "error"
            STOP=$out
        fi
    fi
}

cd $BASEDIR/external

execute git clone https://github.com/matt-komm/slim-theta.git

cd $BASEDIR/external/slim-theta

THETABASEDIR=$BASEDIR/external/slim-theta/build


addVar THETABASEDIR $THETABASEDIR
addVar PATH $THETABASEDIR/bin:"$"PATH

addVar PYTHONPATH $BASEDIR/external/slim-theta/python:"$"PYTHONPATH

mkdir build
cd build

execute $CMAKEBASEDIR/bin/cmake .. \
    -DBOOST_ROOT=$BOOSTBASEDIR \
    -DSQLITE_ROOT=$SQLITEBASEDIR \
    -DCMAKE_CXX_COMPILER=`which g++` \
    -DCMAKE_C_COMPILER=`which gcc`
    
execute make -j8

cd $BASEDIR

echo $STOP

echo $VARS > theta-vars.txt

