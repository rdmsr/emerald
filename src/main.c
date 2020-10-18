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

struct IDT_entry {
	unsigned short int offset_lowerbits;
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int offset_higherbits;
};


struct IDT_entry IDT[IDT_SIZE];


void idt_init(void)
{
	unsigned long keyboard_address;
	unsigned long idt_address;
	unsigned long idt_ptr[2];

	keyboard_address = (unsigned long)keyboard_handler;
	IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
	IDT[0x21].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x21].zero = 0;
	IDT[0x21].type_attr = INTERRUPT_GATE;
	IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;


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

	idt_address = (unsigned long)IDT ;
	idt_ptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
	idt_ptr[1] = idt_address >> 16 ;

	load_idt(idt_ptr);
}



void memory_manager(){}
void kmain(struct stivale_struct *bootloader_data)
{
	const char *str = "Welcome to abb1xOS!";
	clear_screen();
	kprint(str);
	kprint_newline();
	kprint_newline();
	idt_init();
	kb_init();

	while(1);
}
