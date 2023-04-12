# x86 Real Mode

As you may have read before redirecting to this page, x86 Real Mode operates 
in a fundamentally different manner from Protected Mode. Let's do a deep dive 
into the differences between the two modes.

## How x86 Divides up Memory

Formally, x86 divides memory into various segments. Most of the time, code is
only supposed to access data in the appropriate segment. In protected mode, 
the processsor declares a Segmentation Fault if a program violates these 
protections. In Real Mode, there are no memory protections (so no segmentation
fault is declared). I mention the segmentation fault since most people who have
indexed an array out of bounds in a C or C++ program have encountered one. 

Segments originate from the original x86 processors and actually form an 
interesting solution to a problem plauging computers at the time: 16-bits can't
address \*\*\*\*. Well, really 16-bits can access exactly 65536 things, but 
even 8-bit processors at the time needed more than that (don't believe me? the
Super Mario Brother's 3 cartridge is 256 kilobytes, four times this limit) and
we need to fit more than just ROM into our address space. 

There are three solutions that three different architectures came up with to 
solve this issue, in varying orders of processor complexity and program 
flexibility.

The 65c816, a 16-bit version of the 6502 (the processor in just about every 
8-bit thing in the 80s and 90s) defines an address to be 24-bits and just keeps
track of what the highest byte of the address was specified to be. This solution
works, but we can't programmatically control the highest byte, that is, we would
either have to use an indirect-indirect memory address in a fixed location 
(slow!) or always have the same high-byte of memory. 

The Motorola 68000 works a bit in-between a 16-bit processor and a 32-bit
processor, but since its data-path is the same as a 16-bit processor, the 
68000 is a 16-bit processor. The 68000 uses 32-bit registers and just ignores 
the highest byte of the address. This solution works for a 24-bit address and 
mostly expanded to a 32-bit system (where the highest byte was no longer 
ignored). Unfortunately, some 68000-based systems, such as the Apple Macintosh,
used the highest byte to store additional information about the memory --
causing issues with the transition to 32-bits. Other than that slight issue, 
this system is quite flexible but comes with one glaringly obvious issue: it 
makes the processor ridiculously complex. The Motorola 68000 has the same amount
of transistors as an Intel i386 or i486, you know, the processors with built-in
memory management, floating point operations, and actually 32-bit data paths!

Now let's look at segments. Segments are an in-between of the full 32-bit 
register and the "remember what the highest byte was" systems. An address in 
16-bit x86 is 20-bits long (2.5 bytes). The segment is multiplied by 16 (turning
it into a 20-bit number where the lowest four bits are always zero) and added
with the "offset" (what appears to be the address). Then this number is the 
memory location the processor asks for. This system allows the program to use
16-bit addresses and access more memory while also allowing a flexible control
of the entire address. Addresses in Real Mode x86 are referred to in 
segment-offset pairs. So, 0:65h refers to segment zero, offset 0x65.

Unfortunately, segments aren't always intuitive. 0000:10h, 0001:0h, and even 
FFFF:20h (under most circumstances) all refer to the same address in memory! In
fact, we can reach any address accesible from segment 0 using any segment.

Real Mode applies segments implicitly, this is different from protected mode 
where segments are only applied when the processor is explicitly told to. All 
instruction-fetches and branches implicitly use the code segment, `cs`, all 
stack-operations (pushes, pops, and referring to the stack or base pointers' 
addresses, or addresses relative to them) use the stack segment `ss` and all 
other memory operations use the data segment, `ds`. On 32-bit x86 processors 
there exist three more extra segments: `es`, `fs`, and `gs`, which always need
to be named explicitly to use them. I have read that the "e" in `es` stands for
extra, but I have never heard what the "f" and "g" stand for. I like to think 
they stand for at least something since all of the original 16-bit registers
and most of the 32-bit registers in x86 are acronyms x86[^1]

In other words, the Real Mode instruction `mov ax, [0x1234]` is more similar to
the Protected Mode instruction `mov ax, ds:[0x1234]`.

It should be noted, however, that Real Mode has the concept of an A20 gate. If 
this gate is opened, the address is actually 21 bits, giving another
(approximately) 64KiB of memory (it's really a bit less, but close enough). And
critically, FFFF:10h refers to address 0x10000 instead of address 0x0000. The 
method of opening the A20 gate requires external hardware. On BIOS-systems, it's
either already open, or we have to talk to the keyboard controller (you read 
that correctly, the keyboard controller), or we have to talk to something else
that will open it for us. We don't have to worry about opening the A20 gate in 
UEFI since that standard defines the processor to be in protected mode when the
loader gains control.

To exit Real Mode, we need to set up a Global Descriptor Table, Interrupt
Descriptor Table, and (optionally) one or more Local Descriptor Tables. We then
have to load the tables into the correct registers and probably exeucte a 
so-called long-jump (a jump to an absolute address in memory). Intel recommends
disabling interrupts during this process and communicating with external 
hardware to disable the NMI (non-maskable interrupt, the one the processor never
ignores) during this (and really all) mode switches. 

[1] If `FS` and `GS` stand for something, then the only register on x86 that 
lacks an abbreviated name / is not an acronym is the `FLAGS` register, which is
fully spelled out unless you refer to the 32-bit version, `EFLAGS` or 64-bit 
version `RFLAGS`. The upper half of `RFLAGS` is reserved, so the only reason to
even check `RFLAGS` is that x64 requires the stack to be aligned on a 64-bit 
(8-byte) boundary. 