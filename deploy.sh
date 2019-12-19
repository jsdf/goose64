#!/bin/bash
set -eu

# ./build.sh
for f in *.n64; do
  romfile="$f"
done

# loader64 can be found here: https://github.com/jsdf/loader64
 
# upload rom
loader64 -v --write --file "$romfile"

# boot rom that was uploaded
loader64 -v --pifboot