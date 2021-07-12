/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */
#include "fb.h"
#include <emerald/debug.h>
#include <emerald/log.h>

static Framebuffer current_fb = {0};

void fb_draw_pixel(size_t px, size_t py, uint32_t color)
{    
    size_t index = px + (current_fb.pitch / sizeof(uint32_t)) * py;
    
    ((uint32_t *)current_fb.address)[index] = color;
}

void fb_init(struct stivale2_struct_tag_framebuffer *fb)
{
    kassert(fb != NULL);

    current_fb.address = fb->framebuffer_addr;
    current_fb.width = fb->framebuffer_width;
    current_fb.height = fb->framebuffer_height;
    current_fb.pitch = fb->framebuffer_pitch;
    current_fb.bpp = fb->framebuffer_bpp;
}
