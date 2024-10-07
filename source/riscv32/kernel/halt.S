.section .text
.type halt,@function
.global halt

halt:
    jal zero, halt
