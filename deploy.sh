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
cp *.n64 /Volumes/EVERDRIVE64/homebrew/
echo "unmounting $EVERDRIVE64DEVICE"
diskutil unmount /dev/$EVERDRIVE64DEVICE