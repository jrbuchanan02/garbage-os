ENTRY(start)

SECTIONS {
    . = 2M;
    .text : ALIGN(4K)
    {
        *(.multiboot)
        *(.init)
        *(.text)
    }

    .rodata : ALIGN(4K)
    {
        *(.rodata)
    }
    
    .data : ALIGN(4K)
    {
        *(.data)
    }

    .bss : ALIGN(4K)
    {
        PROVIDE(bss_start = .);
        *(.bss)
        *(COMMON)
        . = ALIGN(4K);
        PROVIDE(stack_top = .);
        . += 4096;
        PROVIDE(global_pointer = .);
        PROVIDE(bss_end = .);
    }
}