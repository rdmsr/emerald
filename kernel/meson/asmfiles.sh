#!/bin/sh

for i in $(find ../ -type f -name '*.asm'); do
  echo $i
done
