#!/bin/sh

CC=clang
CFLAGS="-Wall -Wextra"
LDFLAGS=-lm
SRC=main.c
OUT=out/pong

if ! pkg-config --exists raylib; then
    echo "Error: 'raylib' package not found."
    exit 1
fi

RAYLIB=$(pkg-config --cflags --libs raylib)

mkdir -p out/
$CC $CFLAGS $SRC -o $OUT $LDFLAGS $RAYLIB
