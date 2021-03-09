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
    uint32_t div = BASE_FREQ / hz;
    IO_outb(PIT_CTL, 0xB6);
    IO_outb(TIMER2_CTL, div & 0xFF);
    IO_outb(TIMER2_CTL, div >> 8);
}

void PCSpkr_tone_on(uint32_t frequency)
{
    PCSpkr_set_c2(frequency);

    uint8_t tmp = IO_inb(0x61);
    if (tmp != (tmp | 3))
        IO_outb(0x61, tmp | 3);
}

void PCSpkr_tone_off()
{
    /* Shut it up */
    IO_outb(0x61, IO_inb(0x61) & 0xFC);
    PCSpkr_set_c2(1);
}

void PCSpkr_sleep(uint16_t delay)
{
    uint64_t cticks = PIT_get_ticks();
    while (1)
    {
        if (cticks + delay < PIT_get_ticks())
            break;
    }
}

void PCSpkr_beep(uint16_t mstime)
{
    module("PCSpkr");
    /* I desire thee ears survive ;) */
    PCSpkr_tone_on(1000);
    PCSpkr_sleep(mstime);
    /* sleep for sometime */
    PCSpkr_tone_off();

    log(INFO, "Beeped for %d ms!", mstime);
}
