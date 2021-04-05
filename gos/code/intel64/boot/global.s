global init_tgdt, init_gdt
extern error

section .text32
bits 32

init_tgdt:
    lgdt [tgdt.pointer]
    ret
section .text64
bits 64

section .rodata
align 0x1000

tgdt:
    dq 0
.code_segment: equ $ - tgdt
    dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53) ;; code segment's flags
.pointer:
    dw $ - tgdt - 1 ;; table's length
    dq tgdt ;; address