/* AT&T syntax is alien to me but this code works
 * the syntax is foreign not just because everyone uses
 * intel syntax but because the destination, in other ISAs
 * is almost always on the left hand side.*/ 
.set ALIGN,     1 << 0 /*.set -> %define*/ 
.set MEMINFO,   1 << 1
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC /*.long -> dd */
.long FLAGS
.long CHECKSUM

.section .note.GNU-stack,"",@progbits

.section .init
/*global is the same, start is required*/ 
.type start,@function
.global start
start:
    .cfi_startproc
    cli
    mov $stack_top, %esp /*AT&T syntax flips source and destination, it also requires a $ before constants and a % before registers*/
    mov $bss_start, %esi
    mov $bss_end, %edx
    xor %eax, %eax /*this instruction is actually almost identical*/
1:
    /*this is mov [esi], eax*/
    mov %eax, (%esi)
    add 4, %esi
    /*I think this is correct, esi should be less than edx*/
    cmp %edx, %esi
    jna 1b
    call kmain
    jmp halt
    .cfi_endproc
.end
