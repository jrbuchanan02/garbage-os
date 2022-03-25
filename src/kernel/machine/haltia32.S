; // Halt instruction for IA-32 Systems.

.globl halt

halt:
    cli; // clear interrupts (NMI still gets through)
    hlt; // halt processor (just this hardware thread)