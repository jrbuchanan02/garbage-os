ENTRY(main)

SECTIONS
{
    .text : {
        KEEP(*(.text .text.*))
        KEEP(*(.rodata .rodata.*))
        KEEP(*(.data .data.*))
    }
    .bss : {
        KEEP(*.(.bss .bss.*))
    }
    
}