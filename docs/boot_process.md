# How Computers Boot an Operating System

First, a quick bit of pedantics. What we normally call "BIOS" is actually called
firmware. "BIOS", as in, the Basic Input Output System that the IBM PC\AT and 
its clones provide when their CPU is in real mode, is a type of firmware... and
actually a really terrible, loosely defined, almost hostile-to-software firmware
at that. Most computers today (including your tablets, phones, and smartwatches)
use a different type of firmware called EFI (or sometimes UEFI).

That aside, the firmware loads some program (typically agreed to be the 
operating system loader) from the first valid place that has one. Different 
types of firmware have different methods of looking and loading the program, but
if we have built our boot disk correctly and our operating system is the first
operating system to show up, our loader will eventually get to load.

The loader then sets the system into a known state, finds the kernel, and gives
control to the kernel. The exact behavior here particularly varies from firmware
to firmware but there are common questions the loader answers: How much RAM is 
in the system? Where is the RAM? What devices are connected to the system? How
can I use these devices? Am I in the correct processor mode?

The kernel then starts a pre-emptive multitasking routine, this routine uses a 
combination of hardware (a timer) and software to automatically swtich which 
thing (task, thread, whatever) gets access to the cpu right now. This routine
is why you don't program "Hello, World" like this:
```C++

#include <iostream>
#include <cstddef>

using std::cout;

int main(int argc, char **argv)
{
    static char hello_world[] = "Hello, World!\n"
    static int chars_printed = 0;
    for(; chars_printed < sizeof(hello_world) / sizeof(char); chars_printed++)
    {
        cout << hello_world[i];
        return TASK_SWITCH_REQUESTED;
    }
    return 0;
}
```

Once the kernel has pre-emptive multitasking set up, the kernel then loads the
remaining layers. In Garbage OS, the kernel attempts to load all API modules, 
various parts of the API modules can decline to load. Then some part of the 
operating system runs all apps set to run at startup.

After Garbage OS runs the startup apps, the system is considered to be "idle,"
that is, finshed with the boot process.

Here you can return to the main page: [Main Page Link](mainpage.md)