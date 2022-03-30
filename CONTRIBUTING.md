# Contributing to Garbage OS
Some rules to remember about how to contribute to Garbage OS:
1. Write down what you're doing. GOS is primarily for people to learn about Operating Systems.
2. Feel free to write humorous (but professional) comments within your code. 
3. If you had to write it twice, there's possibly a more concise way to write it.

## I want to contribute. How do I do that?
1. Look under issues and find one that looks interesting to you
2. Make a branch and clone
3. Fix the issue and make a pull request

## Some notes on contributing:
Keep in mind (my) second law of computing[^1]:
> The effort it takes to change something scales exponentially with the amount of times you wrote it.

In other words, try to keep items that should be identical aliased to the same value, a good example of
this is the efi.h file [here](./source/kernel/loaders/efi/efi.h) which aliases the beginning of
each structure in the (U)EFI standard with the same attributes -- even same beginning
and ending. If the attributes of each of the countless UEFI structures need to be changed, it
is changing only one line in a 1000 line file (versus many lines in that same file).

[^1]: The "first" law is: every digital logic circuit is writable as a two-stage circuit; the "third" law
is "cost decreases with the decrease in chip count". The three come from general observations
on computers (I am a Computer Engineering major), but have no "real" proof.
