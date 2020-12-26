#include <stdint.h>
#ifndef ASM_H
#define ASM_H
unsigned char EmeraldASM_inb(unsigned short port);
void EmeraldASM_outb(uint16_t port, uint8_t value);
void EmeraldASM_sti();

#endif
