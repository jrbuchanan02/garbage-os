SECTIONS
{
    .text ALIGN(0x1000): {
        KEEP(*(.text .text.*))
        *(.rodata .rodata.*)
        *(.data .data.*)
    }
    .bss ALIGN(0x1000): {
        . = ALIGN(16);
        *(.bss .bss.*)
        *(COMMON)
    }
}