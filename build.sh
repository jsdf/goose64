#!/bin/bash
set -eu
rm -f *.o
rm -f *.out
rm -f *.n64
wine cmd /c compile.bat