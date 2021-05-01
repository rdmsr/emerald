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
#include "font.h"
#include "psf.h"
#include <boot/stivale2.h>
#include <stddef.h>

typedef struct
{
    uint8_t r, g, b;
} Color;

typedef struct
{
    Color colorscheme[8], background, foreground;
    struct stivale2_struct_tag_framebuffer *fb_info;
    int cursor_y, cursor_x;
    PSF font;
} Framebuffer;

Framebuffer Framebuffer_init(Color colorscheme[8], struct stivale2_struct *info);

void Framebuffer_clear();
void Framebuffer_putc(char c, int position_x, int position_y, Framebuffer *self);
void Framebuffer_puts(char *string);
void Framebuffer_set(Framebuffer *self);
void Framebuffer_put(char c);

Color rgb(int r, int g, int b);

void glog(int level, char *format, ...);

enum glog_level
{
    SUCCESS,
    FAILURE,
    SILENT

};
#endif
