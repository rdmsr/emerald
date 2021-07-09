#include <arch/arch.h>
#include <arch/cpuid.h>
#include <arch/memory/pmm.h>
#include <cpuid.h>
#include <devices/com.h>
#include <devices/pit.h>
#include <emerald/debug.h>
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
    log(INFO, "Compiled on {a} {a}", __DATE__, __TIME__);
    log(INFO, "Using x86_64-elf-gcc ({a})", __VERSION__);
    log(INFO, "------------------------------------------------");
}

void kmain(struct stivale2_struct *stivale2_struct)
{
    com_initialize(COM1);

    arch_initialize_descriptors();

    arch_initialize_memory(stivale2_struct);

    pit_initialize(1000);

    kernel_splash();

    log(INFO, "Usable memory: {m}mb\t Usable pages: {i}", get_usable_pages() * PAGE_SIZE, get_usable_pages());

    log(INFO, "CPU vendor: {a}", cpuid_get_vendor());

    while (true)
    {
        asm_hlt();
    }
}
