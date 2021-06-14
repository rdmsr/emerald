#ifndef KERNEL_ASM_H
#define KERNEL_ASM_H
#include <stdint.h>

void cli(void);
void sti(void);
void hlt(void);

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t data);


#endif
