
#ifndef KERNEL_ASM_H
#define KERNEL_ASM_H
#include <emerald/std.h>

#define ASM_CR0_MONITOR_CO_PROCESSOR (1 << 1)
#define ASM_CR0_EMULATION (1 << 2)
#define ASM_CR0_NATIVE_EXCEPTION (1 << 5)

#define ASM_CR4_FXSR (1 << 9)
#define ASM_CR4_UNMASKED_FPU_EXCEPTION (1 << 10)
#define ASM_CR4_XSAVE (1 << 18)

void asm_cli(void);
void asm_sti(void);
void asm_hlt(void);

u8 asm_inb(u16 port);
void asm_outb(u16 port, u8 data);

void asm_xsave(void *region);
void asm_xrstor(void *region);
void asm_fninit(void);
void asm_fxsave(void *region);
void asm_fxrstor(void *region);

u64 asm_read_cr0(void);
u64 asm_read_cr1(void);
u64 asm_read_cr2(void);
u64 asm_read_cr3(void);
void asm_write_cr0(u64 value);
void asm_write_cr1(u64 value);
void asm_write_cr2(u64 value);
void asm_write_cr3(u64 value);
u64 asm_read_msr(u32 msr);
void asm_write_msr(u32 msr, u64 value);

#endif
