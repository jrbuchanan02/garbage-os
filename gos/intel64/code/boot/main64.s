global start64
extern kmain

section .text
bits 64


start64:
    ;; clear the segment registers
    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    mov rcx, rdi ;; bet you forgot about ebx being preserved!
    call kmain
    hlt
