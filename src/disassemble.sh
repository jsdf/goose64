#!/bin/bash
set -eu

# pass the program counter address as an arg to this script to see disassembly around
# the program counter, or pass a space-delimited list of addresses to get a stack trace

executable_file="goose64.out" # this should be the linked binary (not rom file)

WINE_PATH=wine

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"


OBJDUMPCOMMAND=" --disassemble-all --source  --wide --all-header --line-numbers ${executable_file} > disassembly.txt" "$WINE_PATH" cmd /c "$DIR/objdump.bat" 2>/dev/null 
 
i=0
for arg in "${@:1}"; do

  if [[ "$i" = "0" ]]; then
    echo "exception at:"
    grep --color=always -C 8 "^${arg/0x/} <\w*\W" disassembly.txt
  else
    echo "called from:"
    grep --color=always  "^${arg/0x/} <\w*\W" disassembly.txt
  fi

    ((i=i+1))
done
