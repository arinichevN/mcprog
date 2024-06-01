#!/bin/bash

set -e

MODE=$2
GCC_OPTIONS=$3
LIB_DIR=$4
TAB=$5
TAB="${TAB}-"
MODULE_NAME_G=server

function buildModule {
	:
}

function packSelfToLib {
	:
}

function buildDependency {
	cd $1 && \
	./build.sh slave $MODE "$GCC_OPTIONS" $LIB_DIR $TAB
}

function buildDependencies {
	buildDependency ../ServerPackage
}

function master {
	:
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
