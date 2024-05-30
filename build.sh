#!/bin/bash

set -e

clear
clear

MODE=-DMODE_PROD
#MODE=-DMODE_TEST
GCC_OPTIONS="-Wall -pedantic -g"
LIB_DIR=$(pwd)
TAB=""

THIS_DIR=$(pwd)


#building dependencies
cd framework/app && \
./build.sh slave $MODE "$GCC_OPTIONS" $LIB_DIR $TAB


#building self
cd $THIS_DIR && \
gcc  $MODE $GCC_OPTIONS  main.c -o mcprog -L./ -lapp
