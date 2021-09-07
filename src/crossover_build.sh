#!/bin/bash
set -eu
rm -f *.o
rm -f *.out
rm -f *.n64
/Applications/CrossOver.app/Contents/SharedSupport/CrossOver/bin/wineloader32on64 winewrapper.exe --new-console --workdir /Users/jfriend/Library/Application\ Support/CrossOver/Bottles/n64sdk/drive_c/goose64/src --start -- /Users/jfriend/Library/Application\ Support/CrossOver/Bottles/n64sdk/drive_c/goose64/src/compile.bat