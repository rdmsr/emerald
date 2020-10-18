nasm -f elf32 src/kernel.asm -o src/kasm.o

make

#run it in qemu
#qemu-system-x86_64 -kernel kernel -cpu host -enable-kvm

