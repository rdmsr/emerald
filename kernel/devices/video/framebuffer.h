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

/**
A struct containing a RGB color.
*/
typedef struct
{
    /** Red */
    uint8_t r;
    /** Green */
    uint8_t g;
    /** Blue */
    uint8_t b;
} Color;

/** A struct containing information about a framebuffer. */
typedef struct
{
    /** The colorscheme currently used. */
    Color colorscheme[8];
    /** Background color. */
    Color background;
    /** Foreground color. */
    Color foreground;
    /** The bootloader information about the framebuffer. */
    struct stivale2_struct_tag_framebuffer *fb_info;
    /** Position of the cursor. */
    int cursor_y, cursor_x;
    /** PSF font. */
    PSF font;
} Framebuffer;

/** Creates a new framebuffer.
@param[in] colorscheme[8] The RGB colorscheme.
@param[in] info The bootloader information about the framebuffer.
@param[out] fb The newly created framebuffer.*/
Framebuffer Framebuffer_init(Color colorscheme[8], struct stivale2_struct *info);

/** Clears the screen */
void Framebuffer_clear();

/** Puts a character on the framebuffer
@param[in] c The character.
@param[in] position_x The x position of the character.
@param[in] position_y The y position of the character.
@param[in] self The framebuffer.*/

void Framebuffer_putc(char c, int position_x, int position_y, Framebuffer *self);

/** Puts a string on the framebuffer
@param[in] string The string.*/

void Framebuffer_puts(char *string);

/** Sets the currently used framebuffer (every function will now use this one)
@param[in] self The framebuffer to set as currently used framebuffer */

void Framebuffer_set(Framebuffer *self);

/** Puts a character using the cursor
@param[in] c The character. */

void Framebuffer_put(char c);

/** Generates a RGB color
@param[in] r Red.
@param[in] g Green.
@param[in] b Blue.
@param[out] color The generated color.*/
Color rgb(int r, int g, int b);

/** GUI Log
@param[in] level The level of logging.
@param[in] format The formatted string.*/
void glog(int level, char *format, ...);

enum glog_level
{
    SUCCESS,
    FAILURE,
    SILENT

};
#endif
