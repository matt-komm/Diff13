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

execute wget https://cmake.org/files/v3.4/cmake-3.4.0-rc3.tar.gz
execute tar -zxvf cmake-3.4.0-rc3.tar.gz

cd $BASEDIR/external/cmake-3.4.0-rc3

CMAKEBASEDIR=$BASEDIR/external/cmake-3.4.0-rc3/release
addVar CMAKEBASEDIR $CMAKEBASEDIR
addVar PATH $CMAKEBASEDIR/bin:"$"PATH

execute ./bootstrap --prefix=$CMAKEBASEDIR
execute make -j8
execute make install

cd $BASEDIR

echo $STOP

echo $VARS > cmake-vars.txt

