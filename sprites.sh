#!/bin/bash
set -euo pipefail

for f in ./*spr.{bmp,png}; do 
  python3 ultratex.py "$f"
  echo "rebuilt: $f"
done
 
