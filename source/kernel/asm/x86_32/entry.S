.section .init
.intel_syntax
.intel_mnemonic
.type start,@function
.global start
start:
    .cfi_startproc
    cli
    mov esp, stack_top
    mov ebp, esp
    mov esi, bss_start
    mov edx, bss_end
    xor eax, eax
zero_bss:
    mov [esi], eax
    add esi, 4
    cmp esi, edx
    jna zero_bss
    call kmain
    jmp halt
    .cfi_endproc
.end