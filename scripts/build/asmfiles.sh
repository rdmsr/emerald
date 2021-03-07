#!/bin/sh

for i in $(find ../kernel ../libraries -type f -name '*.asm'); do
  echo $i
done
