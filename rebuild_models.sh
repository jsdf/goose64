#!/bin/bash

for f in *.obj; do
  lua wavefront64.lua obj "$f"
done


echo "rebuilt:"
for f in *.obj; do
  echo "$f"
done
