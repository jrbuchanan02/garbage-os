.section .init
.intel_syntax
.intel_mnemonic
.type start,@function
.global start
start:
    .cfi_startproc
    cli
    mov rsp, stack_top
    mov rbp, rsp
    mov rsi, bss_start
    mov rdx, bss_end
    xor rax, rax
zero_bss:
    mov [rsi], rax
    add rsi, 8
    cmp rsi, rdx
    jna zero_bss
    call kmain
    jmp halt
    .cfi_endproc
.end