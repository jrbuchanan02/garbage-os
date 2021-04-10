initstack = 0x4000;
PHDRS
{
  boot PT_LOAD;                                /* one single loadable segment */
}
SECTIONS
{
    . = 0xffffffffffe02000;
    mmio = .;
    . = ALIGN(0x1000);
    
    bootboot = .;
    . += 0x1000;
    environment = .;
    . += 0x1000;
    framebuffer = .;
    . += 0x1000;

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
