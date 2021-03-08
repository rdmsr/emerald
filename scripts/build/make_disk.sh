#!/bin/sh
cd ../build
rm -rf disk.hdd
dd if=/dev/zero bs=1M count=0 seek=64 of=disk.hdd
parted -s disk.hdd mklabel msdos
parted -s disk.hdd mkpart primary 1 100%
../thirdparty/echfs/echfs-utils -m -p0 disk.hdd quick-format 32768
../thirdparty/echfs/echfs-utils -m -p0 disk.hdd import kernel.elf build/kernel.elf
../thirdparty/echfs/echfs-utils -m -p0 disk.hdd import ../limine.cfg limine.cfg
../thirdparty/limine/limine-install disk.hdd
