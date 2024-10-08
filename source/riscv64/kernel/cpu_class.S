.option norvc

.section .text
.type get_this_thread_id,@function
.global get_this_thread_id

/*Compare this function with the x86 version and you
 *quickly see why x86 is kinda bad*/
get_this_thread_id:
    .cfi_startproc
    csrr a0, mhartid
    jalr zero, 0(ra)
    .cfi_endproc
.end
