#!/bin/bash
set -eu

INVALIDATED_FILES=`python3 get_changed_files.py`

if [ -n "$INVALIDATED_FILES" ]; then
  for f in $INVALIDATED_FILES; do
    rm -f "$f"
  done

fi

if [[ $1 == 'modern' ]]; then
  ./build_modern.sh cached
else
  wine cmd /c compile.bat
fi
