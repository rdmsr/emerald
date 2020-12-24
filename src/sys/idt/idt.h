#include <stdint.h>
#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08
#pragma once
extern void isr(void);
extern void isr_irq_master(void);
extern void isr_irq_slave(void);
struct idt_descriptor
{
    uint16_t offset_lo;
    uint16_t cs;
    uint8_t ist;
    uint8_t attrib;
    uint16_t offset_mid;
    uint32_t offset_hi;
    uint32_t ignored;
} __attribute__((packed));
struct idt_pointer
{
    uint16_t size;
    uint64_t addr;
} __attribute__((packed));

void EmeraldSys_IDT_irq_remap(void);
void EmeraldSys_IDT_idt_register(uint16_t idx, void *handler, uint8_t cs, uint8_t attrib);
void EmeraldSys_IDT_isr_init(void);
void EmeraldSys_IDT_idt_load(void);
void EmeraldASM_sti();
void EmeraldSys_IDT_idt_init(void);
