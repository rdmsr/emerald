ENTRY(kmain)

SECTIONS
{
    kernel_phys_offset = 0xffffffff80000000;
    . = kernel_phys_offset + 0x100000;

    .stivale2hdr ALIGN(4K) :
    {
        KEEP(*(.stivale2hdr))
    }

    .text ALIGN(4K) :
    {
        KEEP(*(.text*))
    }

    .rodata ALIGN(4K) :
    {
        KEEP(*(.rodata*))
    }

    .data ALIGN(4K) :
    {
        KEEP(*(.data*))
    }

    .bss ALIGN(4K) :
    {
        KEEP(*(COMMON))
        KEEP(*(.bss*))
    }
   .text : { *(.text) }
   .data : { *(.data) }
   .bss  : { *(.bss)  }
   end = .;
}
