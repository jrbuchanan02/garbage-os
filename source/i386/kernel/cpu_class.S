
.section .note.GNU-stack,"",@progbits
.section .text
.type check_cpuid,@function
.type get_this_thread_id,@function
.global get_this_thread_id


check_cpuid:
        /* Determine if the CPU supports CPUID
     * Since our code supports all the way back to an i386 or an early i486
     * CPUID is NOT guaranteed
     */
    .cfi_startproc
    pushf
    pop %eax
    xor %eax, 1<<21 /*bit 21 is the CPUID bit, flippable if the CPUID instruction is supported
                     *there are other methods to determine say an i386 versus a 8088 but none
                     *can differentiate an i386 versus an i486 so they aren't helpful.*/
    /*remember: AT&T syntax is backwards*/
    mov %eax, %edx
    push %eax
    popf
    pushf
    pop %eax
    /*remember: AT&T syntax is backwards*/
    cmp %eax, %edx
    je 1f
    xor %eax, %eax
    inc %eax
    ret
1:
    xor %eax, %eax
    ret
    .cfi_endproc

/*I know the numbered labels here are backwards, what matters
 *is that the numbered labels are unique.
 *This function also hits all the most common x86 instructions:
 * 1. register - register mov (x86 remains most popular non-orthogonal instruction set)
 * 2. call instruction with a 32-bit relative offset (would be common anyway)
 * 3. test al, al (checks if a function just returned false)
 * 4. xor eax, eax (zeros out eax)
 */
get_this_thread_id:
    .cfi_startproc
    call check_cpuid
    test %al, %al
    /*if check_cpuid returns zero, then cpuid is not supported*/
    jz 1f /*jump if zero*/
    xor %eax, %eax
    cpuid
    /*eax now contains the highest supported leaf*/
    /*remember: AT&T syntax is backwards*/
    cmp $0x1f, %eax /*v2 extended topology information*/
    jae 2f /*jump if above or equal: i.e., >=*/
    /*remember: AT&T syntax is backwards*/
    cmp $0x0B, %eax /* extended topology information*/
    jae 3f /*jump if above or equal: i.e., >=*/
    /*no extended topology supported, thread id already
     *in ebx
     */
    /*remember: AT&T syntax is backwards*/
    mov %ebx, %eax
    /*remember: AT&T syntax is backwards*/
    shr $24, %eax
    /*remember: AT&T syntax is backwards*/
    and $0xFF, %eax
    ret
3:
    /*parse extended topology enumeration leaf
     *we only care about the topology of the current
     *logical processor, however.
     */
    xor %ecx, %ecx
    xor %eax, %eax
    /*remember: AT&T syntax is backwards*/
    add $0x0B, %eax
    cpuid
    /*remember: AT&T syntax is backwards*/
    mov %edx, %eax
    ret
2:
    /*parse the v2 extended topology enumeration leaf
     *we only care about the topology of the current 
     *logical processor, however. This check is put
     *separately from the (v1) extended topology leaf
     *since it's recommended that, when available, the
     *v2 leaf be used instead of the v1 leaf.
     */
    xor %ecx, %ecx
    xor %eax, %eax
    /*remember: AT&T syntax is backwards*/
    add $0x1F, %eax
    cpuid
    /*remember: AT&T syntax is backwards*/
    mov %edx, %eax
    ret
1:
    xor %eax, %eax /*technically unnecessary*/
    ret
    .cfi_endproc

.end
