#!/bin/bash
set -eu


if [[ ${1-''}  != 'cached' ]]; then
  rm -f *.o
fi

SDK_BASE_DIR="/Users/jfriend/.wine/drive_c"
export ROOT="${SDK_BASE_DIR}/ultra"
export N64KITDIR="${SDK_BASE_DIR}/nintendo/n64kit"

make
