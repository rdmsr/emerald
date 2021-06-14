#include <arch/arch.h>
#include <devices/com.h>
#include <emerald/log.h>
#include <emerald/macros.h>
#include <main.h>
void kernel_splash()
{
    log(INFO, "  __                     __ __ __                ");
    log(INFO, " |  |--.-----.----.--.--|  |  |__.--.--.--------.");
    log(INFO, "|  _  |  -__|   _|  |  |  |  |  |  |  |        |");
    log(INFO, "|_____|_____|__| |___  |__|__|__|_____|__|__|__|");
    log(INFO, "                |_____|");
    log(INFO, "copyright 2021 (c) emeraldOS contributors.");
}
void kmain(struct stivale2_struct *stivale2_struct)
{
    UNUSED(stivale2_struct);

    com_initialize(COM1);

    arch_initialize_descriptors();    
    kernel_splash();
    for (;;)
    {
        __asm__("hlt");
    }
}
