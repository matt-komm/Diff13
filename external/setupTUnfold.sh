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
mkdir TUnfold_V17.5
execute wget --no-check-certificate http://www.desy.de/~sschmitt/TUnfold/TUnfold_V17.5.tgz 
execute tar -zxvf TUnfold_V17.5.tgz -C TUnfold_V17.5

cd TUnfold_V17.5

execute make lib

addVar TUNFOLDDIR $BASEDIR/external/TUnfold_V17.5
addVar LD_LIBRARY_PATH $TUNFOLDDIR:"$"LD_LIBRARY_PATH


cd $BASEDIR

echo $STOP

echo $VARS > tunfold-vars.txt

