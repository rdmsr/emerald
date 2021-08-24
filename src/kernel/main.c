#include "arch/context.h"
#include "arch/descriptors/interrupts.h"
#include "emerald/str/fmt.h"
#include "emerald/str/str.h"
#include <arch/arch.h>
#include <arch/cpuid.h>
#include <arch/proc/sched.h>
#include <arch/tasking.h>
#include <devices/apic.h>
#include <devices/pic.h>
#include <devices/pit.h>
#include <emerald/debug.h>
#include <emerald/ds/vec.h>
#include <emerald/functional.h>
#include <emerald/log.h>
#include <main.h>

void kernel_splash()
{

    log("------------------------------------------------");
    log("   __                     __ __ __                  ");
    log(" |  |--.-----.----.--.--|  |  |__.--.--.--------.  ");
    log(" |  _  |  -__|   _|  |  |  |  |  |  |  |        |  ");
    log(" |_____|_____|__| |___  |__|__|__|_____|__|__|__|  ");
    log("                |_____|");
    log("Compiled on {} {}", __DATE__, __TIME__);
    log("Using x86_64-elf-gcc ({})", __VERSION__);
    log("------------------------------------------------");
}

void test()
{
    log("hello I am a task");
}

void kmain(MAYBE_UNUSED struct stivale2_struct *stivale2_struct)
{
    com_initialize(COM1);

    arch_initialize_descriptors();

    arch_initialize_memory(stivale2_struct);

    pic_initialize();

    pit_initialize(1000);

    acpi_initialize(stivale2_struct);

    /* apic_initialize();*/

    apic_timer_initialize();

    kernel_splash();

    log("Usable memory: {}mb\t Usable pages: {}", get_usable_pages() * PAGE_SIZE / 1024 / 1024, get_usable_pages());

    log("CPU model: {}, CPU vendor: {}", cpuid_get_model(), cpuid_get_vendor());

    auto init = task_create(make_str("init"), -1, (uintptr_t)test);
    
    sched_initialize();
    
    sched_start(init);
    
    toggle_sched_init();

    while (true)
      ;
}
