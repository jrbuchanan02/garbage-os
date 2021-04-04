global init_paeging, init_paging
extern error, parseMemmap, alignAddress, pointToMemoryMap
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
    ;; parse through the memory map and initialize memory per the mb2info structure
    ;;
    ;; 
    push edi ;; push mb2info
    call parseMemmap

    cmp eax, 0x00f100f1
    je .no_memmap
    cmp eax, 0x11E511E5
    je .no_memmap
    jne .yes_memmap
.no_memmap:
    mov al, 0x7f ;; ⌂, house for "go home, I'm not ready yet!"
    jmp error
.yes_memmap:
    ;; assemble page tables!
    
    ;; point to (sorted) memory map
    push edi ;; push edi again
    call pointToMemoryMap
    hlt

section .bss
align 0x1000
temp_pages:
.l4: resb 0x1000
.l3: resb 0x1000
.l2: resb 0x1000