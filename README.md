# Garbage OS
Garbage OS, or GOS for short, is a work-in-progress proof-of-concept Operating System that comes with a built-in garbage collector.

GOS is not intended to replace any of the existing operating systems, however, it is meant to see what performance boost, if any, arises from consolodating Garbage-Collection 
services to the max - something achieved with GOS as there is only one Garbage Collector. 

GOS is also a BOOTBOOT compliant Operating System, you can find the source code for BOOTBOOT here: https://gitlab.com/bztsrc/bootboot/-/tree/master.
What complying with BOOTBOOT means for an end-user is that one can directly boot GOS or chainload it from GRUB. GOS is set up to use BOOTBOOT as an efi loader (meaning it works with UEFI).

__**I make no warranty with Garbage OS, nor will I ever.**__
That being said, if you want to try it out or contribute something, feel free to do so.

## Documentation
Almost everything in Garbage OS is documented [here](./docs/index.html)

## BOOTBOOT loader, standard, and image creator.
You can find the README.md for mkbootimg, part of the BOOTBOOT repository and the program that assembles the final disk image for GOS, [here](./mkbootimg/README.md)

## Roadmap
 - Boot in a stable manner that accounts for the unique differences in every computer. (✔)
   + Initialize memory, i.e., the page table around MMIO devices and "dead memory" - handled by BOOTBOOT
   + Setup IDT (and test)
 - Implement a Garbage Collector service, kind of obvious since that's the entire point of this project.
 - Implement preemptive multitasking - this algorithm absolutely needs to be **superfast**.
   + After around 20 min research on OSDev wiki, I am leaning towards implementing a priority based Round Robin.
   + Keep track of processes that are currently running (although, we have to do that for GC).
 - Implement some Open Source Quality of Life improvements
   + ext-based filesystem. Since there is already an extremely popular implementation on GitHub, I might check how it works - implementing it myself
   + elf executable file format (or a modification of that). ELF is simple, fast, and well documented.
 - Implement GUI (BOOTBOOT sets up graphical framebuffer, so GUI becomes a *task*).
 - See if and where it goes from here.

## Contributions
See CONTRIBUTING.md for the latest info.

## Security flaws.
For the latest info, see SECURITY.md

General disclaimer, repeated once more: I make no warranty on this software, nor do I promise that it will do anything.

If I am doing anything wrong, please tell me and I will fix it.

As of the current version of Garbage OS, there is likely not any information to accidently make vulnerable via a security bug. However, if I have messed up a security mechanism beyond what any contributor seems capable of, it is probably a very critical error and needs to be fixed ASAP.

### Version Support

Here is a neat little placeholder-table.

| version number | support state | date state last changed |
| -------------- | ------------- | ----------------------- |
| all versions   |      N/A      |           N/A           |

