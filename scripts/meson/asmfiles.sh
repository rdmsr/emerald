#!/bin/sh

for i in $(find ../kernel -type f -name '*.asm'); do
  echo $i
done
