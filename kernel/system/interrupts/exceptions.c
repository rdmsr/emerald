#include "exceptions.h"
#include <stdint.h>
#include <system/panic.h>
static char *exceptions[] = {
    [0] = "Division by Zero",
    [1] = "Debug",
    [2] = "Non Maskable Interrupt",
    [3] = "Breakpoint",
    [4] = "Overflow",
    [5] = "Bound Range Exceeded",
    [6] = "Invalid opcode",
    [7] = "Device not available",
    [8] = "Double Fault",
    [10] = "Invalid TSS",
    [11] = "Segment not present",
    [12] = "Stack Exception",
    [13] = "General Protection fault",
    [14] = "Page fault",
    [16] = "x87 Floating Point Exception",
    [17] = "Alignment check",
    [18] = "Machine check",
    [19] = "SIMD floating point Exception",
    [20] = "Virtualization Exception",
    [30] = "Security Exception"};

void exception_handler(uint64_t irq)
{
    __asm__("cli");
    log(PANIC, "Oh no! Unhandled exception: %s", exceptions[irq]);
    panic((char *)exceptions[irq]);
    while (1)
        ;
}

void exc_0()
{
    exception_handler(0);
};
void exc_1()
{
    exception_handler(1);
};
void exc_2()
{
    exception_handler(2);
};
void exc_3()
{
    exception_handler(3);
};
void exc_4()
{
    exception_handler(4);
};
void exc_5()
{
    exception_handler(5);
};
void exc_6()
{
    exception_handler(6);
};
void exc_7()
{
    exception_handler(7);
};
void exc_8()
{
    exception_handler(8);
};
void exc_10()
{
    exception_handler(10);
};
void exc_11()
{
    exception_handler(11);
};
void exc_12()
{
    exception_handler(12);
};
void exc_13()
{
    exception_handler(13);
};
void exc_14()
{
    exception_handler(14);
};
void exc_15()
{
    exception_handler(15);
};
void exc_16()
{
    exception_handler(16);
};
void exc_17()
{
    exception_handler(17);
};
void exc_18()
{
    exception_handler(18);
};
void exc_19()
{
    exception_handler(19);
};
void exc_20()
{
    exception_handler(20);
};
void exc_30()
{
    exception_handler(30);
};
