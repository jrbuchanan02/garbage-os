global start
extern start64, init_ram
section .text
bits 32
start:
    ; print `OK`
	mov dword [0xb8000], 0x2f4b2f4f
    mov esp, stack_top

    mov ebx, edi        ;; stash ebx (which holds multiboot 2 information) in edi

    call make_assertions    ;; make some assertions about our environment
                            ;; if these fail, error out here.
    
    call temp_init_mem      ;; temporarily initialize memory
    call enable_paging      ;; enable paging

    lgdt [gdt64.pointer]    ;; load global descriptor table - now in IA-32e long mode (Intel64 mode)
    jmp gdt64.code_segment:start64

	hlt


nmi_handler:
    iret


make_assertions:
    ;; first assertion to make: assert boot was from mb2

    cmp eax, 0x36d76289     ;; magic multiboot2 number
    jne .no_multiboot       ;; assertion failure if not equal

    pushfd
    pop eax                 ;; put flags into eax

    mov ecx, eax            ;; copy eax into ecx

    xor eax, 1 << 21        ;; flip cpuid bit in eax

    push eax
    popfd                   ;; put eax into flags

    pushfd
    pop eax                 ;; grab eax back - if cpuid, then bit 21 remains the same

    push ecx
    popfd                   ;; preserve flags

    cmp eax, ecx
    je .no_cpuid            ;; compare and assert that they are not equal

    mov eax, 0x80000000     ;; ask for extended information
    cpuid
    cmp eax, 0x80000001     ;; if extended information, eax >= 0x80000001
    jb .no_long_mode        ;; if eax < 0x80000001, then no long mode, and error out

    mov eax, 0x80000001
    cpuid                   ;; get information on whether processor supports long mode

    and edx, 1 << 29        ;; mask out all other bits
    cmp edx, 0              ;; if eax is now 0
    je .no_long_mode        ;; then there is no long mode - error out

    ret                     ;; assertions complete

.no_multiboot:
    mov al, 'I'             ;; I for "I don't know how I got here"
    jmp error
.no_cpuid:
    mov al, 'F'             ;; F for "your PC is a Fossil"
    jmp error
.no_long_mode:
    mov al, 'M'             ;; m for "no long Mode"
    jmp error

temp_init_mem:
    mov eax, page_tables.l3
    or eax, (1 << 0) | (1 << 1) ;; l3 table is present and writeable
    mov [page_tables.l4], eax
    
    mov eax, page_tables.l2
    or eax, (1 << 0) | (1 << 1) ;; l2 table is present and writeable
    mov [page_tables.l3], eax

    mov ecx, 0
.loop:
    mov eax, 0x200000       ;; reserve 2 MiB
    mov eax, ecx            ;; eax *= ecx
    or eax, (1 << 7) | (1 << 1) | (1 << 0) ;; l2 entry is huge, writeable, present (in that order)
    mov [page_tables.l2 + ecx * 8], eax
    inc ecx
    cmp ecx, 0x008          ;; loop 8 times, allocating 16 MiB
    jne .loop

    ret

enable_paging:
    call init_ram

    mov eax, page_tables.l4
    mov cr3, eax        ;; fun fact: while paging is disabled, cr3 could be used as a general purpose register
                        ;;
                        ;; DISCLAIMER: don't use CR3 as a general purpose register

    mov eax, cr4
    or eax, (1 << 5)    ;; enable PAE bit
    mov cr4, eax

    mov ecx, 0xC0000080 ;; magic long mode enable number
    rdmsr
    or eax, (1 << 8)    ;; turn on long mode
    wrmsr               ;; save our changes

    mov eax, cr0
    or eax, (1 << 31)   ;; long mode enable bit
    mov cr0, eax        ;; enable long mode (almost!)

    ret


error: ; print "ERR: X" where X is the error code
	mov dword [0xb8000], 0x4f524f45
	mov dword [0xb8004], 0x4f3a4f52
	mov dword [0xb8008], 0x4f204f20
	mov byte  [0xb800a], al
	hlt

section .bss
align 0x1000            ;; 4KiB
page_tables:
.l4:        resb 0x1000
.l3:        resb 0x1000
.l2:        resb 0x1000

stack_bottom:
    resb 0x4000         ;; 16 KiB
stack_top:


section .rodata
gdt64:
    dq 0
.code_segment: equ $ - gdt64
    dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53)
.pointer:
    dw $ - gdt64 - 1
    dq gdt64