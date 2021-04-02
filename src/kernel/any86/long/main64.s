global start
extern memSort
extern main

extern handleDebug          ;; handle debug exception (exception #1)
extern handleNMI            ;; handle non-maskable interrupt.

extern handleUndefined     ;; handle undefined opcodes (exception #6)

MULTIBOOT_CONFIRM       equ     0x36d76289


start:
    cli                             ;; while this code is not timing critical,
                                    ;; we need to do some setup before using 
                                    ;; interrupts. Right now, the Interrupt 
                                    ;; Descriptor Table holds nothing (at best), 
                                    ;; and garbage at worst. The *last* thing
                                    ;; we need right now is our iPhone connecting
                                    ;; to our computer and the IDT doing something 
                                    ;; undefined!

    cmp eax, MULTIBOOT_CONFIRM      ;; assert that we booted via multiboot2
    jne not_via_multiboot_2         ;; if not, play it safe and take a nap.

    mov dword [boot_structure], ebx ;; store ebx - we'll need it later

    mov esp, stack                  ;; initialize stack. until it over / underflows
                                    ;; it will sit within our pre-allocated memory.
    
    pushfd                          ;; push flags onto stack
    pop eax                         ;; pop flags into eax
    mov ecx, eax                    ;; copy eax into ecx

    ;; attempt to flip ID flag, bit 21 - however, if we made it this far, 
    ;; I think we have cpuid. If we do not, we cannot flip the bit.
    ;; if not, we are running on a genuine x386 or x486 (not including SL-enhanced 486)
    xor eax, 1 << 21

    push eax                        ;; push eax onto stack
    popfd                           ;; pop eax into flags

    pushfd                          ;; push flags onto stack
    pop eax                         ;; pop flags from stack

    cmp eax, ecx                    ;; all important check

    jne on_a_hecking_fossil         ;; we are running on non-cpuid processor

    push ecx                        ;; push ecx onto stack (original flags now on stack)
    popfd                           ;; pop flags from stack

    mov eax, 0x80000000             ;; check if we can extended information
    cpuid

    cmp eax, 0x80000000             ;; if we can extended information, we should have at least 
                                    ;; 0x80000001 in eax.

    jb not_64bit_processor          ;; if we do not suport telling the OS that we are a 64-bit
                                    ;; processor, then we are not a 64-bit processor

    mov eax, 0x80000001             ;; ask for page 0x80000001
    cpuid

    and edx, 1 << 29                ;; test for long mode
    cmp edx, 1 << 29
    jne no_long_mode                ;; if no long mode, error out here.




    ;; last thing: load interrupt descriptor table - and test
    lidt idt
    ud ;; ud is like nop, but is always undefined.

    call main
.shutdown:
not_via_multiboot_2:    ;; don't assume anything about our frame buffer or the other
on_a_hecking_fossil:    ;; note the intentional fall-through
no_long_mode:           ;; note the intentional fall-through (again)
    mov [idt + 8 * 1], $;; load ourselves as the nmi exception handler.
    cli                 ;; just ensure that we wait here. 
    hlt                 ;; hlt will wait until next interrupt. CLI prevents that

first_ud_handler:
    ;; update idt
    mov [idt + 8 * 5], handleUndefined
    iret        ;; successfully return from interrupt
section .bss
boot_structure: resb 4  ;; cool "register" for our boot structure's address. 


align 0x4000            ;; align everything else in the bss section to 16 KiB
stack: resb 0x4000      ;; reserve 16KiB for stack

idt: resb 0x0800        ;; 256 * 8 bytes