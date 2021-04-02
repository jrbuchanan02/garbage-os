;; multiboot2 header for i386 and long mode systems.
;;
;; without this file, we would need to create our own bootloader from scratch.
;;
;; Since: 2021-04-01
;; Version: 2021-04-01
;;
;; Author: Joshua Buchanan (@Natara1939344)

bits 32

extern start

MODULEALIGN equ 1 << 0
MEMINFO     equ 1 << 1
FLAGS       equ MODULEALIGN | MEMINFO
MAGIC       equ 0xe85250d6
LENGTH      equ MultiBootHeader - header_end
CHECKSUM    equ -(MAGIC + FLAGS + LENGTH)

REQUEST_INFO_TAG_TYPE   equ     1


MEMORYMAP_REQUEST       equ     6
FRAMEBUFFER_REQUEST     equ     8


section .text

align 4
MultiBootHeader:
    dd MAGIC
    dd FLAGS
    ;; no architecture specified - evil hax >:)
    dd LENGTH
    dd CHECKSUM
    ;; the requesting info took longer to find out than I am willing to admit
    ;; reading other people's documentation is the hardest part of programming
    ;;                                      
    ;;                                  -- Joshua Buchanan, April 1st, 2021
    dw REQUEST_INFO_TAG_TYPE
    dw 0
    dd 16 ;; 16 bytes long
    dd MEMORYMAP_REQUEST
    dd FRAMEBUFFER_REQUEST  ;; request memory map first.
    
    ;; end tag, tells multiboot 2 that no more information is wanted.
    dw 0
    dw 0
    dd 8
header_end:


