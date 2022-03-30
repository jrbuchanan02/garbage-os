# Kernel/loaders/bios
This directory contains the files necessary for loading in a BIOS-based bootloader.
BIOS is old and deprecated. But I see value in supporting as many systems as 
possible as part of the learning experience in GOS. BIOS stands for Basic
Input Output System and is fameous as the firmware on the orignal IBM-PC/AT and
its clones.

## How does BIOS load an operating system?
First, let's define a term: chipset-jutsu[^1]. Chipset-jutsu is any method that
varies from system to system which we would not be expected to know how to do. 

When the machine is reset (or powered on), BIOS uses chipset-jutsu to clear the
RAM on the machine and then sets up an interrupt table. When x86 has not entered
"protected mode" (where we can run an operating system), the interrupt table
always starts at address 0 and each entry is always 4-bytes in length. BIOS finds
the first "valid" disk in an internal list of disks and loads the first 512-bytes
of said disk into RAM starting at address `0x07C00`. The first code the operating
system runs is thus, at address `0x07C00`. The BIOS then checks for the magic
bytes `AA` `55` `FE` (I may have mixed up the `AA` and `55`), which mean that 
the disk is valid and bootable (not all bioses care about the last byte!). BIOS
then jumps to the start of the operating system.

According to the OS Dev wiki [here](https://wiki.osdev.org), there are three things
certain about the state of the system when we gain control:
1. 16 * `CS` + `PC` = `0x7C00` That is, we are at address `0x7C00`, however, the
values of the code segment and program counter registers are undefined.
2. `DL` represents the drive we loaded from.
3. There is a valid interrupt table (BIOS functions are interrupts)

We do not know the processor mode for certain, and we have to use the state of 
the FLAGS register to infer whether we can even boot (we can't run an operating
system on anything before a 286, and it's hard to support anything not 32-bits or
64-bits). We do not know the memory available on the system. We do not know what
devices are connected to the system or if they are in a valid state (BIOS requires
that the state be "functional", but the OS Dev wiki points out that this state
is not necessarily *legal*).

We know that the processor is generally in 32-bit real mode. We need an error
message prepared for when the processor is any other mode.


[^1]: More professionally, "chipset-specific methods" Since the term "jutsu" 
means art (as in "art of programming"), chipset-jutsu means pretty much the 
same thing and keeps the tone much lighter.