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
    . = 0xffffffffffe02000;
    . = ALIGN(0x1000);
    . += 0x1000;
    mmio = .;
    . = ALIGN(0x1000);
    framebuffer = .;
    . += 0x1000;
    . = ALIGN(0x1000);
    bootboot = .;
    . += 0x1000;
    . = ALIGN(0x1000);
    environment = .;
    . += 0x1000;
    . = ALIGN(0x1000);

    .text : {
        KEEP(*(.text.boot)) *(.text .text.*)   /* code */
        *(.rodata .rodata.*)                   /* data */
        *(.data .data.*)
    } :boot
    .bss (NOLOAD) : {                          /* bss */
        . = ALIGN(16);
        *(.bss .bss.*)
        *(COMMON)
    } :boot

    /DISCARD/ : { *(.eh_frame) *(.comment) }
}
