.DEFAULT_TARGET: test

CFILES := $(wildcard *.c)
OFILES = $(patsubst %.c, $(BUILD_DIRECTORY)/%.o, $(CFILES))

BUILD_DIRECTORY := build
DIRECTORY_GUARD = @mkdir -p $(@D)

test: $(OFILES)
	gcc $^ -L../../build -l:libemerald.a -o $@

$(BUILD_DIRECTORY)/%.o: %.c
	$(DIRECTORY_GUARD)
	gcc -I../lib -DHOST -c $< -o $@
run: 
	@./test
clean:
	rm -rf test build
