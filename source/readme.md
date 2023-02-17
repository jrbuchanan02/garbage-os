# src

This folder contains notes and commentary on code within the source directory
(not including subdirectories!)

## metaarch.h

The file can be found [here](./metaarch.h)

### What do the various macros mean?

metaarch.h defines multiple macros based on the targeted architecture, these
are:

1. ARCH_BITS
2. ADDR_BITS
3. SIZE_BITS
4. FLAT
5. EVIL
6. ENDIAN

#### ARCH_BITS

ARCH_BITS refers to the amount of bits that the ALU processes at once. This
macro is useful as it refers to the largest integer size usable without a
performance loss.

#### ADDR_BITS

ADDR_BITS refers to the amount of bits the processor can use to access memory.
ADDR_BITS can be (often) smaller and (rarely) larger than size_t. Multiple
architectures may have the wrong value for ADDR_BITS. 

#### SIZE_BITS

SIZE_BITS refers to the smallest power of two integer that applications can use
to address all memory available to them.

#### FLAT

FLAT is 1 if the architecture uses flat memory model, 0 otherwise.

A flat memory model means that all addresses are equally accessible from some
instruction. For example, in 65c816 the instruction `lda $1234` can load 256 
different addresses based on context, that archetecture does *not* have a flat
memory model while the 68000 instruction `move D0, (FF000000)` functions 
identically to `move D0, (00000000)` on an original 68000, because one 
instruction's address always refers to one instruction it has a flat memory
model.

#### EVIL

EVIL is 1 if the architecture traditionally does not have C language support
**or** if the architecture is not designed with human-readable assembly in mind.

For example, even though x64 is an "orthogonal" instruction set that only
performs integer division on RDX:RAX, it is not evil simply because its assembly
remains human readable and the architecture has had C support for its entire 
lifetime.

However, 65c816 might not even have an available C compiler! Itanium assembly
was not designed to be human readable! Thus, in a debugging sense, it's entirely
possible that debugging the GOS kernel may mean much more effort in reading 
assembly or mean debugging the *compiler* as well.

Similarly, i286 is evil as GCC has never supported it.

#### ENDIAN

ENDIAN is 1 if the architecture is always or can be set to little endian without
a performance loss.

## How some things might work

This section is for describing how some systems within GOS might work. If you are
a developer for a modern operating system, you can take these ideas for your
own use. Even if you do not credit Garbage OS.

1. Updates that require restarting.
    - Establish time that the user is not using the PC.
    - Set wakeup timer (on EFI systems) using a call to 
    `EFI_SYSTEM_TABLE->BootServices->SetWakeupTimer`. According to the UEFI 
    standard, the PC will wake up at that time or the next moment it has 
    sufficent power. 
    - Proceed with the update as normal, clearing the wakup timer.
    <br> I do not know why Windows does not use this function for updates?
    (Windows requests that your computer remain on before updates)
