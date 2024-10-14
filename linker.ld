ENTRY(start)

SECTIONS
{
    /* Align at a 4KB page boundary */
    . = 0x100000;
    
    /* Text (code) section */
    .text :
    {
        *(.text)
    }

    /* Read-only data */
    .rodata :
    {
        *(.rodata)
    }

    /* Data section */
    .data :
    {
        *(.data)
    }

    /* BSS section */
    .bss :
    {
        *(.bss)
        *(COMMON)
    }
}