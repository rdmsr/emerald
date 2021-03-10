#!/bin/sh
cd ../build
qemu-system-x86_64 -vga std -device ahci,id=ahci -drive file=disk.hdd,format=raw -enable-kvm -serial stdio -rtc base=localtime -m 256 -soundhw pcspk # -machine q35 # newer style emulation

