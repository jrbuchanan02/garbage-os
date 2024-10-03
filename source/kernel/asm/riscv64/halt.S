.section .init
.option norvc
.type halt, @function
.global halt

halt:
    .cfi_startproc
    jal zero, halt
    .cfi_endproc
.end