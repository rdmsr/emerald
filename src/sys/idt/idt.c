#include "idt.h"
#include <debug-utilities/logger.h>
#include <devices/keyboard/keyboard.h>
#include <stdint.h>
//unsigned int current_loc = 0;
//char *vidptr = (char*)0xb8000;
static struct idt_descriptor idt[256];
static struct idt_pointer idtr = {.size = 256 * sizeof(struct idt_descriptor), .addr = (uint64_t)idt};

void EmeraldSys_IDT_irq_remap(void)
{
    EmeraldASM_outb(0x20, 0x11);
    EmeraldASM_outb(0xA0, 0x11);

    EmeraldASM_outb(0x21, 0x20);
    EmeraldASM_outb(0xA1, 0x28);

    EmeraldASM_outb(0x21, 0x04);
    EmeraldASM_outb(0xA1, 0x02);

    EmeraldASM_outb(0x21, 0x01);
    EmeraldASM_outb(0xA1, 0x01);

    EmeraldASM_outb(0x21, 0x0);
    EmeraldASM_outb(0xA1, 0x0);
    log(INFO, "IRQs Remapped");
}
void EmeraldSys_IDT_idt_register(uint16_t idx, void *handler, uint8_t cs,uint8_t attrib)
{
    uint64_t ptr = (uint64_t)handler;
    idt[idx] = (struct idt_descriptor){
        .offset_lo = ptr, .cs = cs, .attrib = attrib, .offset_mid = ptr >> 16, .offset_hi = ptr >> 32};
}
void EmeraldSys_IDT_isr_init(void)
{
    for (int i = 0; i < 0x21; i++)
    {
        EmeraldSys_IDT_idt_register(i, isr, KERNEL_CODE_SEGMENT_OFFSET, INTERRUPT_GATE);
    }
    EmeraldSys_IDT_idt_register(0x20, isr_irq_master, KERNEL_CODE_SEGMENT_OFFSET, INTERRUPT_GATE);
    EmeraldSys_IDT_idt_register(0x21, isr, KERNEL_CODE_SEGMENT_OFFSET, INTERRUPT_GATE);

    for (int i = 0x22; i < 0x28; i++)
    {
        EmeraldSys_IDT_idt_register(i, isr_irq_master, KERNEL_CODE_SEGMENT_OFFSET, INTERRUPT_GATE);
    }
    for (int i = 0x28; i < 0x2F; i++)
    {
        EmeraldSys_IDT_idt_register(i, isr_irq_slave, KERNEL_CODE_SEGMENT_OFFSET, INTERRUPT_GATE);
    }
    for (int i = 0x30; i < 256; i++)
    {
        EmeraldSys_IDT_idt_register(i, EmeraldDevices_keyboard_Keyboard_handler_main, KERNEL_CODE_SEGMENT_OFFSET, /*0,*/ INTERRUPT_GATE);
    }
}

void EmeraldSys_IDT_idt_load(void)
{
    asm volatile("lidt %0"
                 :
                 : "m"(idtr));
}

void EmeraldSys_IDT_idt_init(void)
{
    EmeraldSys_IDT_isr_init();
    EmeraldSys_IDT_idt_load();
    EmeraldASM_sti();
    log(INFO, "Initializing IDT...\033[0;37m loading ISR... loading IDT... \033[1;0mDone");
}
