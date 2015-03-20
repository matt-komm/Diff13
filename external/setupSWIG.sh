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

execute wget http://sourceforge.net/projects/swig/files/swig/swig-2.0.12/swig-2.0.12.tar.gz
execute tar -zxvf swig-2.0.12.tar.gz

cd swig-2.0.12
SWIGBASEDIR=$BASEDIR/external/swig-2.0.12/release

addVar SWIGBASEDIR $SWIGBASEDIR


execute ./autogen.sh
execute ./configure --prefix=$SWIGBASEDIR --without-go --without-java --without-javascript --without-octave --without-csharp --without-perl5 --without-r --without-d --without-tcl --without-lua --without-doc
execute make -j8
execute make install

cd $BASEDIR

echo $STOP

echo $VARS > swig-vars.txt

