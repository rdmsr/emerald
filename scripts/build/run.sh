#!/bin/sh
cd ../build
qemu-system-x86_64 -vga std -drive file=disk.hdd,format=raw -enable-kvm -serial stdio -rtc base=localtime -m 256 -soundhw pcspk
