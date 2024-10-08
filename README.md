# Garbage OS
Garbage OS is an open-source operating system that:
1. Does not take itself seriously
2. Is intended to possibly educate on the functionality of Operating Systems
3. Intends to support as many architectures as possible

Garbage OS is distributed under the LGPL version 2.1 with the hopes that
it will be useful. See the LICENSE file for more details.

## Roadmap

All unchecked boxes are subject to change.

1. [ ] Create a basic Kernel (version 0.x)
    - [ ] Kernel version which spins the CPU indefinitely (0.0.x)
    - [ ] Kernel version which features preemptive multitasking through an idle process and an init process that spins in place (0.1.x)
    - [ ] Kernel version with preemptive multitasking and memory protections but nothing else (init process can be configured at compile-time to intentionally cause a kernel panic) (0.2.x)
    - [ ] Kernel version with preemptive multitasking, memory protections, and the ability to load drivers (init process contains a list of drivers to start) (0.3.x)
2. [ ] Create a commandline-based, self-hosted Operating System (version 1.x)
    - [ ] Implement an ext2 filesystem driver that the kernel can correctly read and write to (1.0.x)
    - [ ] Implement an installer which can set up a disk such that the kernel loads from that disk on startup (1.1.x)
    - [ ] Write a command-processor program which runs (last) on startup and responds to user commands by either interpreting them
          directly as a command or running the appropriate executable file (1.2.x)
    - [ ] Port over programs necessary for the Garbage OS build process (1.3.x)

## Why The Name Garbage OS?

It's kind of unprofessional, right? Originally, I intended for Garbage OS to be
written in the D programming language and feature an OS-wide garbage collection
service. Later I realized just how complex a garbage collector would make the 
operating system! However, the name "Garbage OS" kind of stuck: it's ironically
funny to load a kernel named "garbage.bin" and have it run properly.

## Ports

| Instruction Set | Version Added | Status |
| :-------------- | :-----------: | :----: |
| i386            | 0.0.0         | in development |
| ~~x64~~             | ~~0.0.0~~         | ~~in development~~ delayed |
| RISC-V 32       | 0.0.0         | in development |
| RISC-V 64       | 0.0.0         | in development |

Support notes:

i386: Garbage OS assumes its kernel receives control in protected mode with ring 0 privileges.

~~x64: Garbage OS assumes its kernel receives control in long mode with ring 0 privileges.~~
x64: Garbage OS will assume its kernel receives control in 32-bit protected mode with ring 0 privileges
then proceed to initialize 64-bit mode accordingly.

RISCV-32: Garbage OS requires the RV32I base instruction set, control status registers (Zicsr),
and Sv32 (i.e., Supervisor mode, User mode, and 32-bit paging). Most RISC-V processors will run
the RISC-V 32-bit version of the kernel.

RISCV-64: Garbage OS requires the RV64I base instruction set, control status registers (Zicsr),
and Sv39 (i.e., Supervisor mode, User mode, and 39-bit paging). Most 64-bit RISC-V processors
will run the RISC-V 64-bit version of the kernel. 

## Version Naming

Garbage OS versions come in three numbers. The first number represents the major milestone of
Garbage OS development. Versions where the first number is zero represent just the basic kernel, for example.
The second number in the version is the phase of that major milestone, for example versions starting
with 0.0 contain a kernel that (should) successfully load up on the target system and proceed to 
hold the processor in an infinite loop. The third number in the version represents any other updates: a larger
number is a later version. This numbering scheme means the imaginary version 69.420.5678 is newer (and possibly
more stable or feature-rich) than 69.420.1234.

## How to Build Garbage OS

Since Garbage OS builds a cross compilation toolchains from source, Garbage OS requires the following tools installed
and on the path to build properly:

1. Git (if you cloned this repository, you should already have this)
2. make (or some other tool capable of running the makefile)
3. a C/C++ compiler capable of building GCC and GNU Binutils (included as packages together in most linux distributions)
4. Bison
5. Flex
6. GMP (libgmp)
7. MPC (libmpc)
8. MPFR (libmpfr)
9. Texinfo

At some later point, Garbage OS may build native copies from source for each of GCC's dependencies, reducing the
build-prerequisites to simply git, make, and a C/C++ compiler.

To initialize the internal repositories, cloning GCC and binutils, run `make init`. Be warned: GCC and binutils are
very large repositories, `make init` will take some time. You can run `make init` on multiple threads, however, since
the makefile is only cloning two repositories, you will only benefit from allowing two jobs to run in make.[^1]

To build all cross toolchains, run `make build_toolchains`. This command will configure and build all necessary
versions of the cross compiler. First building all binutils versions then building all GCC versions. The makefile
processes architectures in alphabetical order (so riscv32 is between i386 and x86_64). The build_toolchains command
does not speed up with multiple threads, however, it does run the make process for the toolchains with multiple 
threads (currently programmed at 16).

[^1]: `make superclean` removes the dependencies folder. You should never need this command but it's there.