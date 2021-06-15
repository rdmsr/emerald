#include <arch/arch.h>
#include <arch/memory/pmm.h>
#include <devices/com.h>
#include <emerald/log.h>
#include <emerald/macros.h>
#include <main.h>

void kernel_splash()
{

    log(INFO, "------------------------------------------------");
    log(INFO, "   __                     __ __ __                  ");
    log(INFO, " |  |--.-----.----.--.--|  |  |__.--.--.--------.  ");
    log(INFO, " |  _  |  -__|   _|  |  |  |  |  |  |  |        |  ");
    log(INFO, " |_____|_____|__| |___  |__|__|__|_____|__|__|__|  ");
    log(INFO, "                |_____|");
    log(INFO, "{a} {a}", __DATE__, __TIME__);
    log(INFO, "x86_64-elf-gcc ({a})", __VERSION__);
    log(INFO, "------------------------------------------------");
}

void kmain(struct stivale2_struct *stivale2_struct)
{
    UNUSED(stivale2_struct);

    com_initialize(COM1);

    arch_initialize_descriptors();
    pmm_initialize(stivale2_struct);
    kernel_splash();

    log(INFO, "{s} {i}", make_str("Hello"), 10);

    for (;;)
    {
        __asm__("hlt");
    }
}
