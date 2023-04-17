# The Garbage OS Memory Map

Garbage OS has a loose map of memory. Starting at address zero and placed one
after the other, Garbage OS places the loader code, loader data, kernel code, 
kernel data, general use memory, firmware code, firmware data, memory mapped
devices, and unusable memory. Unlike most conventions, Garbage OS treats address
zero as the top of the address space (it looks at the memory map upside-down, so
if the stack normalyl "grows down" Garbage OS views it as "growing up"). If we
have 4096 (0x1000) bytes of each type of memory, the map according to Garbage OS
would look like so:
| start | end | type |
| ---- | ---- | ---- |
| `0x00000000` | `0x00000FFF` | loader code |
| `0x00001000` | `0x00001FFF` | loader data |
| `0x00002000` | `0x00002FFF` | kernel code |
| `0x00003000` | `0x00003FFF` | kernel data |
| `0x00004000` | `0x00004FFF` | general purpose RAM |
| `0x00005000` | `0x00005FFF` | firmware code |
| `0x00006000` | `0x00006FFF` | firmware data |
| `0x00007000` | `0x00007FFF` | memory mapped devices |
| `0x00008000` | `0x00008FFF` | unused memory |

Arranged the right way up, the map looks like this:
| end | start | type |
| ---- | ---- | ---- |
| `0x00008000` | `0x00008FFF` | unused memory |
| `0x00007000` | `0x00007FFF` | memory mapped devices |
| `0x00006000` | `0x00006FFF` | firmware data |
| `0x00005000` | `0x00005FFF` | firmware code |
| `0x00004000` | `0x00004FFF` | general purpose RAM |
| `0x00003000` | `0x00003FFF` | kernel data |
| `0x00002000` | `0x00002FFF` | kernel code |
| `0x00001000` | `0x00001FFF` | loader data |
| `0x00000000` | `0x00000FFF` | loader code |

Garbage OS is not, however, allowed to move some regions of memory. One example
is a region of memory used by [BIOS](./bios.md)-based systems called the EBDA. 
The EDBA is normally used by the firmware in a processor-mode called System 
Management Mode. We cannot prevent the firmware from entering SMM and if we 
make any attempt to move the EDBA or reclaim it, we will likely crash the machine.
Garbage OS solves this issue in the following ways:
1. If the memory happens to fall in the unused region, mark the memory as unused.
2. If the memory happens to fall in or be close to the kernel data region or
be placed "above" (at a lower-valued address) the kernel code, mark it as "owned"
by the kernel, but also not writeable and not executible.
3. If the memory does not fit well into the address space, mark it as owned by
the kernel, unwriteable, and not executible.

In the BIOS environment, the environment-specific loader uses the following memory map.

| start | end | type |
| -- | -- | -- |
| `0x00000` | `0x003FF` | Interrupt Vector Table (CPU defines it here) |
| `0x00400` | `0x004FF` | BIOS Data Area (BIOS defines it here) |
| `0x00500` | `0x07BFE` | Stack |
| `0x07C00` | `0x07DFF` | Loader's First Stage (BIOS defines it here) |
| `0x07E00` | `0x7FFFF` | Later loader stages |
| `0x80000` | `0x9FFFF` | Extended BIOS Data Area (DO NOT WRITE) |
| `0xA0000` | `0xFFFFF` | MMIO / ROM (BIOS defines it here) |
| `0x100000` | beyond | common-loader code goes here. |