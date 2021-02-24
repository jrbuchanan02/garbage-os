; this file's job is onefold: Multiboot2 header
section .multiboot_header

MAGIC equ 0xe85250d6 ;; multiboot 2 header's magic number
PMODE equ 0x00000000 ;; 0 -> i386 protected mode
LNGTH equ header_end - header_start
CHECK equ 0x10000000 - (MAGIC + PMODE + LNGTH)

header_start:
    dd MAGIC
    dd PMODE
    dd LNGTH
    dd CHECK

    ;; these three are the end-tag
    dw 0
    dw 0
    dd 8
header_end: