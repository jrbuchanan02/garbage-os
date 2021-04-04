global error, start
extern init_paeging, init_paging, init_tgdt, init_gdt, init_idt, kmain, stack, cpu_assertions

section .text
bits 32

start:
    ;; at the point where we call start, here are our critical register states:
    ;; 
    ;;  eax -> should be Multiboot2 Magic
    ;;  ebx -> should point to the MB2 Info
    ;;  ecx -> undefined / zero
    ;;  edx -> undefined / zero
    ;;  esi -> undefined / zero
    ;;  edi -> undefined / zero
    ;;  esp -> undefined / zero
    ;;  ebp -> undefined / zero
    ;;  

    ;; first job, setup stack
    mov esp, stack

    cmp eax, 0x36d76289
    jne .not_mb2
    call cpu_assertions
    call init_paeging

    call init_tgdt
    ;;call long_mode
    
    call init_paging
    
    call init_gdt
    call init_idt
    
    call kmain
.not_mb2:
    mov al, 0x13    ;; (‼)
    jmp error

error:
    ; print "ERR: X" where X is the error code
	mov dword [0xb8000], 0x4f524f45 ;; RE
	mov dword [0xb8004], 0x4f3a4f52 ;; :R
	mov dword [0xb8008], 0x4f204f20 ;; [SP SP] (two spaces)
	mov byte  [0xb800a], al ;; overwrite second space w/ al
	hlt

long_mode:
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ret
section .bss


