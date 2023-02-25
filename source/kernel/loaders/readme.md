# source/kernel/loaders

This directory contains code unique to each form that the Garbage OS Loader 
takes. 

This file will also document a bit about each standard.

## BIOS

I jokingly like to call BIOS an "unstandard". While IBM, Intel, and Pheonix
published a 47-ish page standard in the 90s, BIOS *still* does not describe the
way it sets up devices, where data structures are located in memory, and even 
whether there is memory!

What BIOS *does* guarantee however is that these regions are RAM:

the 512 bytes from address 0x7C00 to 0x7E00.

and that these regions are not-RAM:

addresses 0xA0000 to 0xFFFFF.

BIOS also generally "promises" that the processor will be in 16-bit real mode,
however, per the OSDev Wiki, some rare BIOSes will set up some form of protected
mode anyway. 

BIOS also only loads 510 bytes of code to start the operating system. Why 510?
The boot sector is 512 bytes but is only considered valid if the last two are
0xAA55. So in 510 bytes, the first-stage loader *must* find a suitable region
of memory to load the second stage loader and load it from the disk into that
region of memory. Bonus points if the first-stage loader can display an error 
when that is not possible and fills the entire 510 bytes with meaningful code
-- preventing some bad-actor from also living in the boot sector.

This second stage loader would have to query processor and system capabilities.
In 16-bit real mode, since we have no guarantee what the processor looks like, 
we would have to look at the processor flags and make an inference. Fortunately,
Intel has some handy documentation and some flag bits marked as for use in 
protected mode only behave differently on an 8086/8088/80186, an 80286, and all
32-bit or later x86 processors. Once we determine if we're on a 32-bit
processor, the second stage loader would check for the CPUID instruction. If it
exists, it would then check for PAE support then 64-bit mode support.

In the case that we run on something older than a 286, no operating system can
run since no memory protections exist. We would be forced to error out. If
Garbage OS can be made to run on less than 640k of RAM, a possibility might 
exist to suppport GOS on 286-based systems. GOS will support 32-bit and 64-bit
systems with BIOS support and possibly before gaining BIOS support.

In some regions of BIOS data, ACPI requires its RSDP table be stored. We have to
search approximately one megabyte of addresses to find the RSDP table. It can
either exist in the first kilobyte of the BIOS Extended Data Area or anywhere
in the BIOS Data Area.

## CFE

To be added. It's possible that CFE support will never exist in GOS, but we
can dream.

## (U)EFI

UEFI is the successor to BIOS. It starts firmwares and the operating system 
loader during a boot period (all such binaries are the same format as a .exe
file) and starts them with a handle to the "disk" they came from and a pointer 
to the EFI System Table, EFI calls all its types in `THIS_FORMAT`, but GOS 
prefers to name the types in `this_format`. EFI provides all necessary system
information in the System Table or in a table within the system table. Most 
importantly to the EFI loader are the methods to load things from the disk, log
to a stdout (EFI guarantees that it seems to work to the software, not that it
does anything), obtain the memory map, obtain system metadata (such as the 
ACPI RSDP), and exit the boot process.

## Multiboot

Multiboot is technically its own bootloader. GOS's Loader would take the form of
a second stage loader on top of the multiboot system. Multiboot support is
legacy today and may never find its way into GOS, but we can dream.

## Multiboot 2

Multiboot 2 is technically its own bootloader. GOS's Loader would take the form
of a second stagge loader on top of the multiboot 2 system. Multiboot 2 only
supports 32-bit x86 and 32-bit MIPS, both aging architectures. Thus Multiboot 2
support may never find its way into GOS, but we can dream.

## OPAL

OPAL, or Open Power Abstraction Layer, is an IBM proprietary firmware for use
in systems that use the Power architecture. OPAL support may be implemented 
after successful EFI / BIOS support.