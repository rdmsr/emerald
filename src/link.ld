ENTRY(kmain)

SECTIONS
{
    . = 0xffffffff80200000; /* 2 MB in the higher half */
    .stivale2hdr :
    {
        KEEP(*(.stivale2hdr))
    }

    .text :
    {
        KEEP(*(.text*))
    }

    .rodata :
    {
        KEEP(*(.rodata*))
    }

    .data :
    {
        KEEP(*(.data*))
    }

    .bss :
    {
        KEEP(*(COMMON))
        KEEP(*(.bss*))
    }
   .text : { *(.text) }
   .data : { *(.data) }
   .bss  : { *(.bss)  }
   end = .;
}
