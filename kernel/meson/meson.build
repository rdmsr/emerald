project('emerald','c')

asm = run_command('./asmfiles.sh')
c = run_command('./cfiles.sh')
sources = c.stdout().strip().split('\n')
asm_files = asm.stdout().strip().split('\n')
nasm = find_program('nasm')

asm_gen = generator(nasm,
output: '@BASENAME@.asm.o',
arguments: ['-felf64','@INPUT@','-o','@OUTPUT@'])

c_args = run_command('cat','args.txt').stdout().strip().split('\n')
c_link_args = ['-nostdlib','-target','x86_64-unknown-none-elf','-static','-fno-pie','-z','max-page-size=0x1000','-T', '../kernel/link.ld']
executable('kernel.elf',sources,asm_gen.process(asm_files),c_args: c_args,link_args: c_link_args)
