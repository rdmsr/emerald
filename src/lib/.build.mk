LIB_BIN = $(BUILD_DIRECTORY)/libemerald.a

LIBCFILES    := $(shell find src/lib -type f -name '*.c')

LIBOBJS = $(patsubst %.c, $(BUILD_DIRECTORY)/host/%.c.o, $(LIBCFILES))

CFILES += $(LIBCFILES)

HOST_DIRECTORY_GUARD = @mkdir -p $(@D)

HOST_CFLAGS := -DHOST -fPIE -Wno-sequence-point\
			-nostdlib \
			-g 	\
			-O2                                                    \
			-fno-stack-protector			\
			-Wall							\
			-Wextra							\
			-Werror						\
			-ffreestanding					\
			-std=gnu99						\
			-Isrc/lib					\


$(BUILD_DIRECTORY)/host/%.c.o: %.c
	$(HOST_DIRECTORY_GUARD)
	@echo -e CC $(ECHO) $<
	@gcc $(HOST_CFLAGS) -c $< -o $@


$(LIB_BIN): $(LIBOBJS)
	@echo -e AR $(ECHO) $@
	@ar -crs $@ $^

lib: $(LIB_BIN)
