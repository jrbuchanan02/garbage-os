# Garbage OS
Garbage OS is an open-source operating system[^1] that:
1. Does not take itself seriously
2. Features a centralized garbage-collection routine
3. Attempts to roll-its-own everything[^2]
4. Supports as many architectures as possible.[^3]

Garbage OS is published under the LGPL version 2.1. You can learn more about the
license and your rights with Garbage OS [here](./LICENSE) or by reading the 
LICENSE file (which that link points to).

**DISCLAIMER**: The information I present as facts below this point in the 
readme generally reflect what I have garnered from reading on the internet and
from personal experimentation. As such, this information may not all be correct
so do not cite this readme as a purely reputable source. If I am correct, 
corroborate with one of the sources I may list or with a more reputable source.

Garbage OS is currently in the alpha state. While previous builds have gotten to
run on emulator, I did not document what I was doing at all and have forgotten 
all that I did in the intervening year. So. The main purpose of this "reboot"
(pun intended) is to begin documenting almost everything about Garbage OS and 
its development (as you can see in the frequent README.md files). Here is a list
(possibly comprehensive) of all README.md's in the repository:
- [This file](./README.md)
- Bootloading
    + [8086-NEW](./src/bootload/8086/new/readme.md)
    + [8086-OLD](./src/bootload/8086/old/readme.md)
    + [8086-OLD: Why ASM?](./src/bootload/8086/old/entry/readme.md)
    + [ARM-NEW](./src/bootload/arm/new/readme.md)
    + [ARM-OLD](./src/bootload/arm/old/readme.md)
    + [IA64-CDH](./src/bootload/ia64/cdh/readme.md)
    + [IA64-NEW](./src/bootload/ia64/new/readme.md)
    + [IA64-OLD](./src/bootload/ia64/old/readme.md)
    + [MIPS-NEW](./src/bootload/mips/new/readme.md)
    + [PPC-NEW](./src/bootload/ppc/new/readme.md)
    + [PPC-NEW](./src/bootload/ppc/old/readme.md)
    + [RISCV-128](./src/bootload/riscv/128/readme.md)
    + [RISCV-x32](./src/bootload/riscv/x32/readme.md)
    + [RISCV-x64](./src/bootload/riscv/x64/readme.md)

## Roadmap
Garbage OS does not have a strong roadmap since revelopment just (re)started. I
am a busy college student. While that means I greatly appreciate the educational
value that GOS provides, I also have to worry about classes whose grades 
determine whether or not I graduate. I leave this roadmap here for both myself
and the kind developers who wish to help Garbage OS on its journey.

1. [-] Initialize the system in a sane manner where memory mapping can occur in a 
generally abstract memory map. 
    - This means, we get the computer to sit still after performing the boot 
    process. This sitting still should have occurred through a halt-like 
    instruction (like x86's `hlt` instruction) or, if that does not exist, a 
    spin-loop.
    - In this waiting state, we need access to the memory map. If EFI boot 
    services are still active, then GOS definitely needs the ability to
    translate all appropriate memory addresses.
    - There **must** be a valid stack.
    - GOS should know from what bootloader it came from. For some architectures
    such as 8086-NEW, this is a valid question as three potiential options
    exist.
    - Garbage OS should be entirely stable in this state. No interrupts from the
    machine should send execution out into the void (excl. a reset interrupt 
    such as `int 19` on the PC-architecture[^4]).
    - Garbage OS should ensure that the processor is in a mode where there is 
    memory protection and (if available) paging.
    - Garbage OS should have a unittesting mode that runs both on a host machine
    as a user-mode program and a unittest that can run when as an operating 
    system
2. [-] Allow an installation to take place
    - Garbage OS should have a copy-on-write filesystem which supports EIF[^5] 
    and journaling.
    - Garbage OS should have a friendly and easy to use intallation process 
    which copies only the information relavent to the host machine (while the 
    installation medium should be capable of installing to any architecture 
    that GOS supports).
    - Garbage OS should have a *rudimentary* GUI interface library at this 
    point.
    - Garbage OS, pursuing a microkernel structure, should have its own 
    executable file format at this point which supports:
        + Function multiversioning and automatic function resolution
        + Shared Memory between processes
        + Linking programs (like what ELF supports)
        + Multi-architecture files (like those in MACH-O)
        + Multithreading (similarly to how MACH-O supports multithreading)
        + Dynamic-Linking, Static-Linking, and Code Relocation (as well as 
        non-relocatable code).

3. [-] Provide a Consistent Application Program Interface
    - Garbage OS should have as identical as possible of an ABI[^6] across 
    architectures and a fully-interoperable API[^7]
    - Garbage OS should at this point implement the central garbage collection
    service.
    - Garbage OS should implement the following languages' standard libraries:
        + C (all ISO standards)
        + C++ (all ISO standards)
        + D (all versions 2.0 and later) with the footnote that the D-lang 
        garbage collection service will automatically clean up at the same 
        cadence it cleans for the rest of the operating system (which is 
        nonstandard as the D language garbage collector should only clean when
        explicitly asked to).
    - Garbage OS should have its own API and this API should allow the creation
    of:
        + GUI primitives (windows, buttons, labels, etc.)
        + Terminal Control stuff (like the terminal escape sequences)
        + Manages shared memory between applications. 
        + Allows setting advanced thread settings for processes[^8] (which 
        hardware thread a process is assigned to; what percentage of that thread
        is available for CPU use, etc.)
    - Garbage OS should have a terminal emulator which:
        + Fully emulates a VT-100
        + Fully supports ANSI.SYS commands
        + Fully supports ISO-terminal commands
            * Including ITU.410's commands.
        + Has equivalents for Linux's `OSC Pnrrggbb` and Windows's 
        `OSC ]4;n;rr/gg/bb ST` which both change the values of a specific 
        terminal color.
4. [-] Implement many, many, many, many, drivers.
    - Garbage OS should have the capability to install drivers. Drivers should
    near-universally be user-mode applications to limit the number of operating
    system panics.
    - Garbage OS should have an inviting development atmosphere to make 
    developing a driver for Garbage OS an easy task that can be done relatively
    quickly.
    - Garbage OS should have a package manager.
    - Garbage OS should have an anti-malware system built-in. This anti-malware
    system should detect a malware's payload (to potentially detect new threats)
    in addition to performing the usual definition checks. 
    - Garbage OS drivers should be open-source and public where possible (it is
    a project for education, after all), however, that is not a hard requirement
    as many companies wish to protect the inner workings of their hardware from
    the prying eyes of their competitors. The FOSS community respects that 
    sentiment.

[^1]: Well... It doesn't do much yet. But it's possibly getting there in a 
not-so-timely manner.
[^2]: To those who wish to create their own operating systems someday, this is
not recommended. Since GOS is for (my own) educational value, I decided to learn
as much as I can and see if I can possibly improve any such systems.
[^3]: The plan currently is to support i286 (if that's still possible), i386, 
AMD64, aarch32 (and related,32-bit ARM), aarch64 (and releated, 64-bit ARM), 
Itanium (if that demon can be tamed), MIPS, PPC, PC64, RISCV-128, RISCV-64, and
RISCV-32. Architectures and specific systems may be added and removed based on
availability (I would like to run all builds on appropriate hardware), whether
the OS actually runs on the hardware or not (a genuine question given that i286
only supports 1 MiB of address space), and whether or not programming for the 
system (in C or C++) remains possible (support does not seem to exist for 
RISCV-128, RISCV-32, or any Itanium on modern compilers).
[^4]: For those who don't know, 'true' PC's and most all clones (your PC is a
clone) send the interrupt 19 when the control, alt, and delete keys are pressed.
Since this interrupt is handled in hardware, this is how the Windows security
options work (all Windows has to do is set that interrupt to point to the 
security options text.) By default, on systems with BIOS (as opposed to EFI), 
`int 19` has the same effect as resetting the machine, but RAM is not cleared.
[^5]: EIF = Everything Is a File. In essence, file descriptors and the `FILE *`
type can be used to describe everything in the filesystem. Unix, most famously,
also attributes a path to everything with a file descriptor. While Garbage OS 
might not use the Unix-style filesystem, I do want that level of ease-of-access
to certain operating system features (like `/dev/rand` for access to a 
cryptographically secure random number generator).
[^6]: ABI = Application *Binary* Interface. This claim implies that a 32-bit
x86 application will work on a 64-bit target and call the same functions as its
64-bit peers and will not need to be made aware that it is running on a 64-bit 
system (if everything were perfectly compatible). A more reasonable goal is to 
allow a *compatible* ABI between all architectures, but, if you shoot for the 
moon and miss, you land among the stars.
[^7]: API = Application *Program* Interface. This claim implies that a 32-bit 
x86 application iwll work on a x64 target and not need to know that it is a 
32-bit application on a 64-bit machine (however, the operating system will need
to account for this difference and provide two different functions)
[^8]: Here, by "process" I mean "software thread." However, the same concept 
applies.
