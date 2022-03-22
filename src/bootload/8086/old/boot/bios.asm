;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Loads from BIOS                                                            ;;
;; Author: Joshua Buchanan (@jrbuchanan02)                                    ;;
;; Version: 1                                                                 ;;
;; Created: 2022-03-21                                                        ;;
;; Modified: 2022-03-21                                                       ;;
;; Copyright: 2022 (C) Joshua Buchanan; LGPL version 2.1                      ;;
;;                                                                            ;;
;; Allows GOS to load from BIOS on i286-based systems (i.e., 8086-OLD). This  ;;
;; file (necessarily) assumes that the processor is still in real-mode as     ;;
;; there would be no easy way to switch out of protected mode while remaining ;;
;; within the 512-byte size limit imposed on us by the bios architecture.     ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

STACK_ADDR_LO 0x500
STACK_ADDR_HI 0x7BFF
STACK_SIZE equ 64
section .text
bits 16
global biosmain
extern kmain16

CPU 8086
biosmain:
    ;; ensure code segment is zeroed out.
    jmp (0x0000 wrt .entry):.entry
.entry:
    ;; save dl
    mov (seg 0x500):0x500, dl
    int 0x12 ;; request memory (available on all PC's)
    ;; AX now holds amount of low-memory on the system.
    ;; if memory >= 0x7E00, then move stack pointer to 0x7BFF
    ;; if memory < 0x7E00, then calculate where memory should
    ;; become contiguous and work from there.
    cmp ax, 0x7E00
    jl .stack_uh_oh ;; "uh-oh" since it's significantly more complicated than if
                    ;; we can infer that memory is contiguous in this region.

.setup_stack_simple:

    jmp .stack_join
.stack_uh_oh:

.stack_join:    ;; initialized stack at this point.
    ;; setup protected mode
    ;; ensure that main.asm is loaded from disk
    ;; jump to main.asm

    mov ax, seg kmain16
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ;; far jump to kmain16
    jmp cs:kmain16
.end:


%if biosmain.end - biosmain > 512
%error "BIOS section too big"
%endif