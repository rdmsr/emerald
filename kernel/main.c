/*-
 * SPDX-License-Identifier: MIT
 *
 * MIT License
 *
 * Copyright (c) 2020-2021 Abb1x
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

#include "boot/stivale2.h"
#include "system/GDT.h"
#include <ascii.h>
#include <boot/boot.h>
#include <devices/keyboard/keyboard.h>
#include <devices/pci/PCI.h>
#include <devices/pcspkr/pcspkr.h>
#include <devices/serial/serial.h>
#include <devices/video/framebuffer.h>
#include <libk/logging.h>
#include <libk/module.h>
#include <libk/random.h>
#include <libk/time/chrono.h>
#include <libk/time/time.h>
#include <memory/pmm.h>
#include <memory/vmm.h>
#include <stddef.h>
#include <stdint.h>
#include <system/interrupts/IDT.h>
#include <system/interrupts/PIT.h>

Color white = {255, 255, 255}, green = {0, 148, 99}, gray = {94, 94, 94};

void kmain(struct stivale2_struct *info)
{

    module("main");

    GDT_init();
    IDT_init();

    PIT_init(1000);

    Serial_init();

    Color colorscheme[8] = {
        rgb(88, 110, 117),  /* Black */
        rgb(220, 50, 47),   /* Red */
        rgb(133, 153, 0),   /* Green */
        rgb(181, 137, 0),   /* Yellow */
        rgb(38, 139, 210),  /* Blue */
        rgb(108, 113, 196), /* Magenta */
        rgb(42, 161, 152),  /* Cyan */
        rgb(253, 246, 227)  /* White */
    };


    Framebuffer fb = Framebuffer_init(colorscheme, info);
    Framebuffer_clear();

    glog(SUCCESS, "Framebuffer info:");
    glog(SILENT, "\tResolution: %dx%d", fb.fb_info->framebuffer_width, fb.fb_info->framebuffer_height);
    glog(SILENT, "\tPitch: %d", fb.fb_info->framebuffer_pitch);
    glog(SILENT, "\tBPP: %d\n", fb.fb_info->framebuffer_bpp);

    PCI_init();

    Boot_get_info(info);
    DateTime date = RTC_get_date_time();

    glog(SUCCESS, "Time info:");
    glog(SILENT, "\tDate: %x/%x/20%x", date.month, date.day, date.year);
    glog(SILENT, "\tTime: %d:%d:%d\n", date.time.hour, date.time.minute, date.time.second);

    srand(RTC_get_seconds());

    struct stivale2_struct_tag_memmap *memory_map = stivale2_get_tag(info, STIVALE2_STRUCT_TAG_MEMMAP_ID);
    
    PMM_init((void *)memory_map->memmap, memory_map->entries, boot_info);


    /*VMM_init();*/

    PCSpkr_init();
    Keyboard_init();

    glog(SUCCESS, "System booted in %dms", PIT_get_ticks());
    Framebuffer_puts("Welcome to ");
    Framebuffer_puts("\033[32mEmeraldOS!\n\033[0m");

    int c;
    for (c = 0; c < 7; c++)
    {
        glog(SILENT, "\033[%dmcolor\033[0m", c + 30);
    }

    set_ascii();

    while (1)
        ;
}
