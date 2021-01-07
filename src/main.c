#include <ascii.h>
#include <debug-utilities/logger.h>
#include <devices/RTC/rtc.h>
#include <devices/keyboard/keyboard.h>
#include <devices/serial/serial.h>
#include <devices/video/colors.h>
#include <devices/video/vbe/vbe.h>
#include <devices/video/vga/vga.h>
#include <inc/stivale2.h>
#include <libint/int.h>
#include <libstr/string.h>
#include <mem/physical/pmm.h>
#include <mem/virtual/vmm.h>
#include <proc/task.h>
#include <stdint.h>
#include <sys/firmware/legacy/bios.h>
#include <sys/gdt/gdt.h>
#include <sys/idt/idt.h>
#define K 1024
#define M (1024 * K)
#define MSIZE 48 * M
void kmain();
static uint8_t stack[4096] = {0};
struct stivale2_header_tag_framebuffer fb_request = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
        .next = 0},
    .framebuffer_height = 0,
    .framebuffer_width = 0};
struct stivale2_header_tag_smp smp_request = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_SMP_ID,
        .next = (uint64_t)&fb_request},
    .flags = 0};

__attribute__((section(".stivale2hdr"), used)) struct stivale2_header header2 = {
    .entry_point = (uint64_t)kmain,
    .stack = (uintptr_t)stack + sizeof(stack),
    .flags = 0,
    .tags = (uint64_t)&smp_request,

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
    EmeraldDevices_RTC_read_rtc();
}

void kmain(struct stivale2_struct *info)
{
    EmeraldDevices_VBE_init(info);
    init();
    set_ascii();
    EmeraldMem_VMM_initialize();

    log(INFO, "Paging enabled");
    EmeraldSys_IDT_irq_remap();
    EmeraldDevices_VGA_enable_cursor(10, 20);
    EmeraldDevices_VGA_update_cursor(0, 0);
    kprint("Welcome to ", 15);
    kprint("EmeraldOS!", 10);
    thread_t thread;
    EmeraldProc_Task_create_process(10, 20, 0x297DE000, thread, "process1");
    EmeraldProc_Task_create_process(20, 92, 0xFFF, thread, "process2");
    EmeraldProc_Task_create_process(30, 30, 0xFFF, thread, "process3");
    EmeraldProc_Scheduler_schedule_task();
    vbe_clear_screen();
    while (1);
}
