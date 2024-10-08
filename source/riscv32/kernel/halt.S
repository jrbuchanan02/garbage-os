.section .text
.type halt,@function
.global halt

halt:
.align 4
    jal zero, halt
