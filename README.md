# Garbage OS
Garbage OS is an open-source operating system that:
1. Does not take itself seriously
2. Is intended to possibly educate on the functionality of Operating Systems
3. Intends to support as many architectures as possible

Garbage OS is distributed under the LGPL version 2.1 with the hopes that
it will be useful. See the LICENSE file for more details.

## Roadmap

1. [ ] Create a basic Kernel (version 0.x)
    - [ ] Kernel version which spins the CPU indefinitely (0.0.x)
    - [ ] Kernel version which features preemptive multitasking and nothing else (0.1.x)
    - [ ] Kernel version with preemptive multitasking and memory protections but nothing else (0.2.x)
    - [ ] Kernel version with preemptive multitasking, memory protections, and the ability to load drivers (0.3.x)

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
| x64             | 0.0.0         | in development |
| RISC-V 32       | 0.0.0         | in development |
| RISC-V 64       | 0.0.0         | in development |

Support notes:

i386: Garbage OS assumes its kernel receives control in protected mode with ring 0 privileges.

x64: Garbage OS assumes its kernel receives control in long mode with ring 0 privileges.

RISCV-32: Garbage OS requires the RV32I base instruction set, control status registers (Zicsr),
and Sv32 (i.e., Supervisor mode, User mode, and 32-bit paging). Most RISC-V processors will run
the RISC-V 32-bit version of the kernel.

RISCV-64: Garbage OS requires the RV64I base instruction set, control status registers (Zicsr),
and Sv39 (i.e., Supervisor mode, User mode, and 39-bit paging). Most 64-bit RISC-V processors
will run the RISC-V 64-bit version of the kernel. 