CC         = x86_64-elf-gcc
LD 	   = x86_64-elf-ld
AS  	   = nasm

ASMFILES  := $(shell find src -type f -name '*.asm')
CFILES    := $(shell find src -type f -name '*.c')

OBJ = $(patsubst %.c, $(BUILD_DIRECTORY)/%.c.o, $(CFILES)) \
        $(patsubst %.asm, $(BUILD_DIRECTORY)/%.asm.o, $(ASMFILES))

TARGET = $(BUILD_DIRECTORY)/kernel.elf
ISO = emerald.iso
MEMORY = 256

export PATH := $(shell meta/toolchain/use.sh):$(PATH)


CHARDFLAGS := -Wno-sequence-point \
			-nostdlib \
			-g 	\
			-O2                                                     \
			-fno-stack-protector			\
			-Wall							\
			-Wextra							\
			-Werror						\
			-ffreestanding					\
			-std=gnu99						\
			-mcmodel=kernel					\
			-Isrc/kernel					\
			-Isrc/lib					\
			-fsanitize=undefined \
			-fno-pic						\
			-mno-red-zone					\
			-mno-sse						\
			-mno-sse2						\

LDHARDFLAGS := \
	-O2 \
	-nostdlib                 \
	-static                   \
	-z max-page-size=0x1000  \
	-T src/link.ld

NASMFLAGS := -felf64

BUILD_DIRECTORY := build
DIRECTORY_GUARD = @mkdir -p $(@D)

ifndef ECHO
HIT_TOTAL != ${MAKE} ${MAKECMDGOALS} --dry-run ECHO="HIT_MARK" | grep -c "HIT_MARK"
HIT_COUNT = $(eval HIT_N != expr ${HIT_N} + 1)${HIT_N}
ECHO = "[\033[1;32m`expr ${HIT_COUNT} '*' 100 / ${HIT_TOTAL}`%\033[1;0m]"
endif

.PHONY: clean

.DEFAULT_GOAL = $(ISO)

emerald.iso: $(TARGET)
	meta/scripts/make-image.sh > /dev/null 2>&1

run: emerald.iso
	@echo [ QEMU ] $<
	@qemu-system-x86_64 -cdrom emerald.iso -enable-kvm -serial stdio -rtc base=localtime -m $(MEMORY)


debug: emerald.iso
	@echo [ QEMU ] $<
	@qemu-system-x86_64 -cdrom emerald.iso -d int -serial stdio -rtc base=localtime -m $(MEMORY)

$(BUILD_DIRECTORY)/%.c.o: %.c
	$(DIRECTORY_GUARD)
	@echo -e CC $(ECHO) $<
	@$(CC) $(CHARDFLAGS) -c $< -o $@

$(BUILD_DIRECTORY)/%.asm.o: %.asm
	$(DIRECTORY_GUARD)
	@echo -e AS  $(ECHO) $<
	@nasm $(NASMFLAGS) $< -o $@


$(TARGET): $(OBJ)
	@echo -e LD $(ECHO) $@
	@$(LD) $(LDHARDFLAGS) $(OBJ) -o $@
clean:
	rm -rf $(BUILD_DIRECTORY) $(TARGET) $(ISO)
