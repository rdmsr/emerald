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

#ifndef VBE_H
#define VBE_H
#define RED_SHIFT 16
#define GREEN_SHIFT 8
#define BLUE_SHIFT 0

#include <boot/stivale2.h>
#include <stddef.h>

typedef struct
{
    uint8_t r, g, b;
} Color;

typedef struct
{
    size_t x, y;
} Position;

enum shapes
{
    RECTANGLE,
    RHOMBUS,
    TRIANGLE
};
void VBE_init(struct stivale2_struct *info);
void VBE_clear_screen(int info, Color color);
void VBE_putchar(char character, int position_x, int position_y, Color color);
void VBE_puts(char *string, Color color);
void VBE_put(char c, Color color);
void VBE_draw_pixel(int x, int y, uint32_t color);
void VBE_putf(char *format, ...);
void VBE_cputf(Color color, char *format, ...);
void VBE_display_circle(int xc, int yc, int radius);
void VBE_draw_line(int x0, int y0, int x1, int y1);
void VBE_draw_shape(int shape, int width, int height, int x, int y);
struct stivale2_struct_tag_framebuffer* VBE_get_fb_info();
#endif
