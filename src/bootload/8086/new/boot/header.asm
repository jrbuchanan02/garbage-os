;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Multiboot (2) header                                                       ;;
;;                                                                            ;;
;; Author: Joshua Buchanan (@jrbuchanan02)                                    ;;
;; Version: 1                                                                 ;;
;; Created: 2022-03-21                                                        ;;
;; Modified: 2022-03-21                                                       ;;
;; Copyright: 2022 (C) Joshua Buchanan; LGPL version 2.1                      ;;
;;                                                                            ;;
;; Header for both GRUB Multiboot and GRUB Multiboot 2 specifications. Allows ;;
;; GRUB to directly boot GOS on x86 platforms while allowing access to the    ;;
;; power of UEFI on systems that support it.                                  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

MB1_HMAGIC equ 0x1BADB002
MB1_HFLAGS equ 0x7
MB1_HCKSUM equ - ( MB1_HMAGIC + MB1_HFLAGS + MB1_HDRADR + MB1_LODADR + MB1_LDNDAD + MB1_BSNDAD + MB1__ENTRY + MB1_MODETY + MB1_HWIDTH + MB1_HEIGHT + MB1_HDEPTH)
MB1_HDRADR equ 0x0
MB1_LODADR equ 0x0
MB1_LDNDAD equ 0x0
MB1_BSNDAD equ 0x0
MB1__ENTRY equ 0x0
MB1_MODETY equ 0x1 ;; ega
MB1_HWIDTH equ 80 ;; 80 cols
MB1_HEIGHT equ 25 ;; 25 rows
MB1_HDEPTH equ 4 ;; 4-bpp

MB2_HMAGIC equ 0xE85250D6
MB2_ARCHIT equ 0
MB2_LENGTH equ (mb2.end - mb2)
MB2_CHKSUM equ - ( MB2_HMAGIC + MB2_ARCHIT + MB2_LENGTH )

STACK_SIZE equ 0x4000 ;; 16 KiB

;; multiboot 1 header
section .header
NOPROGBITS
mb1:
    dd 0x1BADB002
    dd 0x7 ;; page align, available mem required, video info required, 
    dd MB1_HCKSUM
    dd MB1_HDRADR
    dd MB1_LODADR
    dd MB1_LDNDAD
    dd MB1_BSNDAD
    dd MB1__ENTRY
    dd MB1_HWIDTH
    dd MB1_HEIGHT
    dd MB1_HDEPTH
mb2:
    dd MB2_HMAGIC
    dd MB2_ARCHIT
    dd MB2_LENGTH
    dd MB2_CHKSUM
    ;; tags

    ;; end tag
    dw 0
    dw 0
    dd 8
.end:

section .text
bits 32
global mbmain
extern _kmain

;; entry point for multiboot systems.
mbmain:
    cli
    mov esp, stack + STACK_SIZE
    push ebx
    push eax
    call _kmain

section .bss
stack:
    resb STACK_SIZE