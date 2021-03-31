# Dlang-os
Dlang-OS, or DOS for short, is a work-in-progress proof-of-concept Operating System that comes with a built-in garbage collector.

DOS is not intended to replace any of the existing operating systems, however, it is meant to see what performance boost, if any, arises from consolodating Garbage-Collection 
services to the max - something achieved with DOS as there is only one Garbage Collector. 

__**I make no warranty with Dlang-OS, nor will I ever.**__
That being said, if you want to try it out or contribute something, feel free to do so.


## Roadmap
 - Boot in a stable manner that accounts for the unique differences in every computer. 
   + Initialize memory, i.e., the page table around MMIO devices and "dead memory"
   + Setup IDT (and test)
   + run all static constructors within the operating system - D has these, so it must be done
 - Implement preemptive multitasking - this algorithm absolutely needs to be **superfast**.
   + After around 20 min research on OSDev wiki, I am leaning towards implementing a priority based Round Robin.
   + Keep track of processes that are currently running (although, we have to do that for GC).
 - Implement some Open Source Quality of Life improvements
   + ext-based filesystem. Since there is already an extremely popular implementation on GitHub, I might check how it works - implementing it myself
   + elf executable file format (or a modification of that). ELF is simple, fast, and well documented. The only potential reason for a modification on elf is to indicate whether the language expects a D runtime, however, the D runtime automatically starts up in existing ELF binaries, so that is probably unnecessary.
 - See if and where it goes from here.

## Contributions
See CONTRIBUTING.md for the latest info.

## Security flaws.
For the latest info, see SECURITY.md

General disclaimer, repeated once more: I make no warranty on this software, nor do I promise that it will do anything.

If I am doing anything wrong, please tell me and I will fix it.

As of the current version of Dlang-OS, there is likely not any information to accidently make vulnerable via a security bug. However, if I have messed up a security mechanism beyond what any contributor seems capable of, it is probably a very critical error and needs to be fixed ASAP.

### Version Support

Here is a neat little placeholder-table.

| version number | support state | date state last changed |
| -------------- | ------------- | ----------------------- |
| all versions   |      N/A      |           N/A           |

