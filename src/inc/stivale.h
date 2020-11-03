
#include <stdint.h>
#define STIVALE_MEMORY_AVAILABLE              1
#define STIVALE_MEMORY_RESERVED               2
#define STIVALE_MEMORY_ACPI_RECLAIMABLE       3
#define STIVALE_MEMORY_NVS                    4
#define STIVALE_MEMORY_BADRAM                 5
#define STIVALE_KERNEL_MEMORY                 10
struct stivale_header {
	//stack pointer that's going to be loaded
    uint64_t stack;
    // Flags
    // bit 0   0 = text mode,   1 = graphics mode
    uint16_t flags;
    uint16_t framebuffer_width;
    uint16_t framebuffer_height;
    uint16_t framebuffer_bpp;
	uint64_t entry_point;
} __attribute__((packed));

struct stivale_module {
    uint64_t begin;
    uint64_t end;
    char     string[128];
    uint64_t next;
} __attribute__((packed));
struct {
    uint64_t addr;
    uint64_t len;
    uint32_t type;
    uint32_t unused;
} __attribute__((packed));

struct stivale_struct {
    uint64_t cmdline;
    uint64_t memory_map_addr;
    uint64_t memory_map_entries;
    uint64_t framebuffer_addr;
    uint16_t framebuffer_pitch;
    uint16_t framebuffer_width;
    uint16_t framebuffer_height;
    uint16_t framebuffer_bpp;
    uint64_t rsdp;
    uint64_t module_count;
    uint64_t modules;
    uint64_t epoch;
    uint64_t flags;       // bit 0: 1 if booted with BIOS, 0 if booted with UEFI
} __attribute__((packed));
