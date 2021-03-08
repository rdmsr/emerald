KERNEL_HDD = build/disk.hdd
JOBS := $(shell nproc)
.PHONY: all clean
all:
	ninja -j$(JOBS) -C build/
disk: 
	@ninja -j$(JOBS) -C build/ disk
run: disk
	@ninja -j$(JOBS) -C build/ run
debug:
	@qemu-system-x86_64 -d int -vga std -drive file=$(KERNEL_HDD),format=raw -serial stdio -rtc base=localtime -m 256 -soundhw pcspk

clean:
	@ninja -C build/ clean

