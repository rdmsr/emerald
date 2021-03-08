project('emerald','c')

asm = run_command('../scripts/build/./asmfiles.sh')
c = run_command('../scripts/build/./cfiles.sh')
sources = c.stdout().strip().split('\n')
asm_files = asm.stdout().strip().split('\n')
nasm = find_program('nasm')
compiler = meson.get_compiler('c')

asm_gen = generator(nasm,
output: '@BASENAME@.asm.o',
arguments: ['-felf64','@INPUT@','-o','@OUTPUT@'])

message('Checking whether compiler supports standard headers...')

if compiler.has_header('stdint.h')
endif
if compiler.has_header('stddef.h')
endif
if compiler.has_header('stdbool.h')
endif

message('Checking host operating system... \033[1;1m@0@'.format(host_machine.system()))

if compiler.get_id() == 'clang'
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

else
   c_args = [
   '-ansi',
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
   c_link_args = ['-nostdlib','-static','-fno-pie','-z','max-page-size=0x1000','-T', '../kernel/link.ld']

   
endif

run_target('disk',command : '../scripts/build/make_disk.sh')
run_target('run', command: '../scripts/build/run.sh')
executable('kernel.elf',sources,asm_gen.process(asm_files),c_args: c_args,link_args: c_link_args)
