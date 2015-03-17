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


BASE=`pwd`

#execute hg clone -u pxl-3.5.1 https://forge.physik.rwth-aachen.de/hg/pxl

cd pxl
PXLBASEDIR=`pwd`/release
addVar PXLBASEDIR $PXLBASEDIR
execute mkdir build
cd build

execute cmake .. -DCMAKE_INSTALL_PREFIX=$PXLBASEDIR -DSWIG_DIR=$SWIGBASEDIR -DSWIG_EXECUTABLE=$SWIGBASEDIR/bin/swig 
execute make -j8
execute make install

cd $BASE

echo $STOP

echo $VARS > pxl-vars.txt
