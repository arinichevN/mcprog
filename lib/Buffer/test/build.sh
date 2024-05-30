#!/bin/bash

set -e

clear
clear

#MODE=-DMODE_DEBUG
MODE=-DMODE_TEST
GCC_OPTIONS="-Wall -pedantic -g"
LIB_DIR=$(pwd)
TAB=""

THIS_DIR=$(pwd)


echo "lib dir is here " $LIB_DIR
#building dependencies
cd ../ && \
./build.sh slave $MODE "$GCC_OPTIONS" $LIB_DIR $TAB


#building self
cd $THIS_DIR && \
gcc  $MODE $GCC_OPTIONS  $(pwd)/main.c -o test -L./ -lapp
