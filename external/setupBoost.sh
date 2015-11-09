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

execute wget --no-check-certificate http://downloads.sourceforge.net/project/boost/boost/1.52.0/boost_1_52_0.tar.gz
execute tar -zxvf boost_1_52_0.tar.gz

cd $BASEDIR/external/boost_1_52_0

BOOSTBASEDIR=$BASEDIR/external/boost_1_52_0/release
addVar BOOSTBASEDIR $BOOSTBASEDIR
addVar BOOST_ROOT $BOOSTBASEDIR
addVar BOOST_INCLUDEDIR $BOOSTBASEDIR/include
addVar BOOST_LIBRARYDIR $BOOSTBASEDIR/lib
addVar LD_LIBRARY_PATH $BOOSTBASEDIR/lib:"$"LD_LIBRARY_PATH

execute ./bootstrap.sh --prefix=$BOOSTBASEDIR
execute ./b2 -j8 install

cd $BASEDIR

echo $STOP

echo $VARS > boost-vars.txt

