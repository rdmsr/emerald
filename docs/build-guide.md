# Build guide

## Table of contents
- Building emeraldOS
    -   [Dependencies](#dependencies)
    -   [Getting the source code](#getting-the-source-code)
    -   [Building](#building)
    -   [Running in a virtual machine](#running-in-a-virtual-machine)



## Dependencies

Building emeraldOS requires:

- A Linux distribution or any other UNIX-like operating system
- nasm (Assembler)
- LLVM (toolchain)
- GNU/Make (build system)
- Meson (build system)
- Ninja (build system used with meson)
- QEMU or any other virtualization softwarei

```sh
# On debian-based distributions
$ sudo apt install nasm llvm make meson ninja-build qemu-system-x86 
```

```sh
# on arch-based distributions
$ sudo pacman -S nasm llvm make meson ninja qemu
```

```sh
# on gentoo
$ sudo emerge --ask llvm nasm make meson ninja qemu
```

## Getting the source code

To get the source code, just clone the repository.

```sh
# Github
$ git clone https://github.com/Abb1x/emerald.git --recursive
```
```sh
# Gitlab (mirror)
$ git clone https://gitlab.com/Abb1x/emerald.git --recursive
```

## Building 

Building emerald is pretty simple, just run these commands:

```sh
$ cd emerald
$ make
```

## Running in a virtual machine

Emerald has only been tested on the following virtualization software:
- QEMU
- VirtualBox
- Other VMs might work

To run in QEMU, just execute the following command:

```sh
$ make run
```

To run in VirtualBox,  create a new machine with the following properties:

Make sure to run 
```sh
$ make disk
```
to generate the disk image

- Name: emerald (optional)
- Type: other
- Version: Other/Unknown (64 bit)
- Memory size: 64 mb 
- Hard Drive: use an existing virtual hard disk file -> choose the disk.hdd file in the build/ directory of the cloned repo

Thanks for building emerald! :heart:
