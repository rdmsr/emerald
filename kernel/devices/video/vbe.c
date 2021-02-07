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

#include "vbe.h"
#include "font.h"
#include <libk/logging.h>
#include <stddef.h>

struct stivale2_struct_tag_framebuffer *fb_info;

color_t bg_color = {0, 64, 73};
color_t fg_color = {0, 111, 92};
static color_t white = {255, 255, 255};
size_t cursor_x = 5;
size_t cursor_y = 5;

uint32_t get_color(color_t *color)
{
    return (uint32_t)((color->r << RED_SHIFT) | (color->g << GREEN_SHIFT) | (color->b << BLUE_SHIFT));
}

void VBE_draw_pixel(int x, int y, uint32_t color)
{
    size_t fb_i = x + (fb_info->framebuffer_pitch / sizeof(uint32_t)) * y;
    uint32_t *fb = (uint32_t *)fb_info->framebuffer_addr;

    fb[fb_i] = color;
}
void VBE_clear_screen()
{

    color_t *color;
    color = &bg_color;

    int i, j;
    for (i = 0; i < fb_info->framebuffer_width; i++)
    {
        for (j = 0; j < fb_info->framebuffer_height; j++)
        {
            VBE_draw_pixel(i, j, get_color(color));
        }
    }

    VBE_putf("Framebuffer info:");
    VBE_putf("\t Resolution: %dx%d", fb_info->framebuffer_width, fb_info->framebuffer_height);
    VBE_putf("\t Pitch: %d", fb_info->framebuffer_pitch);
    VBE_putf("\t BPP: %x\n", fb_info->framebuffer_bpp);
}
void VBE_init(struct stivale2_struct *info)
{
    module("VBE");
    struct stivale2_tag *tag = (struct stivale2_tag *)info->tags;
    struct stivale2_struct_tag_framebuffer *videoheader = videoheader;

    while (tag != NULL)
    {
        switch (tag->identifier)
        {

        case STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID:
        {
            videoheader = (struct stivale2_struct_tag_framebuffer *)tag;
            fb_info = videoheader;

            break;
        }
        }

        tag = (struct stivale2_tag *)tag->next;
    }
    log(INFO, "Framebuffer info:");
    log(INFO, "\t Resolution: %dx%d", fb_info->framebuffer_width, fb_info->framebuffer_height);
    log(INFO, "\t Pitch: %d", fb_info->framebuffer_pitch);
    log(INFO, "\t BPP: %x", fb_info->framebuffer_bpp);
}

void VBE_putchar(char character, int position_x, int position_y, color_t color)
{
    uint8_t iy, ix;
    for (iy = 0; iy < 8; iy++)
    {
        for (ix = 0; ix < 8; ix++)
        {
            if ((font[(uint8_t)character][iy] >> ix) & 1)
            {
                uint64_t offset = ((iy + position_y) * fb_info->framebuffer_pitch) + ((ix + position_x) * 4);
                *(uint32_t *)((uint64_t)fb_info->framebuffer_addr + offset) = get_color(&color);
            }
        }
    }
}
void VBE_put(char c, color_t color)
{
    if (c == '\n')
    {
        cursor_x = 5;
        cursor_y += 20;
    }
    if (c == '\0')
    {
        cursor_x -= 1;
        VBE_putchar(' ', cursor_x, cursor_y, color);
        cursor_x++;
    }
    else
    {
        VBE_putchar(c, cursor_x, cursor_y, color);
        cursor_x += 8;

        if (cursor_x >= (size_t)fb_info->framebuffer_width - 5)
        {
            cursor_x = 5;
            cursor_y += 10;
        }
    }
}

void VBE_puts(char *string, color_t color)
{
    while (*string)
    {
        VBE_put(*string++, color);
    }
}

void VBE_putf(char *format, ...)
{

    unsigned int i;
    unsigned int ZERO = 0;
    char *s;

    va_list arg;
    va_start(arg, format);

    while (*format)
    {

        if (*format == '%')
        {
            format++;
            switch (*format)
            {
            case 'c':
                i = va_arg(arg, int);
                VBE_put(i, fg_color);
                break;

            case 'd':
                i = va_arg(arg, int);
                if (i < ZERO)
                {
                    i = -i;
                    VBE_put('-', fg_color);
                }
                VBE_puts(string_convert(i, 10), fg_color);
                break;

            case 'o':
                i = va_arg(arg, unsigned int);
                VBE_puts(string_convert(i, 8), fg_color);
                break;

            case 's':
                s = va_arg(arg, char *);
                VBE_puts(s, fg_color);
                break;

            case 'x':
                i = va_arg(arg, unsigned int);
                VBE_puts(string_convert(i, 16), fg_color);
                break;
            default:
                VBE_put('%', fg_color);
                break;
            }
        }
        else
        {
            VBE_put(*format, fg_color);
        }
        format++;
    }

    va_end(arg);

    VBE_put('\n', fg_color);
}

/* Bresenham's circle algorithm */

void draw_circle(int xc, int yc, position_t position)
{
    int y = position.y;
    int x = position.x;

    VBE_draw_pixel(xc + x, yc + y, get_color(&white));
    VBE_draw_pixel(xc - x, yc + y, get_color(&white));
    VBE_draw_pixel(xc + x, yc - y, get_color(&white));
    VBE_draw_pixel(xc - x, yc - y, get_color(&white));
    VBE_draw_pixel(xc + y, yc + x, get_color(&white));
    VBE_draw_pixel(xc - y, yc + x, get_color(&white));
    VBE_draw_pixel(xc + y, yc - x, get_color(&white));
    VBE_draw_pixel(xc - y, yc - x, get_color(&white));
}

void VBE_display_circle(int xc, int yc, int radius)
{

    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;

    position_t position = {x, y};

    draw_circle(xc, yc, position);

    while (y >= x)
    {
        x++;

        if (d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
        {
            d = d + 4 * x + 6;
        }

        position_t n_position = {x, y};
        draw_circle(xc, yc, n_position);
    }
}

/* Bresenham's line algorithm */

void VBE_draw_line(int x1, int y1, int x2, int y2)
{

    int dx, dy, p, x, y;

    dx = x2 - x1;
    dy = y2 - y1;

    x = x1;
    y = x1;

    p = 2 * dy - dx;

    while (x < x2)
    {
        if (p >= 0)
        {
            VBE_draw_pixel(x, y, get_color(&white));
            y++;
            p = p + 2 * dy - 2 * dx;
        }

        else
        {
            VBE_draw_pixel(x, y, get_color(&white));
            p = p + 2 * dy;
        }
	x++;
    }
}
