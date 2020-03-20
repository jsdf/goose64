#!/bin/bash
set -eu

# ./build.sh
for f in *.n64; do
  romfile="$f"
done

function fail {
  echo $1 >&2
  exit 1
}

function retry {
  local n=1
  local max=5
  local delay=1
  while true; do
    "$@" && break || {
      if [[ $n -lt $max ]]; then
        ((n++))
        echo "Command failed. Attempt $n/$max:"
        sleep $delay;
      else
        fail "The command has failed after $n attempts."
      fi
    }
  done
}

# loader64 can be found here: https://github.com/jsdf/loader64
 
# upload rom
retry loader64 -v --write --file "$romfile"

sleep 1;
# boot rom that was uploaded
retry loader64 -v --pifboot

# tail the logger
if [ -z "${PROFILE-}" ]; then
  ed64log
else
  ed64log | tee trace.log
  echo ""
  echo "unpacking trace"
  python3 process_trace.py trace.log trace.json
  echo "converting trace to html"
  $CATAPULT/tracing/bin/trace2html trace.json --output=trace.html
  open trace.html
fi
