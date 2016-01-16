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

THETABASEDIR=$BASEDIR/external/slim-theta/release


addVar THETABASEDIR $THETABASEDIR
addVar PATH $THETABASEDIR/bin:"$"PATH
addVar LD_LIBRARY_PATH $THETABASEDIR/lib:"$"LD_LIBRARY_PATH

addVar PYTHONPATH $THETABASEDIR/python:"$"PYTHONPATH

mkdir build
cd build

execute $CMAKEBASEDIR/bin/cmake .. \
    -DBOOST_ROOT=$BOOSTBASEDIR \
    -DSQLITE_ROOT=$SQLITEBASEDIR \
    -DCMAKE_CXX_COMPILER=`which g++` \
    -DCMAKE_C_COMPILER=`which gcc` \
    -DCMAKE_INSTALL_PREFIX=$THETABASEDIR
    
execute make -j8
execute make install

cd $BASEDIR

echo $STOP

echo $VARS > theta-vars.txt

