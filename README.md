<img src="https://media.discordapp.net/attachments/711643709753655350/779708791066984489/logo.png">

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/c01428b7b3864d1f86eb1753c6c8deaf)](https://app.codacy.com/gh/Abb1x/emerald?utm_source=github.com&utm_medium=referral&utm_content=Abb1x/emerald&utm_campaign=Badge_Grade)
![GitHub release (latest by date including pre-releases)](https://img.shields.io/github/v/release/Abb1x/emerald?include_prereleases)
![GitHub last commit](https://img.shields.io/github/last-commit/Abb1x/emerald)
![GitHub issues](https://img.shields.io/github/issues-raw/Abb1x/emerald)
![GitHub pull requests](https://img.shields.io/github/issues-pr/Abb1x/emerald)

### This is a basic x86_64 OS written in C
### How to build
get x86_64-elf-gcc [here](https://wiki.osdev.org/GCC_Cross-Compiler)
<br>
First clone the repo:
`git clone https://github.com/Abb1x/emerald.git`
<br>
Then you need to init the submodules(clone limine):
```cd emerald && git submodule init && git submodule update```
<br>
Now you need to compile the source code using the cross-compiler, modify the Makefile if needed

### Prebuilt images
You can use prebuilt .hdd images in the build/ directory I suggest you run them using qemu
### TODO

- [ ] VMM
- [ ] EXT2 filesystem
- [ ] Start Porting stuff
### Screenshots

<img src="https://cdn.discordapp.com/attachments/745313714747605105/773343861526364171/unknown.png">
<img src="https://media.discordapp.net/attachments/745313714747605105/773344029995040788/unknown.png">
