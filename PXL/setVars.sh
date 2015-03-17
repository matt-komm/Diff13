#!/bin/bash

PWD=`pwd`
for file in `find $PWD -name "*-vars.txt"`
    do
    for l in `cat $file`
        do
        echo "setting up ... "$l
        export `echo $l`
        done
    done

