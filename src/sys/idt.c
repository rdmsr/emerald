#include "idt.h"
#include "../devices/keyboard/keyboard.h"
#include <stdint.h>
//unsigned int current_loc = 0;
extern char read_port(unsigned short port);
//char *vidptr = (char*)0xb8000;
void EmeraldSys_IDT_irq_remap(void)
{
	outb(0x20, 0x11);
	outb(0xA0, 0x11);

	outb(0x21, 0x20);
	outb(0xA1, 0x28);

	outb(0x21, 0x04);
	outb(0xA1, 0x02);

	outb(0x21, 0x01);
	outb(0xA1, 0x01);

	outb(0x21, 0x0);
	outb(0xA1, 0x0);
}
void EmeraldSys_IDT_idt_register(uint16_t idx, void *handler, uint8_t cs, uint8_t ist, uint8_t attrib) {
    uint64_t ptr = (uint64_t)handler;
    idt[idx] = (struct idt_descriptor){
        .offset_lo = ptr, .cs = cs, .attrib = attrib, .offset_mid = ptr >> 16, .offset_hi = ptr >> 32};
}
void EmeraldSys_IDT_isr_init(void) {
    for (int i = 0; i < 0x21; i++) {
        EmeraldSys_IDT_idt_register(i,isr,KERNEL_CODE_SEGMENT_OFFSET, 0, INTERRUPT_GATE);
    }
    EmeraldSys_IDT_idt_register(0x20,isr_irq_master,KERNEL_CODE_SEGMENT_OFFSET, 0, INTERRUPT_GATE);
    EmeraldSys_IDT_idt_register(0x21, isr, KERNEL_CODE_SEGMENT_OFFSET, 0, INTERRUPT_GATE);

    for (int i = 0x22; i < 0x28; i++) {
        EmeraldSys_IDT_idt_register(i,isr_irq_master,KERNEL_CODE_SEGMENT_OFFSET, 0, INTERRUPT_GATE);
    }
    for (int i = 0x28; i < 0x2F; i++) {
        EmeraldSys_IDT_idt_register(i,isr_irq_slave,KERNEL_CODE_SEGMENT_OFFSET, 0, INTERRUPT_GATE);
    }
    for (int i = 0x30; i < 256; i++) {
        EmeraldSys_IDT_idt_register(i,EmeraldDevices_keyboard_Keyboard_handler_main,KERNEL_CODE_SEGMENT_OFFSET, 0, INTERRUPT_GATE);
    }
}

void EmeraldSys_IDT_idt_load(void) {
    asm volatile("lidt %0" : : "m"(idtr));
}
void EmeraldASM_sti() {
    asm volatile("sti");
}
void EmeraldSys_IDT_idt_init(void)
{
	unsigned long keyboard_address;
	unsigned long idt_address;
	unsigned long idt_ptr[2];

	EmeraldSys_IDT_isr_init();
	EmeraldSys_IDT_idt_load();

	EmeraldASM_sti();
}
