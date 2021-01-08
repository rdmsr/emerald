/*
MIT License

Copyright (c) 2021 Abb1x

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "vbe.h"
#include "font.h"
#include <debug-utilities/logger.h>
#include <inc/stivale2.h>
#include <mem/virtual/vmm.h>
#include <stddef.h>
struct stivale2_struct_tag_framebuffer *fb_info;
color_t bg_color = {0, 64, 73};
color_t fg_color = {255, 255, 255};
size_t cursor_x = 5;
size_t cursor_y = 5;
uint32_t get_color(color_t *color)
{
    return (uint32_t)((color->r << RED_SHIFT) | (color->g << GREEN_SHIFT) | (color->b << BLUE_SHIFT));
}

void EmeraldDevices_VBE_draw_pixel(size_t x, size_t y, uint32_t color)
{
    size_t fb_i = x + (fb_info->framebuffer_pitch / sizeof(uint32_t)) * y;
    uint32_t *fb = (uint32_t *)fb_info->framebuffer_addr;

    fb[fb_i] = color;
}
void EmeraldDevices_VBE_clear_screen()
{

    color_t *color;
    color = &bg_color;
    for (int i = 0; i < fb_info->framebuffer_width; i++)
    {
        for (int j = 0; j < fb_info->framebuffer_height; j++)
        {
            EmeraldDevices_VBE_draw_pixel(i, j, get_color(color));
        }
    }
}
void EmeraldDevices_VBE_init(struct stivale2_struct *info)
{
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

void EmeraldDevices_VBE_putchar(char character, int position_x, int position_y, color_t color)
{
    for (uint8_t iy = 0; iy < 8; iy++)
    {
        for (uint8_t ix = 0; ix < 8; ix++)
        {
            if ((font[(uint8_t)character][iy] >> ix) & 1)
            {
                uint64_t offset = ((iy + position_y) * fb_info->framebuffer_pitch) + ((ix + position_x) * 4);
                *(uint32_t *)((uint64_t)fb_info->framebuffer_addr + offset) = get_color(&color);
            }
        }
    }
}
void EmeraldDevices_VBE_put(char c, color_t color)
{
    if (c == '\n')
    {
        cursor_x = 5;
        cursor_y += 20;
    }
    if(c == '\0')
    {
      cursor_x -= 1;
      EmeraldDevices_VBE_putchar(' ', cursor_x, cursor_y, color);
      cursor_x++;
    }
    else
    {
        EmeraldDevices_VBE_putchar(c, cursor_x, cursor_y, color);
        cursor_x += 8;

        if (cursor_x >= (size_t)fb_info->framebuffer_width - 5)
        {
            cursor_x = 5;
            cursor_y += 10;
        }
    }
}

void EmeraldDevices_VBE_print(char *string, color_t color)
{
    while (*string)
    {
        EmeraldDevices_VBE_put(*string++, color);
    }
}
