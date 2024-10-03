section .init
.intel_syntax
.intel_mnemonic
.type halt,@function
.global halt

/* Note: I've heard that some 486 processors have a bug in their halt instruction
 * which causes them to never resume after encountering a halt instruction (the
 * processor is supposed to remain halted until receiving an interrupt.*/
halt:
    .cfi_startproc
    hlt
    jmp halt
    .cfi_endproc
.end