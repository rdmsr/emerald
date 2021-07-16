#include <arch/asm.h>
#include <devices/pit.h>

uint32_t pit_read_count(void)
{
    asm_outb(0x43, 0);
    uint32_t counter = asm_inb(0x40);
    counter |= asm_inb(0x40) << 8;

    return counter;
}

void pit_initialize(int hz)
{
    int divisor = 1193180 / hz;

    asm_outb(0x43, 0x36);
    asm_outb(0x40, divisor & 0xff);
    asm_outb(0x40, (divisor >> 8) & 0xFF);

    log("initialized PIT");
}

void pit_usleep(uint16_t ms)
{
    asm_outb(0x43, 0x30);
    asm_outb(0x40, ms & 0xff);
    asm_outb(0x40, (ms >> 8) & 0xff);

    while (pit_read_count() == 0)
        ;
}
