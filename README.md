# D-Based Operating System
## What is it?
Dlang-based-operating-system is intended to create a new operating system that runs entirely in D, 
this will have some benefits, namely a centralized garbage collector for the entire operating 
system. Due to limitations in multiboot2, nasm, and gas (although the gas limitations are probably
*limitations from me*), I can only provide support for i386 and x86-64. When possible, I will add 
support for ARM (specifically aarch64). __The difficulty with supporting ARM and AARCH64 is__
__that they both require creating a custom bootloader from scratch (at the moment)__ 
## How is it better?
The advantage of using D over the more "traditional" systems-programming languages C and C++ is 
that with D, there is a built-in garbage collection service, array-type, and support for utf-32
strings. Moreover, unlike Rust (which was considered for this task), D makes compatibility with 
other C-languages a core part of the language (while C-linkage is possible in Rust, it is not as 
critical a component of the language).
## Where can I find the documentation for DBOS?
I'm working on that :). Fortunately, D has a builtin documentation feature (I just need to find
a method by which I can *host a server* for the files)
# Contributions
## Conventions
 - For D-code, follow the language conventions.
 - For the assembler code, use intel-syntax + NASM (which uses intel-syntax anyways)
    + side-note, as / gas will not accept the NASM dialect of assembly, TASM and MASM probably 
    won't either. Before warning me about every operation having an error, check that you *are* 
    *in-fact using NASM*. 
 - Also for assembler code and such, since D has a builtin assembler-code writing feature, the only
 assembly files should be the boot-loader itself. (until ARM support is added, but then I might 
 switch to ldc2 instead of gdc because ldc has support for ARM-inlined assembly).
 - Use classes and make them private and final where possible
 - Use properties and encapsulate members where possible.
 - Use as many of the "potentially optimizing" attributes as possible (though not pragma(inline) 
 as the kernel should still fit in memory). These are pure, nothrow, @safe, and @nogc. 
## Bug Fixes
### Security
While I'm too poor to offer a bug-bounty, please, _please_ _*please*_, __**PLEASE**__ tell me about
any and all security issues found and / or provide some route to fix them (such as, for example, 
fixing it yourself, if you happen to be knowledgeable in that stuff or even a link to the appropriate
google search). 
### Typos
If you know the proper way to spell the wrod, please fix it yourself.

Note: The typo in this section, "wrod" is intentional to show that I am a terrible speller.
### The unexpected-behavior type bugs
Feel free to contribute and / or let me know about the bug if it is not labeled in a DDOC comment.
## Documentation
Feel free to add to the documentation. If it's a significant change (such as detailing an important
side-effect) you can add your name to the author's list.
## Features
Feel free to take care of anything listed as an issue. 
# Compiling it yourself
Instructions coming soon
# Making Applications for DBOS
## **DISCLAIMER**
Since DBOS is currently in a snapshot-style state, expect any and all API calls to change. Please 
ensure that your application is prepared accordingly. Here are some examples of by how much it may
change:

 - Multiboot 2 loads an 80-column text mode, however eventually DBOS will be a graphical operating 
 system, not unlike the one that you are using to read this file.
 - DBOS will eventually implement the D runtime (in D), however, it has not yet--so until that 
 happens, there will be equivalent runtime-calls but with different names and different package 
 names.
 - DBOS will make use of gtk-d and / or gtk. However, that will have to wait until DBOS works in 
 bitmapped graphics mode.
 - DBOS currently lacks any c / c++ / obj-c / etc. bindings, these will come soon (to allow the 
 easier porting and potentially WINE-like support of existing POSIX / Windows applications)
 - DBOS does not have any filesystem support.
## Conventions, I guess
 - Like with other operating systems, please indicate that the executable is for DBOS
 - I'm planning on implementing the ELF format, because of its speed, versatility, and commonality.
 - If you need to know, all architectures I'm planning to port DBOS to use little endian addresses 
 (while ARM-architectures do use big.LITTLE, addresses are always little endian, if you don't 
 believe me, read up on the architecture--I almost attempted implementing a custom bootloader and 
 ensured that I would not have to worry about endianness for memory-addresses)
 - DBOS will have a built-in garbage collection service, so at the beginning of the application, it
 should be linked to (I would think, however, this note is probably only of interest to
  compiler-developers)
# Unironically using DBOS on your machine
## Boot Errors
For **DISCLAIMERS** see the so named section under Making Applications for DBOS. Just know that 
for these snapshot-style version, you may need to upgrade frequently. DBOS is published under the
LGPL (Lesser General-use Public License) and thus I am not responsible for any damages it causes 
nor do I make any promises about its capabilities. However, if you trust this operating system with
your computer, by all means, feel free to use it.

DBOS may error out at boot due to a setup that it cannot account for. The error codes (and how to 
fix them) are documented here:
### ERR: B
 - This code means that the operating system was loaded, but not by a Multiboot2 compliant 
 bootloader. 
 - This error is non recoverable and probably will not be fixed by a restart
 - Fix this error by ensuring that you installed the operating system properly. The bootloader 
 that DBOS is compiled with should remain Multiboot2 compliant.
### ERR: C
 - This code means that your CPU does not support the cpuid instruction. Seeing this error likely
 means that your computer's processor is too old to run DBOS. 
### ERR: L
 - This code occurs when your CPU is attempting to run the 64-bit distribution of DBOS but your CPU
 is not a 64-bit CPU. This issue can be fixed by installing the 32-bit distribution
### ERR: P
 - This code occurs when your CPU does not support Physical Address Extensions (PAE). Like with 
 the ERR: C code, this error likely occurs because the target CPU is too old to run DBOS.
