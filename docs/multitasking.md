# How Operating Systems Manage Programs

If you have ever seen a program crash (who hasn't), you probably know that it
does not cause the entire computer (always) permanently freeze up, mostly. Even
in the cases where the computer *does* slow down or become less responsive, 
certain actions such as moving the mouse around or sometimes the ctrl-alt-del
key combination continue working. The reason this works, and why you should 
get particularly worried if the mouse *can't* move, is that the Operating System
sets a timer. When this timer runs out, the CPU gives control to the Operating
System and the OS chooses whether or not to change which task is currently 
running on the CPU. This system is called Preemptive Multitasking. 
Alternatively, the programs can voluntarily give CPU back to the operating 
system, a scheme called Cooperative Multitasking, but Cooperative Multitasking
is prone to issues regarding when programs crash. 

In fact, to be a modern operating system is to implement a preemptive 
multitasking system. However, the CPU (normally) can't do preemptive 
multitasking solely on its own. It needs an external timer.

So, from the hardware side, a timer interrupts ("interrupt" being both the 
formal term and a good way to describe what happens) the CPU every hundred
microseconds or so and automatically resets when it interrupts. 

From the operating system's perspective, we tell the hardware to call a special
function every hundred microseconds or so and in that function we save what the
previous task was doing and load the next task. Note that the next task could 
be the same as the previous task or also be a special task to do nothing.

From the application's perspective, we hog the whole CPU and other programs 
magically also get some playtime. Remember, executing our code means we have CPU
access. 

How does the operating system choose the next program? Well, there's lot's of 
ways to do that, but here's the method chosen for Garbage OS:

First, each currently running task is stored in a binary max-heap where the top
item has the largest difference between "now" and when we last gave it the CPU.
This structure also stores it's stack pointer and where to find its context, 
totaling a 24-byte structure on a 64-bit system and a 12-byte structure on a 
32-bit system. The structure itself does not store the difference, but the time
we last gave it the CPU, since we know the current time we can quite easily find
the difference. Note that we store the difference to allow for a concept called
"aging." Essentially, if we have two equal-priority tasks, no matter which 
started first, they should get the same CPU access, simply storing the start 
time means that an app started five seconds ago would be incorrectly given CPU
time over an app that started five hours ago. There is one caveat with the 
difference however: while it handles integer overflow, we can't have the current
time overflow more than once before all tasks in the heap are serviced.

Tasks have two additional properties: a priority, sort of like a multiplier, 
higher priority tasks will get serviced more often and in favor of lower 
priority tasks, and a cooldown. All tasks have the same cooldown, the cooldown
simply allows the CPU to be idle if it can be. The priority ensures that certain
performance-critical tasks (such as anything the kernel needs to do) are 
treated with more importance and background tasks (such as syncing files with an
online backup) are treated with less importance. 

The task with the highest key, taking into account the task priority, gets the
CPU. If all tasks are in their cooldowns, the CPU is kept idle until the timer
expires again. If the CPU is not idle, then we perform a decrease key operation
on the chosen task.

Binary heaps are well suited to this system: easy to form and manage, expandible
in O(1) ammortized time, increase key, insert, remove are all in O(lg(N)) time,
and it's taught in every computer science course so I likely don't have to 
explain it in the wiki. 


Here you can return to the main page: [Main Page Link](mainpage.md)