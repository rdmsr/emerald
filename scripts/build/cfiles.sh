#!/bin/sh

for i in $(find ../kernel/ ../libraries/ -type f -name '*.c'); do
  echo $i
done
