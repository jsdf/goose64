#!/bin/bash
set -euo pipefail

cd wavefront64
for f in ../*.obj; do
  lua wavefront64.lua obj "$f"
done

cd ..
echo "rebuilt:"
for f in *.obj; do
  echo "$f"
done
