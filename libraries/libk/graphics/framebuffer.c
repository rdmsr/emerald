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

#include <libk/graphics/framebuffer.h>
#include <libk/logging.h>

/* OOP C because I wanted to try it */

/* The font is terminus.

Copyright (c) 2011 Tilman Blumenbach,
with Reserved Font Name "Terminus (TTF)".

This Font Software is licensed under the SIL Open Font License, Version 1.1.
This license is  available with a FAQ at:
http://scripts.sil.org/OFL
*/
extern PSF fb_font;

struct stivale2_struct_tag_framebuffer *Framebuffer_get_info();

/* Initializing the framebuffer */
static Framebuffer current_framebuffer;
static Color bg_color = {0, 64, 73}, fg_color = {238, 232, 213};

static void init(struct stivale2_struct *info, Framebuffer *self)
{
    /* Setting the logging module */
    module("Framebuffer");

    /* Getting info */
    struct stivale2_struct_tag_framebuffer *fb_info = Framebuffer_get_info(info);

    self->width = fb_info->framebuffer_width;
    self->height = fb_info->framebuffer_height;
    self->bpp = fb_info->framebuffer_bpp;
    self->pitch = fb_info->framebuffer_pitch;
    self->address = fb_info->framebuffer_addr;

    /* Initializing the VBE driver */
    VBE_init(info);
}

static void clear_screen(Framebuffer *self)
{
    VBE_clear_screen(0, self->bg_color);
}

struct stivale2_struct_tag_framebuffer *Framebuffer_get_info(struct stivale2_struct *info)
{
    struct stivale2_tag *tag = (struct stivale2_tag *)info->tags;
    struct stivale2_struct_tag_framebuffer *fb_info = NULL;

    while (tag != NULL)
    {
        switch (tag->identifier)
        {

        case STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID:
        {
            fb_info = (struct stivale2_struct_tag_framebuffer *)tag;

            break;
        }
        }

        tag = (struct stivale2_tag *)tag->next;
    }
    return fb_info;
}

static uint32_t get_color(Color *color)
{
    return (uint32_t)((color->r << RED_SHIFT) | (color->g << GREEN_SHIFT) | (color->b << BLUE_SHIFT));
}

static void putchar(uint8_t c, Position pos, Framebuffer *self)
{

    /* FIXME: &self->fb_font.data is not working, we need to use the fb_font directly */
    uint8_t *glyph = &fb_font.data[c * self->font.glyph_size];

    size_t x = pos.x * self->font.width, y = pos.y * self->font.height;

    static const uint8_t masks[8] = {128, 64, 32, 16, 8, 4, 2, 1};

    size_t i, j;
    for (i = 0; i < self->font.height; i++)
    {
        for (j = 0; j < self->font.width; j++)
        {
            if (glyph[i] & masks[j])
            {
                VBE_draw_pixel(x + j, y + i, get_color(&self->fg_color));
            }
            else
            {
                VBE_draw_pixel(x + j, y + i, get_color(&self->bg_color));
            }
        }
    }
}

static void puts(char *string, Framebuffer *self)
{
    while (*string)
    {
        putchar(*string++, (Position){1, 1}, self);
    }
}

Framebuffer Framebuffer_get_current()
{
    return current_framebuffer;
}

Framebuffer _Framebuffer()
{
    Framebuffer new;

    /* Functions */
    new.init = init;
    new.clear_screen = clear_screen;
    new.puts = puts;
    new.putchar = putchar;

    /* Font */
    new.font = fb_font;

    /* Colors */
    new.bg_color = bg_color;
    new.fg_color = fg_color;

    current_framebuffer = new;

    return new;
}
