; While this file is not documented directly, here is what its job is:
; 1. determine whether we are on a 32 or 64 bit machine, while running
; 32-bit code on either is not an issue, we cannot run 64-bit code on a 
; 32-bit machine.
; 
; 2. setup paging, PAE, and the GDT
; 
; 3. call the appropriate function that then calls the kernel
; 
; 4. run all static constructors (and, by extension, all static destructors)


#ifdef i386
#define enter enter_through32
#elif
#define enter enter_through64
#endif

global start
extern enter
extern start_dtors, end_dtors, start_ctors, end_ctors

; x86-64 processors and x86 processors all start in IA-32 mode
; this file then, must be in 32-bits.
bits 32

section .text

start:
    ; initialize stack
    mov esp, stack_top ; all too easy :)
    cmp eax, 0x36D76289 ; check for multiboot2
    jne .multiboot_assert_fail

.multiboot_assert_fail:
    mov al, "B" ; b for boot
    jmp error


error:
    mov dword [0xb8000], 0x4f524f45
	mov dword [0xb8004], 0x4f3a4f52
	mov dword [0xb8008], 0x4f204f20
	mov byte  [0xb800a], al
.loop:
    hlt ; halt pauses until next interrupt
    jmp loop

section .data

stack_top:
    resb 0x4000 ; == 16KiB
stack_bot:

section .bss