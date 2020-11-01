#define GDTBASE    0x00000800
//Global Descriptor table (GDT)
struct gdt_pointer {
    uint16_t limit;
    uint64_t base;
} __attribute__ ((packed));
struct gdt_descriptor {
    uint16_t limit_low16;
    uint16_t base_low16;
    uint8_t  base_mid8;
    uint8_t access;
    uint8_t granularity;
    uint8_t  base_high8;  
} __attribute__((packed));
struct gdt_descriptor gdt[8];
extern struct gdt_pointer gdtr = {.limit = sizeof(gdt) - 1, .base = (uint64_t)gdt};


void gdt_load() {
    asm volatile("lgdt %0" : : "m"(gdtr) : "memory");
    asm volatile (R"(
    mov %%rsp, %%rax
    push $0x10
    push %%rax
    pushf
    push $0x8
    push $1f
    iretq
    1:
    mov $0x10, %%ax
    mov %%ax, %%ds
    mov %%ax, %%es
    mov %%ax, %%ss
    mov %%ax, %%fs
    mov %%ax, %%gs
                )" : : : "rax", "memory");
}
void gdt_init(){
    gdt[1] = (struct gdt_descriptor) { .access = 0b10011010  ,.granularity = 0b00100000 };
    gdt[2] = (struct gdt_descriptor) { .access = 0b10010010  ,.granularity = 0 };
    gdt_load();
	}
