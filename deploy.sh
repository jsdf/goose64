#!/bin/bash
set -eu

set +e
EVERDRIVE64DEVICE=$(diskutil list |grep EVERDRIVE64 | grep -Eo '(disk[0-9]s[0-9])')
GREP_ERROR=$?
set -e

echo $EVERDRIVE64DEVICE
if [ "$GREP_ERROR" -eq 1 ]; then
  echo "couldnt find EVERDRIVE64DEVICE"
  exit 1;
fi

diskutil mount /dev/$EVERDRIVE64DEVICE

./build.sh
for f in *.n64; do
  cp -- "$f" "/Volumes/EVERDRIVE64/homebrew/aaa_$f";
done
echo "unmounting $EVERDRIVE64DEVICE"
diskutil unmount /dev/$EVERDRIVE64DEVICE