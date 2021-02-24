global enter_through32
extern main

bits 32
section.text


enter_through32:
    mov eax, 32
    push eax
    call main