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
#include "framebuffer.h"
#include <boot/boot.h>
#include <libk/logging.h>
#include <libk/string.h>
#include <memory/pmm.h>

/* The font is terminus.
Copyright (c) 2011 Tilman Blumenbach,
with Reserved Font Name "Terminus (TTF)".
This Font Software is licensed under the SIL Open Font License, Version 1.1.
This license is  available with a FAQ at:
http://scripts.sil.org/OFL
*/

extern PSF fb_font;

typedef enum
{
    OFF,
    PRE_PARSING,
    PARSING,

} ParsingState;

ParsingState state = OFF;
Framebuffer current_fb;

size_t ptr = 0;
int cursor_x = 0, cursor_y = 0;
char nbr[3];

Color rgb(int r, int g, int b)
{
    return (Color){r, g, b};
}

uint32_t get_color(Color *color)
{
    return ((color->r & 0xff) << 16) + ((color->g & 0xff) << 8) + (color->b & 0xff);
}

void Framebuffer_draw_pixel(int x, int y, uint32_t color, Framebuffer *self)
{
    size_t fb_i = x + (self->fb_info->framebuffer_pitch / sizeof(uint32_t)) * y;
    uint32_t *fb = (uint32_t *)self->fb_info->framebuffer_addr;

    fb[fb_i] = color;
}

void Framebuffer_set(Framebuffer *self)
{
    current_fb = *self;
}

void Framebuffer_putc(char c, int position_x, int position_y, Framebuffer *self)
{

    switch (c)
    {
    case '\n':
    {
        cursor_y++;
        cursor_x = 1;
        current_fb.cursor_y = cursor_y;
	current_fb.cursor_x = cursor_x;
        return;
    }
    case '\t':
    {
        cursor_x += 4;
        current_fb.cursor_x = cursor_x;
        return;
    }

    /* ANSI Escape code parsing */
    case '\033':
    {
        state = PRE_PARSING;
        return;
    }
    }

    if (state == PARSING)
    {

        if (c != ';' && c != 'm')
        {
            nbr[ptr++] = c;
            return;
        }

        else
        {
        }
    }

    if (state == PRE_PARSING && c == '[')
    {
        state = PARSING;
        return;
    }

    if (state == PARSING && c == 'm')
    {
        int digit = atoi(nbr);
        state = OFF;

        if (digit == 0)
        {
            self->foreground = rgb(131, 148, 150);
            self->background = rgb(0, 43, 54);
        }

        if (digit > 29 && digit < 38)
        {
            self->foreground = self->colorscheme[digit - 30];
        }

        ptr = 0;
        memset(nbr, 0, 3);

        return;
    }

    uint8_t *glyph = &fb_font.data[c * self->font.glyph_size];

    size_t x = position_x * self->font.width, y = position_y * self->font.height;

    static const uint8_t masks[8] = {128, 64, 32, 16, 8, 4, 2, 1};

    size_t i, j;
    for (i = 0; i < self->font.height; i++)
    {
        for (j = 0; j < self->font.width; j++)
        {

            if (glyph[i] & masks[j])
            {
                Framebuffer_draw_pixel(x + j, y + i, get_color(&self->foreground), self);
            }
            else
            {
                Framebuffer_draw_pixel(x + j, y + i, get_color(&self->background), self);
            }
        }
    }

    if (c != '\n')
    {
        self->cursor_x++;
        cursor_x++;
    }
}

void Framebuffer_puts(char *string)
{

    current_fb.cursor_x = cursor_x;
    current_fb.cursor_y = cursor_y;
    while (*string)
    {
        Framebuffer_putc(*string++, current_fb.cursor_x, current_fb.cursor_y, &current_fb);
    }
}

void Framebuffer_clear()
{
    cursor_x = 1;
    cursor_y = 0;
    current_fb.cursor_x = 1;
    current_fb.cursor_y = 0;

    size_t x;
    size_t y;

    for (x = 0; x < current_fb.fb_info->framebuffer_width; x++)
    {
        for (y = 0; y < current_fb.fb_info->framebuffer_height; y++)
        {
            Framebuffer_draw_pixel(x, y, get_color(&current_fb.background), &current_fb);
        }
    }
}

void glog(int level, char *format, ...) /* GUI logging */
{
    va_list ap;
    va_start(ap, format);

    char s[4096] = {0};

    vsprintf(s, format, ap);

    switch (level)
    {
    case SUCCESS:
        Framebuffer_puts("\033[32m* \033[0m");
        break;
    case SILENT:
        break;
    case FAILURE:
        Framebuffer_puts("\033[31m* \033[0m");
        break;
    }

    Framebuffer_puts(s);
    va_end(ap);

    Framebuffer_puts("\n");
}

void Framebuffer_put(char c)
{
    Framebuffer_putc(c, current_fb.cursor_x, current_fb.cursor_y, &current_fb);
}

Framebuffer Framebuffer_init(Color colorscheme[8], struct stivale2_struct *info)
{

    module("Framebuffer");
    Framebuffer new;

    new.fb_info = stivale2_get_tag(info, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);
    int i;
    for (i = 0; i < 7; i++)
    {
        new.colorscheme[i] = colorscheme[i];
    }

    new.foreground = rgb(131, 148, 150);
    new.background = rgb(0, 43, 54);
    new.cursor_x = 1;
    new.cursor_y = 0;
    new.font = fb_font;

    current_fb = new;

    log(INFO, "Initialized");

    return new;
}
