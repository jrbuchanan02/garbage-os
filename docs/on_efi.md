# On EFI
Garbage OS, while it did not create UEFI and UEFI is documented in detail [here](https://uefi.org/sites/default/files/resources/UEFI_Spec_2_9_2021_03_18.pdf), 
keeps its own documentation on the Unified Extensible Firmware Interface for convenience / educational purposes.

## What is UEFI

## How UEFI Works

## Why is UEFI useful to Operating Systems.
UEFI provides a singular method for Firmware (the code which knows how to run a specific device) to communicate with the Operating System (us). So, the Operating 
System does not need to know how to manage every possible printer, just how to find the correct Firmware to talk to. While a large group of developers could
feasably implement support for every possible printer, when you remember that the same methods have to happen for *every* device out there, you really see the
appeal of UEFI. 

In addition, UEFI (during the boot process) provides implementations typical to an Operating System, which helps in designing a [microkernel architecture](#microkernels)


## Microkernels
An operating system needs to be stable. However, the more things that are going on in code means the greater likelyhood of a crash. If all of our OS is in one binary,
and just one part errors out, the entire system crashes. The solution: break the kernel (the core part of our operating system) into as many small pieces as possible. If
one small piece crashes and cannot recover, the system may still be able to function in a limited capacity (as opposed to crashing, destroying data, or awaking skynet).
