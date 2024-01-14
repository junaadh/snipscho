#!/bin/sh

set -xe

mkdir -p build
CFLAGS="-Wall -Wextra `pkg-config --cflags jansson`"
LIBS="`pkg-config --libs jansson`"
clang $CFLAGS $1 -o build/snipscho src/main.c $LIBS -lm
