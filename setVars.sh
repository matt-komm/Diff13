#!/bin/bash

PWD=`pwd`
for file in `find $PWD -name "*-vars.txt"`
    do
    echo "reading "$file" ..."
    for l in `cat $file`
        do
        echo "     setting up ... "$l
        eval "export" $l
        done
    done

