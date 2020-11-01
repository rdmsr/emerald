
#include "devices/keyboard/keyboard.h"
#include "sys/idt.h"
#include "sys/gdt.h"
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
#define VGA_LIGHT_GREEN   10
#define VGA_LIGHT_CYAN   11
#define VGA_LIGHT_RED    12
#define VGA_LIGHT_PURPLE 13
#define VGA_YELLOW       14
#define VGA_WHITE        15



#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08


extern void load_idt(unsigned long *idt_ptr);
extern void isr(void);
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


void kmain(struct stivale_struct *bootloader_data)
{
	EmeraldSys_GDT_gdt_init();
    EmeraldSys_IDT_irq_remap();
	const char *str = "Welcome to ";
	clear_screen();

	kprint_newline();
	kprint_load("GDT",false);
	EmeraldSys_IDT_idt_init();
	kprint_load("IDT",false);
	EmeraldDevices_keyboard_Keyboard_init();
	kprint_load("Keyboard",true);
	
	kprint(str,15);
	kprint("EmeraldOS!",10);
	kprint_newline();

	while(1);
}
