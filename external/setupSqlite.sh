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

execute wget https://www.sqlite.org/2015/sqlite-autoconf-3090200.tar.gz
execute tar -zxvf sqlite-autoconf-3090200.tar.gz

cd $BASEDIR/external/sqlite-autoconf-3090200

SQLITEBASEDIR=$BASEDIR/external/sqlite-autoconf-3090200/release
addVar SQLITEBASEDIR $SQLITEBASEDIR
addVar PATH $SQLITEBASEDIR/bin:"$"PATH
addVar LD_LIBRARY_PATH $SQLITEBASEDIR/lib:"$"LD_LIBRARY_PATH
addVar PKG_CONFIG_PATH $SQLITEBASEDIR/lib/pkgconfig:"$"PKG_CONFIG_PATH

execute ./configure --prefix=$SQLITEBASEDIR
execute make -j8
execute make install

cd $BASEDIR

echo $STOP

echo $VARS > sqlite-vars.txt

