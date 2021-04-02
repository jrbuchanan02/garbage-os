global start


MULTIBOOT_CONFIRM       equ     0x36d76289

bits 32

extern memSort ;; the memsort function

section .text
main_asm:
    ;; we need to move the pointer to the memory map into edx
    ;; and the pointer to the size of the data into ecx

    mov eax, ebx ;; do not change ebx (is special!)
    add eax, 8
.find_memory_map:
    cmp eax, 6 ;; eax should now refer to the type of one of the fields

    je .make_call

    mov ecx, [eax + 1]  ;; grab size
    add eax, ecx        ;; move eax along

    mov edx, eax
    and edx, 7          ;; check if eax is aligned to 8 bytes

    cmp edx, eax        ;; perform the actual comparison detailed above

    je main_asm.find_memory_map

    add edx, 8          ;; account for padding
    mov eax, edx        ;; put padded eax into edx

    jmp main_asm.find_memory_map

.make_call:
    mov edx, eax + 16 ;; edx points to first entry
    mov ecx, eax + 4  ;; ecx now points to tag's size.

    push ebx ;; save ebx
    call memSort
    pop ebx ;; get ebx back

    ;; set up page tables...
    ;; first, we need to assert that we have PAE

.asssert_pae:
    mov eax, 0x01 

section .bss

