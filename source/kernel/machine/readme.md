# source/kernel/machine

Contains various functions that can only be implemented in either inline
assembly or something dangerous like a shell code. However, if possible, there
are default implementations of these functions written in c.


## Files inside:

- halt.c/h: contains the definition of a halt function that halts the processor
as much as software can halt the processor. That is, clearing interrupts and 
waiting for one (within a while loop). Many of these functions were written a 
good minute ago and need to be updated.