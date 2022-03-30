# Documentation: main.c and main.h
This file (and the associated header) represents a unified entry point for 
Garbage OS on all operating environments. So, regardless if we are an i286
system which booted from BIOS or if we are a RISC-V, EFI system running in 
128-bit mode, or a Power 10 supercomputer booting ... however those boot up, 
this function is part of the fine line between machine-specific code and code
universal to all machines running Garbage OS.

## List of functions
- [kmain](#kmain)



## kmain
This is the kernel "entry point". This function takes two parameters:

1. A pointer to a structure containing the information about the machine. As
of the current version of GOS, this structure is not yet defined. However, it
will include information on:
    - the memory map
    - the devices attached to the machine
2. A pointer to a structure which identifies the current operating environment.
This structure must contain information on:
    - what firmware loaded us (eg, BIOS, EFI, etc.)
    - what architecture we loaded from
    - whether we are actually running on an operating system (EFI programs are
    executable under some / most operating systems)

This function, eventually, will do the following:
1. Finish the loading process, loading all necessary drivers to establish a 
GUI and communicate with the hardware attached to the machine.
2. Enter the task switching routine.

kmain cannot return, as its returning would not make sense. If an error reaches
kmain, Garbage OS will reset the system or halt the processor. If the processor
receives an NMI after kmain has halted, the behavior is currently undefined.