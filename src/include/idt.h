#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08
#pragma once
struct idt_descriptor {
    uint16_t offset_lo;
    uint16_t cs; 
    uint8_t ist;
    uint8_t attrib;
    uint16_t offset_mid;
    uint32_t offset_hi;
    uint32_t ignored; 
} __attribute__((packed));
struct idt_pointer {
    uint16_t size;
    uint64_t addr;
} __attribute__((packed));

static struct idt_descriptor idt[256];

static struct idt_pointer idtr = {.size = 256 * sizeof(idt) - 1, .addr = (uint64_t)idt};

 

void irq_remap(void)
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
void idt_register(uint16_t idx, void *handler, uint8_t cs, uint8_t ist, uint8_t attrib) {
    uint64_t ptr = (uint64_t)handler;
    // each gdt entry is 8 bytes, so transform array index into byte index
    idt[idx] = (struct idt_descriptor){
        .offset_lo = ptr, .cs = cs, .attrib = attrib, .offset_mid = ptr >> 16, .offset_hi = ptr >> 32};
}


void idt_load() {
    asm volatile("lidt %0" : : "m"(idtr));
}

void sti() {
    asm volatile("sti");
}
void idt_init(void)
{
	unsigned long keyboard_address;
	unsigned long idt_address;
	unsigned long idt_ptr[2];

	idt_register(0x21, keyboard_handler_main, KERNEL_CODE_SEGMENT_OFFSET, 0, INTERRUPT_GATE);
	idt_register(0x21,isr,KERNEL_CODE_SEGMENT_OFFSET, 0, INTERRUPT_GATE);
	idt_load();

	sti();
}
