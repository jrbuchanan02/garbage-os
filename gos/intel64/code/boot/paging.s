%define WHEN_RAM_FLAGS (1 << 0) | (1 << 1) | (0 << 2) | (0 << 3) | (0 << 4) | (1 << 5) | (0 << 7)
%define FOR_ACPI_FLAGS (1 << 0) | (1 << 1) | (1 << 2) | (0 << 3) | (0 << 4) | (1 << 5) | (0 << 7)
%define FOR_PRES_FLAGS (1 << 0) | (0 << 1) | (1 << 2) | (0 << 3) | (0 << 4) | (1 << 5) | (0 << 7)
%define FOR_BRKN_FLAGS (0 << 0) | (0 << 1) | (0 << 2) | (0 << 3) | (0 << 4) | (1 << 5) | (0 << 7)

global init_ram
extern error        ;; allow erroring out here
section .text
bits 32

init_ram:
    mov ebx, edi    ;; get the mbi2info back
    mov ecx, [ebx]
    cmp ecx, 8
    jle no_multiboot2_tags
    mov eax, 8
    call move_along_mb2info

    mov eax, 6
    mov edx, 0
    call find_tag


    ret

move_along_mb2info:
    ;; adds eax to ebx
    ;; subtracts eax from ecx
    ;; if ecx <= 0, jumps to the not_enough_space error
    add ebx, eax
    sub ecx, eax
    cmp ecx, 0
    jle .not_enough_space
    ret
.not_enough_space:
    cmp edx, 0
    jne not_enough_space
    mov eax, -1
    ret

not_enough_space:
    mov al, 'E' ;; E for Enough space
    jmp error
no_multiboot2_tags:
    mov al, 'N' ;; N for no
    jmp error
find_tag:
    ;; points ebx to the tag with the id in eax
    ;; presumes that ebx is at the first tag

.loop:
    cmp dword [ebx], eax
    je .return
    ;; if not pointing there
    push eax
    mov eax, dword [ebx + 4] ;; move the size of the tags into eax
    call move_along_mb2info
    jmp .loop
.return:
    ret
section .bss
mem_size:
lo: resb 4
hi: resb 4