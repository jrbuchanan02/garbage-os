.global _start32

//.extern foo
//.extern get_memory_map

#define EAX_MAGIC 0x36D76289

#define NOT_MB2_PANIC                       $0x01 // ☺
#define NO_CPUID_PANIC                      $0x02 // ☻
#define NO_LONG_MODE_PANIC                  $0x03 // ♥
#define THE_PROGRAMMER_PANICS               $0x04 // ♦
#define NO_MEM_MAP_PANIC                    $0x05 // ♣
#define INSANE_MBI_PANIC                    $0x06 // ♠
#define C_PROGRAMS_NOT_ACCESSABLE_PANIC     $0x07 // •

.equ CPUID_BIT, 1 << 21
.equ EXTENDED_CPUINFO, 0x80000000
.equ FOO_RESPONSE, 42
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

    call .find_memory_map


    hlt // for now.
.find_memory_map:
    // mb2 info

    // (size)
    // (reserved)
    // 
    // tag -> (type, size) [padding]
    // tag -> (type, size) [padding]
    // ... 
    // tag -> 0, 8

    mov %ecx, %esi
    mov $8, %edi

..loop:
    jmp ..get_tag_type
...gotten_tag_type:
    cmp $0, %eax
    je .insane_mbi_panic

    cmp $6, %eax
    je ..found_it

    // todo: add more?

    jmp ..go_to_next
...went_to_next:
    jmp ..loop
..get_tag_type:
    mov (%esi, %edi, 1), %eax
    jmp ...gotten_tag_type
..go_to_next:
    mov %edi, %ebx
    add $4, %ebx
    mov (%ebx), %ebx
    add %ebx, %edi
    and $7, %ebx
    add %ebx, %edi
    jmp ...went_to_next
..found_it:
    mov %edi, %eax
    add %esi, %eax
    mov %esi, %ecx
    ret
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
.insane_mbi_panic:
    mov INSANE_MBI_PANIC, %ax
    jmp panic
.no_c_programming_language_panic:
    mov C_PROGRAMS_NOT_ACCESSABLE_PANIC, %ax
    jmp panic
panic:
    movl $0x4F524f45, 0xb8000 // red background -> 4, white foreground -> F
    movl $0x4F3A4f52, 0xb8004 // digits between 4 and F are VGA character, hex version of what alt-codes give
    movl $0x4F204f20, 0xb8008 // error codes are encoded with this system. i.e., error 1 is ☺ and error 24 is ↑
    movb %al, 0xb800a
    cli // ensure interrupts disabled
    hlt // halt cpu
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

