.option norvc

.section .init
.type start,@function
.global start

start:
    .cfi_startproc
.option push
.option norelax
    la gp, global_pointer
.option pop
    csrw satp, zero
    la t0, halt
    /*make exceptions halt the processor, for now*/
    csrw stvec, t0
    la sp, stack_top
    la t0, bss_start
    la t1, bss_end
    addi t2, zero, 0
zero_bss:
    sw t2, 0(t0)
    addi t0, t0, 4
    bltu t0, t1, zero_bss
    la t0, kmain
    csrw sepc, t0
    call get_this_thread_id
    call kmain
    jal zero, halt
    .cfi_endproc
.end
