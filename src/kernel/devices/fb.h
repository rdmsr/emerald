/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef KERNEL_FB_H
#define KERNEL_FB_H
#include <emerald/std.h>
#include <boot/stivale2.h>

typedef struct
{
    uint16_t width, height;
    uint64_t address;
    uint32_t bg, fg;
    uint16_t pitch, bpp;
} Framebuffer;

void fb_draw_pixel(size_t px, size_t py, uint32_t color);
void fb_init(struct stivale2_struct_tag_framebuffer *fb);

#endif
