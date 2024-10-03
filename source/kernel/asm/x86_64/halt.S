section .init
.intel_syntax
.intel_mnemonic
.type halt,@function
.global halt

halt:
    .cfi_startproc
    hlt
    jmp halt
    .cfi_endproc
.end