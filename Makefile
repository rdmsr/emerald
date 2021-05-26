ASMFILES  := $(shell find kernel/ libraries/ -type f -name '*.asm')
CFILES    := $(shell find kernel/ libraries/ -type f -name '*.c')
CC         = clang
OBJ       := ${CFILES:.c=.o} ${ASMFILES:.asm=.o}
KERNEL_HDD = build/disk.hdd
KERNEL_ELF = kernel.elf
NASMFLAGS := -felf64

ARCH = x86_64



CHARDFLAGS :=	-target ${ARCH}-unknown-none	\
			-nostdlib						\
			-g \
			-O2                                                     \
			-fno-stack-protector			\
			-Wall							\
			-Wextra							\
			-Werror							\
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
	-O2 \
        -target x86_64-unknown-none \
        -nostdlib                 \
	-static                   \
	-fno-pic -fpie                   \
	-z max-page-size=0x1000  \
	-T kernel/link.ld


ifndef ECHO
HIT_TOTAL != ${MAKE} ${MAKECMDGOALS} --dry-run ECHO="HIT_MARK" | grep -c "HIT_MARK"
HIT_COUNT = $(eval HIT_N != expr ${HIT_N} + 1)${HIT_N}
ECHO = "[\033[1;32m`expr ${HIT_COUNT} '*' 100 / ${HIT_TOTAL}`%\033[1;0m]"
endif

.PHONY: clean

.DEFAULT_GOAL = $(KERNEL_HDD)

disk: $(KERNEL_HDD)

run: $(KERNEL_HDD)
	@echo [ QEMU ] $<
	@qemu-system-x86_64 -vga std -drive file=$(KERNEL_HDD),format=raw -enable-kvm -serial stdio -rtc base=localtime -m 256 -soundhw pcspk

documentation/:

	@echo -e "Generating documentation to\033[1;32m" $@ "\033[0m"
	@doxygen > /dev/null 2>&1
	@echo -e "\033[1;36mDone!\033[0m"

docs: documentation/
debug: $(KERNEL_HDD)
	@echo [ QEMU ] $<
	@qemu-system-x86_64 -vga std -drive file=$(KERNEL_HDD),format=raw -d int -serial stdio -rtc base=localtime -m 256

%.o: %.c
	@echo -e CC $(ECHO) $<
	@$(CC) $(CHARDFLAGS) -c $< -o $@
%.o: %.asm
	@echo -e AS  $(ECHO) $<
	@nasm $(NASMFLAGS) $< -o $@


$(KERNEL_ELF): $(OBJ)

	@echo -e LD $(ECHO) $@
	@$(CC) $(LDHARDFLAGS) $(OBJ) -o $@

limine/limine-install:
	@$(MAKE) -C thirdparty/limine/ limine-install

$(KERNEL_HDD): limine/limine-install $(KERNEL_ELF)
	@echo -e LIMINE $(KERNEL_HDD)
	@-mkdir build > /dev/null 2>&1
	@rm -f $(KERNEL_HDD)
	@dd if=/dev/zero bs=1M count=0 seek=64 of=$(KERNEL_HDD) status=none
	@parted -s $(KERNEL_HDD) mklabel msdos
	@parted -s $(KERNEL_HDD) mkpart primary 1 100%
	@echfs-utils -m -p0 $(KERNEL_HDD) quick-format 32768
	@echfs-utils -m -p0 $(KERNEL_HDD) import $(KERNEL_ELF) build/$(KERNEL_ELF)
	@echfs-utils -m -p0 $(KERNEL_HDD) import limine.cfg limine.cfg
	@echfs-utils -m -p0 $(KERNEL_HDD) import thirdparty/limine/bin/limine.sys limine.sys
	@thirdparty/limine/bin/limine-install $(KERNEL_HDD)

clean:
	rm -f $(KERNEL_HDD) $(KERNEL_ELF) $(OBJ)
