#include "bios.h"
#include <libasm/asm.h>
#include <stdint.h>
void reboot()
{
    uint8_t good = 0x02;
    while (good & 0x02)
        good = EmeraldASM_inb(0x64);
    EmeraldASM_outb(0x64, 0xFE);
    asm volatile("hlt");
}
