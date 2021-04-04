global cpu_assertions
extern error
section .text
bits 32

cpu_assertions:
    mov esi, eax
    mov edi, ebx

    pushfd
    pop eax
    mov ecx, eax
    xor eax, 1 << 21

    push eax
    popfd
    pushfd
    pop eax
    cmp eax, ecx
    je .no_cpuid

    push ecx
    popfd       ;; restore flags

    mov eax, 0x80000000
    cpuid

    cmp eax, 0x80000001
    jl .no_extended_info
    mov eax, 0x80000001
    cpuid
    test edx, 1 << 29
    jz .no_long_mode

    mov ebx, edi
    mov eax, esi
    ret

.no_cpuid:
    mov al, 0x14    ;; ¶ sign, special p for cpuid
    jmp error
.no_extended_info:
    mov al, 0x78    ;; x, for eXtended info
    jmp error
.no_long_mode:
    mov al, 0x3f    ;; ?, for where is long mode
    jmp error