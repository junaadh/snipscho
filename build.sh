#!/bin/sh

set -xe

prog="snipscho"
iflag="--install"

mkdir -p build
CFLAGS="-Wall -Wextra `pkg-config --cflags jansson`"
LIBS="`pkg-config --libs jansson`"
clang $CFLAGS -o build/$prog src/main.c $LIBS -lm

if [ "$1" = "$iflag" ]; then
  rm -f ~/.local/bin/$prog
  cp ./build/$prog ~/.local/bin
fi
