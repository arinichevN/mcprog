#!/bin/bash

set -e

MODE=$2
GCC_OPTIONS=$3
LIB_DIR=$4
TAB=$5
TAB="${TAB}-"
MODULE_NAME_G=StdServer

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
	buildDependency ../result_t && \
	buildDependency ../server && \
	buildDependency ../ServerPackage && \
	buildDependency ../iServer
}

function master {
	clear && clear
	local MODULE_NAME=$MODULE_NAME_G
	MODE=-DMODE_TEST
	GCC_OPTIONS="-Wall -pedantic -g"
	buildModule $MODULE_NAME && \
	echo "building " $MODULE_NAME ": DONE"
}

function slave {
	local MY_DIR=$(pwd)
	local MODULE_NAME=$MODULE_NAME_G
	echo $TAB "building " $MODULE_NAME "..."
	buildDependencies && \
	
	cd $MY_DIR && \
	buildModule $MODULE_NAME && \
	packSelfToLib $MODULE_NAME && \
	echo $TAB "building " $MODULE_NAME ": DONE"
}


f=$1
${f}
