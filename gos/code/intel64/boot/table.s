global init_gdt, init_idt
extern error

section .text
bits 32

init_gdt:
    mov al, 'G'
    jmp error

    ret

init_idt:
    mov al, 'I'
    jmp error

    ret