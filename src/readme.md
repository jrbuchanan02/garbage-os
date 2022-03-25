# src
This folder contains notes and commentary on code within the source directory
(not including subdirectories!)

## metaarch.h
The file can be found [here](./metaarch.h)

### On `EVIL` Architectures:

GOS marks architectures as evil if they are infamous in the development community
for being difficult in *two out of three* of the following ways:

1. To write compilers for (as in, more difficulty than normally writing a compiler)
2. Read the assembly listing; if the listing is even meant to be human readable
3. Adapt C / C++ / other high level languages to run in.

As such, here are the architectures that GOS defines as `EVIL`:
1. IA-64, also known as Itanium.
    - Itanium, which organizes all instructions into triplets each run at the 
    same time, is notoriously difficult to write a compiler for. Even Itanium
    debuggers are known to occaisionally have bugs which prevent their proper
    execution. 
    - Itanium code, relying on these compilers which had the unicorns of bugs, 
    was not meant to be human readable. As a result, Itanium assembly has garnered
     a reputation for itself as hard (if not impossible) to understand.
2. i286, also known as 16-bit x86
    - It is difficult to write compilers for as due to its age. The only two 
    compilers that I know of which can target i286 are below. The end result of
    this short list of an old host and ancient compiler is is that we may have 
    to *write our own* (increasing the difficulty significantly).
        + GCC on a Windows *3.1* host; yes, you need a DOS machine to target i286
        + Possibly some very old versions of MSVC
    - Because i286 uses a segmented memory model, it is even (falsely) beleived
    that it is incompatible with the C programming language (this is false, since
    C compilers exist for DOS). However, this also means that we cannot store a
    pointer to just anywhere in RAM anywhere on the processor. Said pointer has
    to be split between a segment register and another 16-bits. Meaning that 
    `size_t` is either one sixteenth of our address space or `size_t` is 4096 
    times the address space.
3. 65c816
    - C genuinely can barely target the 6502 processor (which is almost identical
    to the 65c816) as I only have heard of
    one compiler capable of targeting it. This compiler is not able to use 
    local variables in functions.
    - The stack is limited to 256 bytes in length. Our return addresses are 3 
    bytes long meaning that we have only around 85 functions to nest.

GOS will still work to support these `EVIL` architectures. `EVIL` generally means
that the compiler has a theoretically higher chance of bugs and that debugging
is more difficult. Perhaps leading to more debugging info dropped and hinting
to whoever reads a crash-log that they should attempt to replicate it on a similar
but not-`EVIL` system. It doesn't (and should **never**) reflect opinions on the
architecture (I quite love 6502), but instead reflects a perceived probability
of difficult-to-trace down bugs.