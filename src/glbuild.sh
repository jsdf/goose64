#!/bin/bash
set -eu

: ${CC:="clang"}
: ${CXX:="clang++"}

LIB_SOURCE_FILES="vec3d.c game.c gameobject.c"
CPP_SOURCE_FILES="glgoose.cpp gl/objloader.cpp gl/texture.cpp"

$CC $LIB_SOURCE_FILES -std=c89 -pedantic -Wall -Wno-comment -fsyntax-only
$CXX $LIB_SOURCE_FILES $CPP_SOURCE_FILES -Wno-deprecated-declarations -Wno-comment -framework OpenGL -framework GLUT -stdlib=libc++ -o glgoose  -Wno-deprecated -Wno-c++11-compat-deprecated-writable-strings
# builds ./glgoose