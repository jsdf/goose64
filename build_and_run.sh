#!/bin/bash
set -eu
rm -f stage00.o
./build.sh && ./run.sh