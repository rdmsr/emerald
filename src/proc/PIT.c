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
#include "PIT.h"
#include <debug-utilities/logger.h>
#include <sys/idt/idt.h>
#include <libasm/asm.h>
#include <stdint.h>
int ticks = 0;
void EmeraldProc_PIT_init(uint32_t frequency)
{
    uint16_t divisor = 1193182 / frequency;
    EmeraldASM_outb(0x43, 0x36);
    EmeraldASM_outb(0x40, (uint8_t)divisor & 0xFF);
    EmeraldASM_outb(0x40, (uint8_t)(divisor >> 8) & 0xFF);
    log(INFO, "Initialized PIT, frequency: %d Hz",frequency);

}
void EmeraldProc_PIT_start_timer()
{
    ticks++;
}
int EmeraldProc_PIT_get_ticks()
{
  return ticks;
}
