#!/bin/bash
set -eu

LIB_SOURCE_FILES="vec3d.c game.c gameobject.c"

# clang $LIB_SOURCE_FILES -std=c89 -pedantic -Wall -Wno-comment -fsyntax-only
emcc $LIB_SOURCE_FILES glgoose.cpp -Wno-deprecated-declarations -Wno-comment -o emgoose
