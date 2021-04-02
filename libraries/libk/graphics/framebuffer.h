/*-
 * SPDX-License-Identifier: MIT
 *
 * MIT License
 *
 * Copyright (c) 2021 Abb1x
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

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <devices/video/vbe.h>
#include <libk/graphics/psf.h>
#include <stdint.h>

/* OOP C */
typedef struct framebuffer_struct
{

    /* Functions */
    void (*clear_screen)(struct framebuffer_struct *);
    void (*scroll)();
    void (*puts)(char *, struct framebuffer_struct *);
    void (*putchar)(uint8_t, Position, struct framebuffer_struct *);
    /* Framebuffer info */
    int width, height;
    uint64_t address;
    uint16_t pitch;
    uint16_t bpp;

    /* Colors */
    Color fg_color, bg_color;

    /* Font */
    PSF font;
} Framebuffer;

Framebuffer _Framebuffer(struct stivale2_struct *info);
Framebuffer Framebuffer_get_current();

#endif
