project('emerald','c')

asm = run_command('../scripts/meson/./asmfiles.sh')
c = run_command('../scripts/meson/./cfiles.sh')
sources = c.stdout().strip().split('\n')
asm_files = asm.stdout().strip().split('\n')
nasm = find_program('nasm')

asm_gen = generator(nasm,
output: '@BASENAME@.asm.o',
arguments: ['-felf64','@INPUT@','-o','@OUTPUT@'])

c_args = [
'-ansi',
'-target',
'x86_64-unknown-none-elf',
'-nostdlib',
'-Werror',
'-Wall',
'-Os',
'-Wextra',
'-fno-pic',
'-mno-sse',
'-mno-sse2',
'-mno-mmx',
'-mno-avx',
'-g',
'-mno-80387',
'-mno-red-zone',
'-mcmodel=kernel',
'-ffreestanding',
'-fno-stack-protector',
'-I../kernel/',
'-I../libraries/'
]
c_link_args = ['-nostdlib','-target','x86_64-unknown-none-elf','-static','-fno-pie','-z','max-page-size=0x1000','-T', '../kernel/link.ld']
executable('kernel.elf',sources,asm_gen.process(asm_files),c_args: c_args,link_args: c_link_args)
