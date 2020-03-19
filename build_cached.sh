set -eu

CHANGED_C_FILES=`python3 get_changed_files.py`

if [ -n "$CHANGED_C_FILES" ]; then
  for f in $CHANGED_C_FILES; do
    rm -f -- "${f%.c}.o"
  done

  /Applications/Wine\ Stable.app/Contents/Resources/wine/bin/wine cmd /c compile.bat
fi
