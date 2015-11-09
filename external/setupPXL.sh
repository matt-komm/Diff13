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

execute wget --no-check-certificate https://forge.physik.rwth-aachen.de/attachments/download/391/pxl-3.5.1.tar.gz
execute tar -zxvf pxl-3.5.1.tar.gz

cd pxl-3.5.1

PXLBASEDIR=$BASEDIR/external/pxl-3.5.1/release
addVar PXLBASEDIR $PXLBASEDIR
addVar PATH $PXLBASEDIR/bin:"$"PATH
addVar LD_LIBRARY_PATH $PXLBASEDIR/lib:"$"LD_LIBRARY_PATH
addVar PYTHONPATH $PXLBASEDIR/lib/python2.7/site-packages:"$"PYTHONPATH
addVar PKG_CONFIG_PATH $PXLBASEDIR/lib/pkgconfig:"$"PKG_CONFIG_PATH

execute mkdir build
cd build

execute $CMAKEBASEDIR/bin/cmake .. \
    -DCMAKE_INSTALL_PREFIX=$PXLBASEDIR \
    -DSWIG_DIR=$SWIGBASEDIR \
    -DSWIG_EXECUTABLE=$SWIGBASEDIR/bin/swig \
    -DENABLE_NUMPY=OFF \
    -DENABLETESTING=OFF \
    -DENABLESWIGDOCSTRINGS=OFF \
    -DCMAKE_CXX_COMPILER=`which g++` \
    -DCMAKE_C_COMPILER=`which gcc`
execute make -j8
execute make install

cd $BASEDIR

echo $STOP

echo $VARS > pxl-vars.txt

