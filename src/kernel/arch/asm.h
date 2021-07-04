
#ifndef KERNEL_ASM_H
#define KERNEL_ASM_H
#include <stdint.h>

#define ASM_CR0_MONITOR_CO_PROCESSOR (1 << 1)
#define ASM_CR0_EMULATION (1 << 2)
#define ASM_CR0_NATIVE_EXCEPTION (1 << 5)

#define ASM_CR4_FXSR (1 << 9)
#define ASM_CR4_UNMASKED_FPU_EXCEPTION (1 << 10)
#define ASM_CR4_XSAVE (1 << 18)

void asm_cli(void);
void asm_sti(void);
void asm_hlt(void);

uint8_t asm_inb(uint16_t port);
void asm_outb(uint16_t port, uint8_t data);

void asm_xsave(void *region);
void asm_xrstor(void *region);
void asm_fninit(void);
void asm_fxsave(void *region);
void asm_fxrstor(void *region);

uint64_t asm_read_cr0(void);
uint64_t asm_read_cr1(void);
uint64_t asm_read_cr2(void);
uint64_t asm_read_cr3(void);
void asm_write_cr0(uint64_t value);
void asm_write_cr1(uint64_t value);
void asm_write_cr2(uint64_t value);
void asm_write_cr3(uint64_t value);
#endif
