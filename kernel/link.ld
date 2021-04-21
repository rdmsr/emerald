ENTRY(kmain)

SECTIONS
{
    . = 0xffffffff80200000; /* 2 MB in the higher half */
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
