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
1. If the code you are writing is specific to a type of processor, (for example, code that halts the processor), make a header file with the function name and have an
assembly file named \[function name\]\[architecture name\].S for each architecture GOS compiles for. If there is an architecture that cannot perform whatever operation
you're implementing, make the assembly code return -1.
