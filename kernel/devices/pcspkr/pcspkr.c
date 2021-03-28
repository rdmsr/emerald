/*
MIT License

Copyright (c) 2021 Smart6502 (Xenon6502)

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

#include "pcspkr.h"
#include <libk/io.h>
#include <libk/logging.h>
#include <libk/module.h>
#include <libk/time/time.h>
#include <system/interrupts/PIT.h>

void PCSpkr_init()
{
    IO_outb(0x61, IO_inb(0x61) | 0x1);
    module("PCSpkr");
    log(INFO, "Initialized PC speaker");
}

/* Change the c2 f */
void PCSpkr_set_c2(uint32_t hz)
{
    __asm__ volatile("cli");

    uint32_t div = 1193182 / hz;

    IO_outb(0x42, 0xB6);

    IO_outb(0x40, div & 0xFF);

    IO_outb(0x40, div >> 8);
    
    __asm__ volatile("sti");
}

void PCSpkr_play(uint32_t frequency)
{
    uint8_t tmp;
    PCSpkr_set_c2(frequency);

    tmp = IO_inb(0x61);
    if (tmp != (tmp | 3))
        IO_outb(0x61, tmp | 3);
}

void PCSpkr_stop()
{
    IO_outb(0x61, IO_inb(0x61) & 0xFC);
    PCSpkr_set_c2(1);
}

void PCSpkr_beep(uint16_t mstime)
{
    module("PCSpkr");

    PCSpkr_play(1000);

    sleep(mstime);

    PCSpkr_stop();

    log(INFO, "Beeped for %d ms!", mstime);
}
