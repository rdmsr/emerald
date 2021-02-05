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

#include <stdbool.h>
#include "boot/stivale2.h"
#include "system/GDT.h"
#include <ascii.h>
#include <boot/boot.h>
#include <devices/keyboard/keyboard.h>
#include <devices/pci/PCI.h>
#include <devices/serial/serial.h>
#include <devices/video/vbe.h>
#include <libk/logging.h>
#include <libk/module.h>
#include <memory/pmm.h>
#include <memory/vmm.h>
#include <stddef.h>
#include <stdint.h>
#include <system/interrupts/IDT.h>
#include <system/interrupts/PIT.h>
#include <libegl/egl.h> /* Graphics Library */

color_t white = {255, 255, 255}, green = {0, 192, 99}, grey = {157, 157, 157};

void kmain(struct stivale2_struct *info)
{
    module("main");
    
    char VER[] = "0.0.3";
    char REV[] = "1";
    
    PIT_init(1000);

    log(INFO, "Ticks %d", tick);
    GDT_init();
    IDT_init();

    Serial_init();

    info = (void *)info + MEM_OFFSET;

    VBE_init(info);
    
    PCI_init();
    
    BootInfo *boot_info = Boot_get_info(info);

    PMM_init((void *)boot_info->memory_map, boot_info->memory_map->entries);

    /*VMM_init();*/

    Keyboard_init();

    module("main");

    #ifdef EGL_H
    VBE_putf("EGL version: "); VBE_putf(EGL_VER); VBE_put('\n', white);
    #endif

    VBE_puts("\nWelcome to ", white);
    VBE_puts("\x04 EmeraldOS", green);
    VBE_puts(" Version ", grey); VBE_puts(VER, grey);
    VBE_puts(" Revision ", grey); VBE_puts(REV, grey);
    VBE_puts("\n\n", white);

    log(INFO, "Ticks %d", tick);

    VBE_putf("Took %dms to boot\n\n", tick);

    set_ascii();
    
    randByte(); /* Seed random number generator */
    
    VBE_set_fgc(white);
    EGL_randFill(255, 255, 512, 512);
    EGL_drawFCircle(384, 384, randByte() >> 1, EGL_randColor());
    EGL_drawCircle(384, 384, randByte() >> 1, EGL_randColor());
    EGL_drawFTriangle(randByte() + 256, randByte() + 256, randByte() + 256, randByte() + 256, randByte() + 256, randByte() + 256, EGL_randColor());
    EGL_drawTriangle(randByte() + 256, randByte() + 256, randByte() + 256, randByte() + 256, randByte() + 256, randByte() + 256, EGL_randColor());
    EGL_drawFRectangle(randByte() + 256, randByte() + 256, randByte() + 256, randByte() + 256, EGL_randColor());
    EGL_drawRectangle(randByte() + 256, randByte() + 256, randByte() + 256, randByte() + 256, EGL_randColor());

    while (1)
    {
        VBE_put(getKey(), white);/*
        color_t tmpcolor = {randByte(), randByte(), randByte()};
        EGL_drawCircle(randByte(), randByte(), randByte(), get_color(&tmpcolor));
        EGL_drawLine(randByte(), randByte(), randByte(), randByte(), get_color(&tmpcolor));*/
    };
}
