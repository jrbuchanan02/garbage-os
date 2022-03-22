# src/bootload/8086/old/entry
Unfortunately, GCC cannot target the 16-bit x86 platforms, certainly not as a 
freestanding implementation and certainly not from a host other than Windows 
3.1. This means that we have to make this code entirely in assembly. I am well
versed enough in x86-assembly to possibly pull off such a feat, but it means 
maintaining two identical versions of every function in Garbage OS!