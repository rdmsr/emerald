#include "../kutils/keyboard_map.h"
#include "../kutils/kbutils.h"
#include <stivale.h>
#include <stdint.h>
#define VGA_ADDRESS 0xb8000
#define VGA_COLOR(character, color) ((uint16_t) (character) | (uint16_t) (color) << 8)
#define VGA_BLACK        0c
#define VGA_BLUE         1
#define VGA_GREEN        2
#define VGA_CYAN         3
#define VGA_RED          4
#define VGA_PURPLE       5
#define VGA_BROWN        6
#define VGA_GRAY         7
#define VGA_DARK_GRAY    8
#define VGA_LIGHT_BLUE   9
#define VGA_LIGH_GREEN   10
#define VGA_LIGHT_CYAN   11
#define VGA_LIGHT_RED    12
#define VGA_LIGHT_PURPLE 13
#define VGA_YELLOW       14
#define VGA_WHITE        15



#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08


extern void load_idt(unsigned long *idt_ptr);

static char stack[4096] = {0};

__attribute__((section(".stivalehdr"), used))
struct stivale_header header = {
    .stack = (uintptr_t)stack + sizeof(stack),
    .framebuffer_bpp = 0,
    .framebuffer_width = 0,
    .framebuffer_height = 0,
    .flags = 0,
    .entry_point = 0
};

struct idt_descriptor {
    uint16_t offset_lo;
    uint16_t cs; // Code segment selector of the ISR
    uint8_t ist; // Interrupt stack offset
    uint8_t attrib;
    uint16_t offset_mid;
    uint32_t offset_hi;
    uint32_t ignored; // Set to zero
} __attribute__((packed));

// The pointer to the IDT structure; it's officially known as the IDTR
struct idt_pointer {
    uint16_t size;  // size - 1
    uint64_t addr;
} __attribute__((packed));

static struct idt_descriptor idt[256];
static struct idt_pointer idtr = {.size = sizeof(idt) - 1, .addr = (uint64_t)idt};

void idt_register(uint16_t idx, void *handler, uint8_t cs, uint8_t ist, uint8_t attrib) {
    uint64_t ptr = (uint64_t)handler;
    // each gdt entry is 8 bytes, so transform array index into byte index
    idt[idx] = (struct idt_descriptor){
        .offset_lo = ptr, .cs = 8 * cs, .attrib = attrib, .offset_mid = ptr >> 16, .offset_hi = ptr >> 32};
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

	idt_register(0x21, keyboard_handler, KERNEL_CODE_SEGMENT_OFFSET, 0, INTERRUPT_GATE);

	write_port(0x20 , 0x11);
	write_port(0xA0 , 0x11);

	write_port(0x21 , 0x20);
	write_port(0xA1 , 0x28);

	write_port(0x21 , 0x00);
	write_port(0xA1 , 0x00);

	write_port(0x21 , 0x01);
	write_port(0xA1 , 0x01);



	write_port(0x21 , 0xff);
	write_port(0xA1 , 0xff);
	idt_load();
}



void memory_manager(){/*TODO*/}
void kmain(struct stivale_struct *bootloader_data)
{
	const char *str = "Welcome to abb1xOS!";
	clear_screen();
	kprint(str);
	kprint_newline();
	kprint_newline();
	idt_init();
	kb_init();
	sti();
	while(1);
}
