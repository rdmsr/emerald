#include <arch/asm.h>

void cli(void)
{
    __asm__ volatile("cli");
}

void asm_sti(void)
{
    __asm__ volatile("sti");
}

void asm_hlt(void)
{
    __asm__ volatile("hlt");
}
uint8_t asm_inb(uint16_t port)
{
    uint8_t data;
    __asm__ volatile("inb %1, %0"
                     : "=a"(data)
                     : "d"(port));
    return data;
}

void asm_outb(uint16_t port, uint8_t data)
{
    __asm__ volatile("outb %0, %1"
                     :
                     : "a"(data), "Nd"(port));
}

#define ASM_MAKE_CRN(N)                                   \
    uint64_t asm_read_cr##N(void)                         \
    {                                                     \
        uint64_t value = 0;                               \
        __asm__ volatile("mov %%cr" #N ", %0"             \
                         : "=r"(value));                  \
        return value;                                     \
    }                                                     \
                                                          \
    void asm_write_cr##N(uint64_t value)                  \
    {                                                     \
        __asm__ volatile("mov %0, %%cr" #N ::"a"(value)); \
    }

ASM_MAKE_CRN(0);
ASM_MAKE_CRN(1);
ASM_MAKE_CRN(2);
ASM_MAKE_CRN(3);

void asm_xsave(void *region)
{
    __asm__ volatile("xsave (%0)" ::"a"(region));
}

void asm_xrstor(void *region)
{
    __asm__ volatile("xrstor (%0)" ::"a"(region));
}

void asm_fninit(void)
{
    __asm__ volatile("fninit");
}

void asm_fxsave(void *region)
{
    __asm__ volatile("fxsave (%0)" ::"a"(region));
}

void asm_fxrstor(void *region)
{
    __asm__ volatile("fxrstor (%0)" ::"a"(region));
}

uint64_t asm_read_msr(uint32_t msr)
{
    uint32_t eax;
    uint32_t edx;

    __asm__ volatile("rdmsr"
                     : "=a"(eax), "=d"(edx)
                     : "c"(msr));

    return ((uint64_t)edx << 32) | eax;
}

void asm_write_msr(u32 msr, u64 value)
{
    uint32_t eax = (uint32_t)value;
    uint32_t edx = (uint32_t)(value >> 32);
    __asm__ volatile("wrmsr" ::"a"(eax), "c"(msr), "d"(edx));
}

void asm_outd(uint16_t port, uint32_t value)
{
    __asm__ volatile("out %%eax,%%dx" ::"a"(value), "d"(port));
}

uint32_t asm_ind(uint16_t port)
{
    uint32_t ret;
    __asm__ volatile("in %%dx,%%eax"
                     : "=a"(ret)
                     : "d"(port));
    return ret;
}
