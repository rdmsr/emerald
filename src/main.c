#include "../kutils/keyboard_map.h"
#include "../kutils/kbutils.h"
#include "../kutils/cliutils.h"
#include "include/idt.h"
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
#define GDTBASE    0x00000800


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
//idt


//Global Descriptor table (GDT)
struct gdt_pointer {
    uint16_t limit;
    uint64_t base;
} __attribute__ ((packed));
struct gdt_descriptor {
    uint16_t limit_low16;
    uint16_t base_low16;
    uint8_t  base_mid8;
    uint8_t access;
    uint8_t granularity;
    uint8_t  base_high8;  
} __attribute__((packed));
struct gdt_descriptor gdt[8];
extern struct gdt_pointer gdtr = {.limit = sizeof(gdt) - 1, .base = (uint64_t)gdt};


void gdt_load() {
    asm volatile("lgdt %0" : : "m"(gdtr) : "memory");
    asm volatile (R"(
    mov %%rsp, %%rax
    push $0x10
    push %%rax
    pushf
    push $0x8
    push $1f
    iretq
    1:
    mov $0x10, %%ax
    mov %%ax, %%ds
    mov %%ax, %%es
    mov %%ax, %%ss
    mov %%ax, %%fs
    mov %%ax, %%gs
                )" : : : "rax", "memory");
}
void gdt_init(){
    gdt[1] = (struct gdt_descriptor) { .access = 0b10011010  ,.granularity = 0b00100000 };
    gdt[2] = (struct gdt_descriptor) { .access = 0b10010010  ,.granularity = 0 };
    gdt_load();
	}
void kmain(struct stivale_struct *bootloader_data)
{
	gdt_init();
	const char *str = "Welcome to abb1xOS!";
	clear_screen();

	kprint_newline();
	kprint_load("GDT");
	idt_init();
	kb_init();
	kprint_load("IDT");
	
	kprint(str,15);
	while(1);
}
