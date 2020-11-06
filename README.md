

<h1><img src="https://cdn.discordapp.com/attachments/765557872682729479/774391211288821780/logo.png" width="25"> EmeraldOS</h1>

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/c01428b7b3864d1f86eb1753c6c8deaf)](https://app.codacy.com/gh/Abb1x/emerald?utm_source=github.com&utm_medium=referral&utm_content=Abb1x/emerald&utm_campaign=Badge_Grade)


### This is a basic x86_64 OS written in C

Build Requirements : 
```
Linux distro(Debian,Arch...)
x86_64-elf-gcc
Qemu
Limine(Bootloader)
```
### How to build:

install x86_64-elf-gcc. 
For arch users:
`yay -S x86_64-elf-gcc`
For other distros:
use the make_toolchain script using:
`sh make_toolchain.sh`

### Prebuilt images
You can use prebuilt .hdd images in the build/ directory I suggest you run them using qemu
### TODO

- [ ] VMM
- [ ] EXT2 filesystem
- [ ] Start Porting stuff
### Screenshots:

<img src="https://cdn.discordapp.com/attachments/745313714747605105/773343861526364171/unknown.png">
<img src="https://media.discordapp.net/attachments/745313714747605105/773344029995040788/unknown.png">


