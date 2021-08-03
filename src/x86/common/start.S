%define MULTIBOOT_TAG_ALIGN 8
%define MULTIBOOT_TAG_TYPE_END 0
%define MULTIBOOT_TAG_TYPE_CMDLINE 1
%define MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME 2
%define MULTIBOOT_TAG_TYPE_MODULE 3
%define MULTIBOOT_TAG_TYPE_BASIC_MEMINFO 4
%define MULTIBOOT_TAG_TYPE_BOOTDEV 5
%define MULTIBOOT_TAG_TYPE_MMAP 6
%define MULTIBOOT_TAG_TYPE_VBE 7
%define MULTIBOOT_TAG_TYPE_FRAMEBUFFER 8
%define MULTIBOOT_TAG_TYPE_ELF_SECTIONS 9
%define MULTIBOOT_TAG_TYPE_APM 10
%define MULTIBOOT_TAG_TYPE_EFI32 11
%define MULTIBOOT_TAG_TYPE_EFI64 12
%define MULTIBOOT_TAG_TYPE_SMBIOS 13
%define MULTIBOOT_TAG_TYPE_ACPI_OLD 14
%define MULTIBOOT_TAG_TYPE_ACPI_NEW 15
%define MULTIBOOT_TAG_TYPE_NETWORK 16
%define MULTIBOOT_TAG_TYPE_EFI_MMAP 17
%define MULTIBOOT_TAG_TYPE_EFI_BS 18
%define MULTIBOOT_TAG_TYPE_EFI32_IH 19
%define MULTIBOOT_TAG_TYPE_EFI64_IH 20
%define MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR 21

%define MULTIBOOT_HEADER_TAG_END 0
%define MULTIBOOT_HEADER_TAG_INFORMATION_REQUEST 1
%define MULTIBOOT_HEADER_TAG_ADDRESS 2
%define MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS 3
%define MULTIBOOT_HEADER_TAG_CONSOLE_FLAGS 4
%define MULTIBOOT_HEADER_TAG_FRAMEBUFFER 5
%define MULTIBOOT_HEADER_TAG_MODULE_ALIGN 6
%define MULTIBOOT_HEADER_TAG_EFI_BS 7
%define MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS_EFI32 8
%define MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS_EFI64 9
%define MULTIBOOT_HEADER_TAG_RELOCATABLE 10

%define MULTIBOOT_ARCHITECTURE_I386 0
%define MULTIBOOT_ARCHITECTURE_MIPS32 4
%define MULTIBOOT_HEADER_TAG_OPTIONAL 1

%define MULTIBOOT_LOAD_PREFERENCE_NONE 0
%define MULTIBOOT_LOAD_PREFERENCE_LOW 1
%define MULTIBOOT_LOAD_PREFERENCE_HIGH 2

%define MULTIBOOT_CONSOLE_FLAGS_CONSOLE_REQUIRED 1
%define MULTIBOOT_CONSOLE_FLAGS_EGA_TEXT_SUPPORTED 2


;; assemble-time constants
multiboot2magicnumber       equ 0xE85250D6
multiboot2architecture      equ 0
multiboot2headerlength      equ header.end - header
multiboot2checksum          equ -(multiboot2magicnumber+multiboot2architecture+multiboot2headerlength)

;; can be moved to another file and (possibly?) made MIPS32 compatible
section .hdr
align 8
header:
    dd multiboot2magicnumber
    dd multiboot2architecture
    dd multiboot2headerlength
    dd multiboot2checksum


    ;;dw MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS
    ;;dw MULTIBOOT_HEADER_TAG_OPTIONAL
    ;;dd 12
    ;;dd 0 ;; for alignment?

    dd 0 ;; end tag
    dd 8

    dd _start32
.end:
section .text
global _start32
global setgdt
global hascpuid
extern kmain
bits 32

_start32: ;; information for linker
    mov byte [0xb8001], 0x2f
    mov byte [0xb8000], "X"
%ifdef ___X64___
    mov byte [0xb8003], 0x2f
    mov byte [0xb8002], "6"
    mov byte [0xb8005], 0x2f
    mov byte [0xb8004], "4"
%elifdef ___X86___
    mov byte [0xb8003], 0x2f
    mov byte [0xb8002], "8"
    mov byte [0xb8005], 0x2f
    mov byte [0xb8004], "6"
%else
%error "AHHHHHHHHHHH!"
%endif
    mov byte [0xb8006], '?'
    mov byte [0xb8007], 0x2f
    mov esp, stack.end
    mov ecx, eax
    mov edx, ebx
    call kmain
    hlt
setgdt:
    mov eax, [esp+4]
    mov [.gdtr+2], eax
    mov ax, [esp+8]
    mov [.gdtr], ax
    sgdt [.gdtr]
    jmp 0x08:.reloadcs ;; load code segment
.reloadcs:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret
.gdtr:
    dw 0 ;; limit storage
    dd 0 ;; base storage

hascpuid:
    enter 0, 0
    pushf
    pop eax
    mov edx, eax
    xor eax, 1<<21
    push eax
    popf
    pushf
    pop eax
    push edx
    popf
    xor eax, edx ;; only bit set should be cpuid
    sar eax, 21 ;; (1<<21)>>21 = 1
    ret
section .bss
align 0x1000
stack: resb 0x1000 * 4 ;; 4096 possible stack entries
.end:
section .data
align 0x1000