#!/bin/bash
set -eu

clang glgoose.c vec3d.c game.c gameobject.c -std=c89 -pedantic -Wall -Wno-deprecated-declarations -Wno-comment -framework OpenGL -framework GLUT -o glgoose 
./glgoose