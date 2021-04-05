global init_paeging, init_paging
extern error, assemblePages, long_mode
section .text32
bits 32

init_paeging:
    mov eax, temp_pages.l3
    or eax, (1 << 0) | (1 << 1)
    mov dword [temp_pages.l4], eax

    mov eax, temp_pages.l2
    or eax, (1 << 0) | (1 << 1)
    mov dword [temp_pages.l3], eax

    mov ecx, 0
.loop:
    mov eax, 0x200000
    mul ecx
    or eax, (1 << 0) | (1 << 1) | (1 << 7)
    mov [temp_pages.l2 + ecx * 8], eax
    inc ecx
    cmp ecx, 64
    jle .loop

    mov eax, temp_pages.l4
    mov cr3, eax
    ret
section .text64
bits 32

init_paging:
    ;; call assemblePages, which will leave return values and such in eax and ebx.
    ;; if eax == 0, we can load the value in ebx.
    ;; otherwise, al is a character-error-code

    push edi
    call assemblePages
    
    

    mov cr3, edx    ;; lower 32-bits
    jmp long_mode
section .bss
align 0x1000
temp_pages:
.l4: resb 0x1000
.l3: resb 0x1000
.l2: resb 0x1000