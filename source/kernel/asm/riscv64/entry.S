.section .init
.option norvc
.option push
.option norelax
.type start, @function
.global start

start:
    .cfi_startproc
    la gp, global_pointer
    csrw satp, zero
    la sp, stack_top
    la t0, bss_start
    la t1, bss_end
zero_bss:
    sd zero, 0(t0)
    addi t0, t0, 8
    bltu t0, t1, zero_bss
    la t0, kmain
    csrw mepc, t0
    call kmain
    tail halt
    .cfi_endproc
.option pop
.end