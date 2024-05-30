#!/bin/sh
clear
clear
gcc -Wall -pedantic -g -c "../implementation.c" -o "Object.o" -DMODE_TEST && \
gcc -Wall -pedantic -g -o test main.c object.o -DMODE_TEST && \
valgrind ./test

