# echfs

The echfs filesystem is a 64-bit FAT-like filesystem which aims to support
most UNIX and POSIX-style features while being extremely simple to implement.
Ideal for hobbyist OS developers who want a simple filesystem and don't want
to deal with old crufty FAT (which isn't UNIX/POSIX compliant either),
or complex filesystems such as ext2/3/4.

Keep in mind that this is still a work in progress, and the specification might change.
I'll try to keep everything backwards compatible (in a clean way)
when I add new features or make modifications to the filesystem.

In this repo you can find the full specification in the `spec.txt` file,
and a utility to manipulate the filesystem (`echfs-utils`).
You can compile and install the `echfs-utils` program using `make` the usual way.

A FUSE implementation of a filesystem driver named `echfs-fuse` is also provided (thanks to Geertiebear).

```
make
sudo make install
```

## TODO:

Improve and add instructions to `echfs-utils`.
