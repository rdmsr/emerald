
#include "debug-utilities/logger.h"
#include "devices/keyboard/keyboard.h"
#include "devices/serial/serial.h"
#include "devices/video/colors.h"
#include "devices/video/vga/vga.h"
#include "inc/stivale2.h"
#include "mem/pmm.h"
#include "sys/gdt.h"
#include "sys/idt.h"
#include <stdint.h>

#define VGA_ADDRESS 0xb8000

#define K 1024
#define M (1024 * K)
#define MSIZE 48 * M
#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08
void kmain(struct stivale2_struct* info);
static uint8_t stack[4096] = { 0 };
struct stivale2_header_tag_smp smp_request = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_SMP_ID,
        .next = 0 },
    .flags = 0
};

__attribute__((section(".stivale2hdr"), used)) struct stivale2_header header2 = {
    .entry_point = (uint64_t)kmain,
    .stack = (uintptr_t)stack + sizeof(stack),
    .flags = 0,
    .tags = (uint64_t)&smp_request
};
void init()
{
    EmeraldDevices_keyboard_Keyboard_init();
    kprint_newline();
    kprint_load("Keyboard", false);
    EmeraldDevices_Serial_init_serial();
    EmeraldSys_GDT_gdt_init();
    kprint_load("GDT", false);

    EmeraldSys_IDT_idt_init();
    kprint_load("IDT", false);
    EmeraldMem_PMM_pmm_init(1096 * M);
    kprint_load("PMM", false);
}
void kmain(struct stivale2_struct* info)
{
    init();

    EmeraldSys_IDT_irq_remap();
    EmeraldDevices_VGA_enable_cursor(10, 20);
    EmeraldDevices_VGA_update_cursor(0, 0);
    kprint("Welcome to ", 15);
    kprint("EmeraldOS!", 10);
    while (1)
        ;
}
