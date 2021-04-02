global bioscall

bits 64

bioscall:
    mov ah, dl
    ;; long butt switch statement
    cmp cx, 0x0000
    je .int0
    cmp cl, 0x01
    je .int1
    cmp cl, 0x2
    je .int2

    ;; -- temporary snip --

.int0:
    int 0x0
.int1:
    int 0x1 ;; this is actually instruction
.int2:
    int 0x2

.into:
    into
.int_three:
    int3

    ret