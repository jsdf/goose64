#!/bin/bash
set -eu
RELEASE=y ./build.sh
cp goose64.n64 release/
echo "check that its not broken"
/Applications/Games/mupen64plus-bundle-osx-2.5.9/mupen64plus.app/Contents/MacOS/mupen64plus  release/goose64.n64 
