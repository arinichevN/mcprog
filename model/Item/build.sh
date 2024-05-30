#!/bin/bash

set -e

MODE=$2
GCC_OPTIONS=$3
LIB_DIR=$4
TAB=$5
TAB="${TAB}-"

function buildModule {
	gcc $MODE $GCC_OPTIONS -c implementation.c -o $1.o
}

function packSelfToLib {
	ar -cr $LIB_DIR/libapp.a $1.o
}

function buildDependency {
	cd $1 && \
	./build.sh slave $MODE "$GCC_OPTIONS" $LIB_DIR $TAB
}

function buildDependencies {
	buildDependency ../../lib/result_t && \
	buildDependency ../stime_t && \
	buildDependency ../Ton && \
	buildDependency ../Node && \
	buildDependency ../iCommand/common && \
	buildDependency ../PStSp
}

function master {
	clear && clear
	local MODULE_NAME=Item #local settings
	MODE=-DMODE_TEST
	GCC_OPTIONS="-Wall -pedantic -g"
	buildModule $MODULE_NAME && \
	echo "building " $MODULE_NAME ": DONE"
}

function slave {
	local MY_DIR=$(pwd)
	local MODULE_NAME=Item #local settings
	echo $TAB "building " $MODULE_NAME "..."
	buildDependencies && \
	
	cd $MY_DIR && \
	buildModule $MODULE_NAME && \
	packSelfToLib $MODULE_NAME && \
	echo $TAB "building " $MODULE_NAME ": DONE"
}


f=$1
${f}
