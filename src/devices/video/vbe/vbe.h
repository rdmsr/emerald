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
#pragma once
#include <inc/stivale2.h>
#include <stddef.h>
#define RED_SHIFT   16
#define GREEN_SHIFT 8
#define BLUE_SHIFT  0

typedef struct
{
    uint8_t r, g, b;
} color_t;
void EmeraldDevices_VBE_init(struct stivale2_struct *info);
void EmeraldDevices_VBE_draw_pixel(size_t x, size_t y, uint32_t color);
void EmeraldDevices_VBE_clear_screen();
void EmeraldDevices_VBE_putchar(char character, int position_x, int position_y,color_t color);
void EmeraldDevices_VBE_print(char* string,color_t color);
void EmeraldDevices_VBE_put(char c,color_t color);
#endif
