set -eu

INVALIDATED_FILES=`python3 get_changed_files.py`

if [ -n "$INVALIDATED_FILES" ]; then
  for f in $INVALIDATED_FILES; do
    rm -f "$f"
  done
  echo "invalidated files: $INVALIDATED_FILES"

  /Applications/Wine\ Stable.app/Contents/Resources/wine/bin/wine cmd /c compile.bat
fi
