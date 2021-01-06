#include "PIT.h"
#include <debug-utilities/logger.h>
#include <libasm/asm.h>
#include <stdint.h>
void EmeraldProc_PIT_init(uint32_t frequency)
{
    uint16_t divisor = 1193182 / frequency;
    EmeraldASM_outb(0x43, 0x36);
    EmeraldASM_outb(0x40, (uint8_t)divisor & 0xFF);
    EmeraldASM_outb(0x40, (uint8_t)(divisor >> 8) & 0xFF);
    log(INFO, "Initialized PIT");
}
