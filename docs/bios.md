# BIOS

While most people colloquially call all firmware "BIOS", that's fortunately not
true. While most PCs still have some form of BIOS implementation, hardly anything
boots via BIOS. Instead we use (U)EFI, a much more versatile standard.

However, Garbage OS will still support booting via BIOS.

## What "Standard" Defines

If you read the (short) 40-ish page standard on BIOS, you will find absolutely 
no descriptions on what any of the functions defined in the BIOS Data Area do.
You also (probably) won't find where the BIOS places the OS's loader, how large
that loader is, or really anything else that can help you write an Operating 
System. 

That part isn't really all to sad, the sad part is that the PC came out in 
1985-ish and the standard was published in 1996. 

However, there are a few things of note we can gather from the standard:

1. At some point in the boot process, the BIOS "initializes" all system devices.
The standard does not specify the state the devices are in and the OSDev wiki 
notes that some BIOS implementations are known to put devices an an illegal 
state. While some implementations are probably quite helpful in this 
initialization state, given the wild variety of initializations, the distinction
between "initializing" and "writing something to" seems lost in the lack of a 
well-defined standard.
2. The computer you're on is an x86 machine. BIOS is bound to the x86 
architecture. 
3. The processor is almost always in "Real Mode": no memory protections, 1 MiB
of address space, interrupt table hard-coded at address 0x00000, and segments
are always used. On occasion the processor is actually in protected mode, the
general advice in the hobbyist OS-development community is to assume these 
computers don't exist. Especially since they initialize 16-bit protected mode,
which isn't really useful to us.
4. The Boot Sector (first 512 bytes of the disk) is placed into memory starting
at address 0x07C00, if it ends in the magical bytes 0x55, 0xAA (the processor
reads this as the 16-bit number 0xAA55), then the BIOS assumes the entire sector
to be a valid program and starts running it.
5. The only guaranteed valid register is the DL register, which holds the 
current drive code. Technically, we know that `CS<<4+IP` equals 0x07C00, however
that puts absolutely no restrictions on the value of the code segment.

Since the x86 Real-Mode environment is fundamentally different from the 
Protected Mode environment, look at [this](real_mode.md) page for more info.

The BIOS memory map is as follows:

1. The first Kilobyte is defined as the <strike>interrupt table</strike> BIOS
Data Area. It's really the interrupt table. Suspiciously, all BIOS functions are
interrupts and suspiciously marked as hardware-reserved (eg, for things the 
processor considers errors).
2. Addresses 0x00000 to 0x9FFFF are RAM, if present. This RAM is guaranteed 
contiguous (meaning that if address X is RAM, addresses X - 1 to 0 are RAM) and 
the computer will invariably fail to boot if there is less than 0x07E00 bytes of
RAM. Dramatically so if there is no RAM where interrupt 19h should be, since the
BIOS standard says that's what it fires to tell itself to look for the operating
system, meaning that the system is either not contiguous or the computer has at
least 32KiB of data. BIOS reserves up to 128.25KiB (128 kilobytes + 256 more
bytes).
3. Addresses 0xA0000 to 0xFFFFF are not-RAM, if they exist. This general 
classification of "not-RAM" includes ROM (when the processor resets, it always
asks for the instruction at 0xFFFF0) for the BIOS, memory-mapped devices (video
hardware and bios code), and anything else the BIOS wants to put in 40% of 
the address space, if there is only 1MiB of addresses.
4. If the system can open the A20 gate, addresses 0x100000 to 0xEFFFFF are RAM
if it exists.
5. Conventionally, if it exists, addresses 0xF00000 to 0xFFFFFF are memory mapped
devices (the ISA memory hole)
6. Conventionally, 
