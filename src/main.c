
#include "devices/keyboard/keyboard.h"
#include "devices/video/vga.h"
#include "devices/video/colors.h"
#include "devices/serial/serial.h"
#include "inc/stivale.h"
#include "mem/pmm.h"
#include "sys/gdt.h"
#include "sys/idt.h"
#include <stdint.h>
#include "debug-utilities/logger.h"

#define VGA_ADDRESS 0xb8000

#define K 1024
#define M (1024 * K)

#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08
static char stack[4096] = { 0 };
__attribute__((section(".stivalehdr"), used)) struct stivale_header header = {
    .stack = (uintptr_t)stack + sizeof(stack),
    .framebuffer_bpp = 0,
    .framebuffer_width = 0,
    .framebuffer_height = 0,
    .flags = 0,
    .entry_point = 0
};

void kmain(struct stivale_struct* bootloader_data)
{
    EmeraldDevices_Serial_init_serial();
    EmeraldSys_GDT_gdt_init();
    EmeraldSys_IDT_irq_remap();
    kprint_newline();
    kprint_load("GDT", false);
    EmeraldSys_IDT_idt_init();
    kprint_load("IDT", false);
    EmeraldDevices_keyboard_Keyboard_init();
    kprint_load("Keyboard", false);
    EmeraldMem_PMM_pmm_init(1096 * M);
    kprint_load("PMM", false);
    kprint("Welcome to ", 15);
    kprint("EmeraldOS!", 10);
    log("hello %");
    while (1);

}
