#!/bin/bash
set -eu

export SDK_BASE_DIR="/workspace"
export ROOT="${SDK_BASE_DIR}/ultra"
export N64KITDIR="${SDK_BASE_DIR}/nintendo/n64kit"

make
