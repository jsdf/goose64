#!/bin/bash
set -eu

WINE_PATH="/Applications/Wine Stable.app/Contents/Resources/wine/bin/wine"

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

executable_file="goose64.out" # this should be the linked binary (not rom file)

# pass program counter address as arg to this script to see disassembly around
# the program counter
pc_address="${1:-''}"

OBJDUMPCOMMAND=" --disassemble-all --source  --wide --all-header --line-numbers ${executable_file} > disassembly.txt" "$WINE_PATH" cmd /c "$DIR/objdump.bat" 2>/dev/null 


if [ -n "$pc_address" ]; then 
  grep --color=always -C 8 "${pc_address/0x/}" disassembly.txt
fi 
