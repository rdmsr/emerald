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
#include <devices/serial/serial.h>

struct stivale2_struct_tag_framebuffer *fb_info;

color_t bg_color = {0, 64, 73};
color_t fg_color = {0, 127, 127};

size_t cursor_x = 0;
size_t cursor_y = 0;

uint32_t get_color(color_t *color)
{
    return (uint32_t)((color->r << RED_SHIFT) | (color->g << GREEN_SHIFT) | (color->b << BLUE_SHIFT));
}

void VBE_set_fgc(color_t color)
{
    fg_color = color;
}

void VBE_set_bgc(color_t color)
{
    bg_color = color;
}

void VBE_draw_pixel(position_t pos, uint32_t color)
{
    size_t fb_i = pos.x + (fb_info->framebuffer_pitch / sizeof(uint32_t)) * pos.y;
    uint32_t *fb = (uint32_t *)fb_info->framebuffer_addr;

    fb[fb_i] = color;
}

uint32_t VBE_read_pixel(position_t pos)
{
    size_t fb_i = pos.x + (fb_info->framebuffer_pitch / sizeof(uint32_t)) * pos.y;
    uint32_t *fb = (uint32_t *)fb_info->framebuffer_addr;

    return fb[fb_i];
}

uint32_t VBE_read_pixel_offset(position_t pos, int offset)
{
    size_t fb_i = pos.x + (fb_info->framebuffer_pitch / sizeof(uint32_t)) * (pos.y + offset);
    uint32_t *fb = (uint32_t *)fb_info->framebuffer_addr;

    return fb[fb_i];
}

void VBE_clear_screen()
{

    color_t *color;
    color = &bg_color;
    int h = fb_info->framebuffer_height;
    int w = fb_info->framebuffer_width;
    int i, j;
    position_t position;
    int c_err = 1;
    while (c_err != 0) {
        c_err = 0;
        for (i = 0; i < h; i++)
        {
            for (j = 0; j < w; j++)
            {
                position.x = j;
                position.y = i;
                VBE_draw_pixel(position, get_color(color));
            }
        }
        /* Workaround for QEMU's weird display buffer glitches */
        for (i = 0; i < h; i++)
        {
            for (j = 0; j < w; j++)
            {
                position.x = j;
                position.y = i;
                if (VBE_read_pixel(position) != get_color(color)) {c_err++; VBE_draw_pixel(position, get_color(color));}
            }
        }
    }
}

void VBE_scroll(int lines)
{
    color_t *color;
    color = &bg_color;
    int h = fb_info->framebuffer_height;
    int w = fb_info->framebuffer_width;
    position_t position;
    int i, j;
    for (i = 0; i < h; i++)
    {
        for (j = 0; j < w; j++)
        {
            position.x = j;
            position.y = i;
            VBE_draw_pixel(position, VBE_read_pixel_offset(position, lines));
        }
    }
    for (i = h - lines; i < h; i++)
    {
        for (j = 0; j < w; j++)
        {
            position.x = j;
            position.y = i;
            VBE_draw_pixel(position, get_color(color));
        }
    }
}

void VBE_init(struct stivale2_struct *info)
{
    module("VBE");
    struct stivale2_tag *tag = (struct stivale2_tag *)info->tags;
    struct stivale2_struct_tag_framebuffer *videoheader = videoheader;

    while (tag != 0)
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
    VBE_clear_screen();
    VBE_putf("Framebuffer info:\n");
    VBE_putf("\t Resolution: %dx%d\n", fb_info->framebuffer_width, fb_info->framebuffer_height);
    VBE_putf("\t Pitch: %d\n", fb_info->framebuffer_pitch);
    VBE_putf("\t BPP: %x\n", fb_info->framebuffer_bpp);
}

void VBE_putchar(char character, int position_x, int position_y, color_t color)
{
    uint8_t iy, ix;
    for (iy = 0; iy < 8; iy++)
    {
        for (ix = 0; ix < 8; ix++)
        {
            if ((font[(uint8_t)character][iy] >> (7 - ix)) & 1)
            {
                uint64_t offset = ((iy + position_y) * fb_info->framebuffer_pitch) + ((ix + position_x) * 4);
                *(uint32_t *)((uint64_t)fb_info->framebuffer_addr + offset) = get_color(&color);
            }
            else /* More QEMU workarounds */
            {
                uint64_t offset = ((iy + position_y) * fb_info->framebuffer_pitch) + ((ix + position_x) * 4);
                *(uint32_t *)((uint64_t)fb_info->framebuffer_addr + offset) = get_color(&bg_color);
            }
        }
    }
}

void VBE_move_cursor(int cx, int cy)
/*avoids a implicit declaration error*/
{
    cursor_x = cx;
    cursor_y = cy;
}

void VBE_put_nf(char c, color_t color)
{
    VBE_putchar(c, cursor_x, cursor_y, color);
    cursor_x += 8;

    if (cursor_x >= (size_t)fb_info->framebuffer_width)
    {
        cursor_x = 0;
        cursor_y += 8;
    }
    if (cursor_y >= (size_t)fb_info->framebuffer_height - 7) {VBE_scroll(8); cursor_y -= 8;}
}

void VBE_put(char c, color_t color)
{
    if (c == '\n')
    {
        cursor_x = 0;
        cursor_y += 8;
    }
    else if (c == '\0') {}
    else if (c == '\b') 
    {
        cursor_x -= 8;
        if (/*cursor_x < 0 || */cursor_x >= (size_t)fb_info->framebuffer_width) {cursor_x = (size_t)(fb_info->framebuffer_width - 8); cursor_y -= 8;}
        if (/*cursor_y < 0 || */cursor_y >= (size_t)fb_info->framebuffer_width) {cursor_y = 0; cursor_x = 0;}
        VBE_putchar('\xDB', cursor_x, cursor_y, bg_color);
    }
    else
    {
        VBE_putchar(c, cursor_x, cursor_y, color);
        cursor_x += 8;
    }
    if (cursor_x >= (size_t)fb_info->framebuffer_width)
    {
        cursor_x = 0;
        cursor_y += 8;
    }
    if (cursor_y >= (size_t)fb_info->framebuffer_height - 7) {VBE_scroll(8); cursor_y -= 8; cursor_x = 0;}
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
            if (*format == '\\') {
                format++;
                if (*format == 'n') {
                    VBE_put('\n', fg_color);
                } else if (*format == '\\') {
                    VBE_put('\\', fg_color);
                }
            } else {
                if (*format == '\t') {
                    VBE_puts("    ", fg_color);
                } else {
                    VBE_put(*format, fg_color);
                }
            }
        }
        format++;
    }

    va_end(arg);

    /*VBE_put('\n', fg_color);*/
}
