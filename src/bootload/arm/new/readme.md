# src/bootload/arm/new
New speaks to all 64-bit ARM processors. While I suspect that this architecture
would, in a vacuum, be the easiest, there is one major challenge: I have no 
access to appropriate hardware (despite **owning a 64-bit ARM device**). While I
can use qemu to *emulate* a 64-bit ARM system, I cannot necessarily do so at 
full speed (and running these things on real hardware is important).

As such, the challenge with ARM-NEW is to use the community to help debug the
software (and have as similar as possible of an ABI to ARM-OLD)