#!/bin/sh
cd ../.. && clang-format --style=file -i $(find kernel/ libraries/ -type f -name '*.c')

