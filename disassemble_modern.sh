#!/bin/bash
set -eu
 

executable_file="goose64.out" # this should be the linked binary (not rom file)

# pass program counter address as arg to this script to see disassembly around
# the program counter
pc_address="${1:-''}"


mips64-elf-objdump  --disassemble-all --source  --wide --all-header "$executable_file" > disassembly.txt

if [ -n "$pc_address" ]; then 
  grep --color=always -C 8 "${pc_address/0x/}" disassembly.txt
fi 
