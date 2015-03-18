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

execute wget https://forge.physik.rwth-aachen.de/attachments/download/391/pxl-3.5.1.tar.gz
execute tar -zxvf pxl-3.5.1.tar.gz

cd pxl-3.5.1
PXLBASEDIR=`pwd`/release
addVar PXLBASEDIR $PXLBASEDIR
execute mkdir build
cd build

execute cmake .. \
    -DCMAKE_INSTALL_PREFIX=$PXLBASEDIR \
    -DSWIG_DIR=$SWIGBASEDIR \
    -DSWIG_EXECUTABLE=$SWIGBASEDIR/bin/swig \
    -DENABLE_NUMPY=OFF \
    -DUSE_HEALPIX=OFF \
    -DENABLETESTING=OFF \
    -DENABLESWIGDOCSTRINGS=OFF
execute make -j8
execute make install

cd $BASE

echo $STOP

echo $VARS > pxl-vars.txt
