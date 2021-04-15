initstack = 0x4000; /* 16KiB stack per core. Generally, systems have much more RAM than that
                     * The Idea here is that more stack, 16KiB is not that much, will help out
                     * algorithms that rely on recursion, such as the Euclidiean Algorithm. 
                     */
PHDRS
{
  boot PT_LOAD;                                /* one single loadable segment */
}
SECTIONS
{
    fb =            0xffffFFFFfc000000;
    mmio =          0xffffFFFFf8000000;
    bootboot =      0xffffFFFFffe00000;
    environment =   0xffffFFFFffe01000;
    . =             0xffffffffffe02000;
    
    .text : {
        KEEP(*(.text.boot)) *(.text .text.*)   /* code */
        *(.rodata .rodata.*)                   /* data */
        *(.data .data.*)
    } :boot
    .bss : {                          /* bss */
        . = ALIGN(16);
        *(.bss .bss.*)
        *(COMMON)
    } :boot

    /DISCARD/ : { *(.eh_frame) *(.comment) }
}
