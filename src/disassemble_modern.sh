#!/bin/bash
set -eu
 


# pass program counter address as arg to this script to see disassembly around
# the program counter
pc_address="${1:-''}"


# this file has original source code info but not final relocated symbol locations
executable_file="codesegment.o"
mips64-elf-objdump  --disassemble   --prefix-addresses  --source --wide "$executable_file" > "${executable_file}.dasm"
# this file has final symbol locations but no source code. use offsets in functions to map instructions back to the codesegment.o file
# would be good if spicy had add an option to not strip this stuff out
executable_file="goose.out" # this should be the linked binary (not rom file)
mips64-elf-objdump    --disassemble-all --source --prefix-addresses --wide --all-header "$executable_file" > "${executable_file}.dasm"

if [ -n "$pc_address" ]; then 
  grep --color=always -C 8 "${pc_address/0x/}" "${executable_file}.dasm"
fi 
