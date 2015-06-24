#!/bin/sh

gcc -ggdb -O3 $2 -std=c99 -o $1 $1.c -lm -lgmp -lpthread

