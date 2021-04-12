ASMFILES  := $(shell find kernel/ libraries/ -type f -name '*.asm')
CFILES    := $(shell find kernel/ libraries/ -type f -name '*.c')
CC         = clang
OBJ       := ${CFILES:.c=.o} ${ASMFILES:.asm=.o}
KERNEL_HDD = build/disk.hdd
KERNEL_ELF = kernel.elf
NASMFLAGS := -felf64

ARCH = x86_64



CHARDFLAGS :=	-target ${ARCH}-unknown-none	\
			-ggdb							\
			-nostdlib						\
			-O3                                                     \
			-fno-stack-protector			\
			-Wall							\
			-Wextra							\
			-ffreestanding					\
			-ansi						\
			-mcmodel=kernel					\
			-Ikernel/								\
			-Ilibraries							\
			-fno-pic						\
			-mno-red-zone					\
			-mno-sse						\
			-mno-sse2						\

LDHARDFLAGS :=        \
        -target x86_64-unknown-none \
        -nostdlib                 \
	-static                   \
	-fno-pie                   \
	-z max-page-size=0x1000  \
	-T kernel/link.ld


.PHONY: clean

.DEFAULT_GOAL = $(KERNEL_HDD)

disk: $(KERNEL_HDD)

run: $(KERNEL_HDD)
	@echo [ QEMU ] $<
	@qemu-system-x86_64 -vga std -drive file=$(KERNEL_HDD),format=raw -enable-kvm -serial stdio -rtc base=localtime -m 256 -soundhw pcspk


debug: $(KERNEL_HDD)
	@echo [ QEMU ] $<
	@qemu-system-x86_64 -vga std -drive file=$(KERNEL_HDD),format=raw -d int -serial stdio -rtc base=localtime -m 256

%.o: %.c
	@echo [ CC ] $<
	@$(CC) $(CHARDFLAGS) -c $< -o $@
%.o: %.asm
	@echo [ NASM ] $<
	@nasm $(NASMFLAGS) $< -o $@


$(KERNEL_ELF): $(OBJ)

	@echo [ LD ] $<
	@$(CC) $(LDHARDFLAGS) $(OBJ) -o $@

limine/limine-install:
	$(MAKE) -C thirdparty/limine/ limine-install

$(KERNEL_HDD): $(KERNEL_ELF) limine/limine-install
	@echo [ LIMINE ] $(KERNEL_ELF)
	@-mkdir build
	@rm -f $(KERNEL_HDD)
	@dd if=/dev/zero bs=1M count=0 seek=64 of=$(KERNEL_HDD)
	@parted -s $(KERNEL_HDD) mklabel msdos
	@parted -s $(KERNEL_HDD) mkpart primary 1 100%
	@echfs-utils -m -p0 $(KERNEL_HDD) quick-format 32768
	@echfs-utils -m -p0 $(KERNEL_HDD) import $(KERNEL_ELF) build/$(KERNEL_ELF)
	@echfs-utils -m -p0 $(KERNEL_HDD) import limine.cfg limine.cfg
	@thirdparty/limine/limine-install limine/limine.bin $(KERNEL_HDD)

clean:
	rm -f $(KERNEL_HDD) $(KERNEL_ELF) $(OBJ)
