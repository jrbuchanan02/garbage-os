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

section .hdr
align 8
header:
    dd multiboot2magicnumber
    dd multiboot2architecture
    dd multiboot2headerlength
    dd multiboot2checksum


    dw MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS
    dw MULTIBOOT_HEADER_TAG_OPTIONAL
    dd 12
    dd _start32
.end:
section .text
bits 32

_start32: ;; information for linker
    mov byte [0xb8000], 0x2f
    mov byte [0xb8001], "X"
%ifdef ___X64___
    mov byte [0xb8002], 0x2f
    mov byte [0xb8003], "6"
    mov byte [0xb8004], 0x2f
    mov byte [0xb8005], "4"
%elifdef ___X86___
    mov byte [0xb8002], 0x2f
    mov byte [0xb8003], "8"
    mov byte [0xb8004], 0x2f
    mov byte [0xb8005], "6"
%else
%error "AHHHHHHHHHHH!"

%endif
    hlt

section .bss


section .data