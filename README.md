# Garbage OS
## The Garbage Collecting\* Operating System
\*Does not yet, in fact, perform garbage collection services on your code
## What is Garbage OS?
Garbage OS is an "ambitious" operating system made for educational value. Eventually, Garbage OS will support booting from Multiboot2, UEFI, and BIOS; roll its own filesystem; roll its own executable file type; 
perform automatic, periodic garbage collection (heap collection and defragmentation, handle-cleanup, etc.); support hardware accelerated preemptive multitasking; and (hopefully) provide some
support for these ISAs:
1. i286 (i.e., 16-bit protected mode)
2. i386 (i.e., 32-bit protected mode)
3. amd64(i.e., 64-bit long mode)
4. Itanium (hopefully, natively -- as opposed to in i386 compatibility mode)
5. EBC  (I've never heard of it either, but the UEFI standard references it as an architecture that UEFI supports)
6. MIPS 32
7. arm / thumb mixed instructions (i.e., 32-bit ARM)
8. aarch 64 (or, as I like to call it, "Apple Architecture 64")
9. RISCV-32
10. RISCV-64
11. RISCV-128 (though I have yet to find a compiler that supports this one -- but unlike with EBC, I've actually *looked* for a RISCV-128 compiler)

Garbage OS also intends on implementing "true" memory protection (the kind you find in most operating systems), mixing 16, 32, and 64-bit x86, and 
a rudimentary GUI.

## When can I use Garbage OS on my machine?
Well, you can **technically** use it on your machine now, i386 machines boot the current .iso file (though you would have to build it). However, I would not recommend 
using Garbage OS in its state right now: it only prints "X86?" (white text on a green background) to the top left corner of the screen, then halts the CPU. 

Here is a table of the combinations of architecture and bootloader, along with indications of how much (if any) support Garbage OS has for them:

Key:
X - this combination is not possible
N - no support for this architecture exists yet
M - Garbage OS currently *boots* this combination of architecture and bootloader
Y - All Garbage OS features are supported on this combination of architecture and bootloader

|     | BIOS | Multiboot 2 | UEFI |
| :-: | :-:  | :---------: | :--: |
| i286    | N    | X           | X    |
| i386    | N    | M / Y\*     | N    |
| amd64   | N    | M / Y\*     | N    |
| Itanium | N    | M / Y\* \*\*| N    |
| EBC     | X    | X           | N    |
| MIPS 32 | X    | N           | X    |
| ARM (32)| X    | X           | N    |
| AARCH64 | X    | X           | N    |
| RISCV32 | X    | X           | N    |
| RISCV64 | X    | X           | N    |
| RISCV128| X    | X           | N    |

\* Marked both M and Y because GOS currently has no features :/
\*\* TBH, I'm not sure if Itanium is fully backwards compatible with i386. Amd64 is not so marked because all x64 processors still put themselves into 
real mode after reset - then go to 32-bit protected mode when CR0.PE is set.
