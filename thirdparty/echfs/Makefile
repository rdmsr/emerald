CC=cc
PREFIX=/usr/local
CFLAGS=-O3 -Wall -Wextra -pipe

.PHONY: all clean install

all: echfs-utils echfs-fuse mkfs.echfs

echfs-utils: echfs-utils.c part.c part.h
	$(CC) $(CFLAGS) part.c echfs-utils.c -luuid -o echfs-utils

echfs-fuse: echfs-fuse.c part.c part.h
	$(CC) $(CFLAGS) part.c echfs-fuse.c $(shell pkg-config fuse --cflags --libs) -o echfs-fuse

mkfs.echfs: mkfs.echfs.c
	$(CC) $(CFLAGS) mkfs.echfs.c -o mkfs.echfs

clean:
	rm -f echfs-utils
	rm -f echfs-fuse
	rm -f mkfs.echfs

install:
	install -d $(PREFIX)/bin
	install -s echfs-utils $(PREFIX)/bin
	install -s echfs-fuse $(PREFIX)/bin
	ln -s $(PREFIX)/bin/echfs-fuse $(PREFIX)/sbin/mount.echfs-fuse
	ln -s $(PREFIX)/bin/echfs-fuse $(PREFIX)/sbin/mount.echfs
	install -s mkfs.echfs $(PREFIX)/bin
