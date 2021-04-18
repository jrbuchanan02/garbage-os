.global _start32
#define EAX_MAGIC 0x36D76289

#define NOT_MB2_PANIC           $0x01 // ☺
#define NO_CPUID_PANIC          $0x02 // ☻
#define NO_LONG_MODE_PANIC      $0x03 // ♥
#define THE_PROGRAMMER_PANICS   $0x04 // ♦
#define NO_MEM_MAP_PANIC        $0x05 // ♣
#define INSANE_MBI_PANIC        $0x06 // ♠

.equ CPUID_BIT, 1 << 21
.equ EXTENDED_CPUINFO, 0x80000000

#ifdef __LONG_MODE__
.extern _start64
#else
.extern kmain
#endif



.section .text


_start32:
    cli // allow us to work without accidentally resetting the processor :P
    mov stack_bottom, %esp // initialize stack
    mov %ebx, %edi


    cmp $EAX_MAGIC, %eax
    jne .not_multiboot2

    mov CPUID_BIT, %edx
    pushfl
    pop %ecx

    xor %edx, %ecx
    push %ecx
    popfl

    pushfl
    pop %eax
    cmp %eax, %ecx

    jne .no_cpuid_panic
    xor %edx, %eax
    push %eax
    popfl

    // if here, safe to cpuid

    // the below code might not be necessary
    mov $0x80000000, %eax
    cpuid
    cmp $0x80000000, %eax
    jle .no_long_mode_panic
    // the above code might not be nessary

    // this code asserts that long mode is here
    mov $0x80000001, %eax
    cpuid

    and $0x20000000, %edx
    jz .no_long_mode_panic

    // here is what we have to do:
    // 1. ensure that this computer is, in fact, an x64 computer (check)
    // 2. map the memory and such!

    mov %edi, %ecx // prepare to perform a C-style call

    call find_memory_map

    mov (%eax, 1), %bl

    add '0', %al
    jmp panic

    hlt // for now.
.not_multiboot2:
    mov NOT_MB2_PANIC, %ax
    jmp panic
.no_cpuid_panic:
    mov NO_CPUID_PANIC, %ax
    jmp panic
.no_long_mode_panic:
    mov NO_LONG_MODE_PANIC, %ax
    jmp panic
.the_programmer_is_panicing_too:
    mov THE_PROGRAMMER_PANICS, %ax
    jmp panic
.no_memory_map_panic:
    mov NO_MEM_MAP_PANIC, %ax
    jmp panic
_insane_mbi_panic:
    mov INSANE_MBI_PANIC, %ax
    jmp panic
panic:
    movl $0x4F524f45, 0xb8000 // red background -> 4, white foreground -> F
    movl $0x4F3A4f52, 0xb8004 // digits between 4 and F are VGA character, hex version of what alt-codes give
    movl $0x4F204f20, 0xb8008 // error codes are encoded with this system. i.e., error 1 is ☺ and error 24 is ↑
    movb %al, 0xb800a
    cli // ensure interrupts disabled
    hlt // halt cpu

find_memory_map:
    mov %ecx, %esi // make esi the list
    mov $0, %edi

    cmp $0, (%esi, %edi, 1)
    je _insane_mbi_panic

    add $8, %edi // move edi along to first entry.

.loop:
    cmp $0, (%esi, %edi, 1)
    je ..is_end_of_list
..is_not_end_of_list:
    cmp $6, (%esi, %edi, 1)
    je .break
    jne .continue

..is_end_of_list:
    add $4, %edi
    cmp $8, (%esi, %edi, 1)
    je _insane_mbi_panic
    sub $4, %edi
    jmp ..is_end_of_list
.continue:
    call .go_to_next_entry
    jmp .loop
.break:
    mov %esi, %ecx // restore %ecx
    mov (%esi, %edi, 1), %eax // return the address of the memory map.
    ret
.go_to_next_entry:
    add $4, %edi
    mov (%esi, %edi, 1), %eax
    mov %eax, %ebx
    and $7, %ebx
    sub $4, %edi

    add %eax, %edi
    add %ebx, %edi

    ret
.section .data

stack_top:
    .fill 0x4000,, 0
stack_bottom:

gdt:
    .fill 0x2000,, 0
idt:
    .fill 0x800,,  0
ldt:
    .fill 0x2000,, 0

.page_table_ptr: .fill 0x8,,0
.section .bss

