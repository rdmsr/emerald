/*-
 * SPDX-License-Identifier: MIT
 *
 * MIT License
 *
 * Copyright (c) 2020 Abb1x
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
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
#include <boot/boot.h>
#include <stdint.h>
#include <sys/firmware/legacy/bios.h>
#include <sys/gdt/gdt.h>
#include <sys/idt/idt.h>
#define M (1024 * 1024)
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

color_t white = {255, 255, 255}, green = {0, 148, 99}, gray = {94, 94, 94};

void print_load(char *string)
{
    EmeraldDevices_VBE_print(string, gray);
    EmeraldDevices_VBE_print(": ", gray);
    EmeraldDevices_VBE_print("Initialized ", green);
    EmeraldDevices_VBE_print(string, green);
    EmeraldDevices_VBE_print("\n", gray);
}

void init(struct stivale2_struct *info)
{
    EmeraldDevices_VBE_init(info);
    EmeraldBoot_Stivale_get_boot_info(info);
    EmeraldDevices_VBE_clear_screen();
    EmeraldDevices_keyboard_Keyboard_init();
    print_load("Keyboard");
    EmeraldDevices_Serial_init_serial();
    EmeraldSys_GDT_gdt_init();
    print_load("GDT");
    EmeraldSys_IDT_idt_init();
    print_load("IDT");
    EmeraldMem_PMM_pmm_init(1096 * M);
    print_load("PMM");
    EmeraldDevices_RTC_read_rtc();
    print_load("RTC");
    EmeraldMem_VMM_initialize();
    log(INFO, "Paging enabled");
    print_load("VMM");
}
void kmain(struct stivale2_struct *info)
{
    init(info);
    set_ascii();
    EmeraldSys_IDT_irq_remap();
    thread_t thread;
    EmeraldProc_Task_create_process(10, 20, 0x297DE000, thread, "process1");
    EmeraldProc_Task_create_process(20, 92, 0xFFF, thread, "process2");
    EmeraldProc_Task_create_process(30, 30, 0xFFF, thread, "process3");
    EmeraldProc_Scheduler_schedule_task();
    EmeraldDevices_VBE_print("Welcome to ", white);
    EmeraldDevices_VBE_print("EmeraldOS!\n", green);
    while (1);
}
