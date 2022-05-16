#!/bin/bash
set -eu

INVALIDATED_FILES=`python3 get_changed_files.py`

count=0
if [ -n "$INVALIDATED_FILES" ]; then
  for f in $INVALIDATED_FILES; do
    rm -f "$f"
    ((count++))
  done
fi
echo "invalidated $count files"

if [[ ${1-''} == 'modern' ]]; then
  ./build_modern.sh cached
else
  # rm -f *.out
  # rm -f *.n64
  wine cmd /c compile.bat
fi
