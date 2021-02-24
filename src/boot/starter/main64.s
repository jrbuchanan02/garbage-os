global enter_through64
extern main

bits 64
section .text

enter_through64:
    mov rax, 64
    call main