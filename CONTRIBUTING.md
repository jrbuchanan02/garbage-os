# Contributing to Garbage OS
Some rules to remember about how to contribute to Garbage OS:
1. Write down what you're doing. GOS is primarily for people to learn about Operating Systems.
2. Feel free to write humorous (but professional) comments within your code. GOS
does not take itself too seriously.
3. Only write it once. Share code whenever possible.
4. Note the security implications of your code. 
5. Limit the usage of global variables.

## I want to contribute. How do I do that?
1. Look under issues and find one that looks interesting to you
2. Make a branch and clone
3. Fix the issue and make a pull request

## The Syle Guide

Garbage OS intends to keep a specific style of code and thsu follows these rules.

### Clarifications on the style guide

**High Level Language**: Any programming language that does not individually list out
instructions by name for a CPU. Examples of high level programming languages include
C, C++, FORTRAN, Java, Python, and MATLAB. Garbage OS primarily utilizes C++.

**Assembly Language**: A computer program written by individually listing instructions
for a CPU to execute. Assembly Language programs are typically limited to one instruction
set architecture. Garbage OS currently utilizes x86 and RISC-V assembly but plans to 
include and support other instruction sets (namely ARM).

### The Actual Style Guide

1. All code written in a High Level Language shall strictly conform to that High Level Language's
   standards. For example, all C translation units used by Garbage OS shall satisfy the requirements
   of a strictly conforming C program according to the corresponding revision of ISO 9899 (the ISO C 
   standard). This style requirement shall not apply to compiler extensions necessary to export functions
   in a shared library or to specify a specifc structure layout in a program, if such a layout is 
   necessary to interact with an external library.
   - This requirement means that C and C++ programs are prohibited from using inline assembly.
   - This requirement means that C code used by Garbage OS must follow ISO C implementation limits,
     even the less logical ones such as the 15-header-deep include file nesting depth limit and the
     cryptic 31-character-but-sometimes-characters-count-for-six-or-ten-characters external identifier
     length limit.
   - This requirement does not apply to compiler specific features necessary to specify an ABI, export
     a function, or pack structures.
2. All functions in Garbage OS shall have a cyclomatic complexity of 5 or less. Switch statements only
   count towards cyclomatic complexity if a fallthrough exists or if any case label contains a selection
   statement.
   - For the uninitiated, this means less than 5 independent paths through the program
   - A quick and dirty way to calculate cyclomatic complexity for a C / C++ program is to count the instances
     of the keywords `if`, `while`, `goto`, `catch` and `for` within a function. Since this requirement only occasionally
     counts switch-statements, count all cases in a switch statement if any of the labels does not end in the
     break keyword or if any of the labels contains `if`, `while`, `goto`, `for`, `do` `catch`, or `switch`.
   - Assembly languages count conditional branch instructions, dependent sequences of conditional instructions, and
     options for indirect branches for their paths.[^2] If an assembly language code implements a jump-table or similar
     and fails to perform a bounds check, then the cyclomatic complexity for that indirect jump includes all theoretical
     targets for the jump. e.g., blindly executing `call [rax]` has a cyclomatic complexity of 2 to the power of 64.
3. All functions in Garbage OS shall have a length below 301 logical source lines of code. Here, a logical source
   line of code is the total of non-empty, non-comment statements within a function.
   - This requirement means the longest function possible is still infinitely long but it can contain a total of 300 
     statements or fewer.
   - If the statements within the parenthetical block of a for-loop are nontrivial (i.e., not just initializing a variable,
     a boolean condition, and a statement with a single side effect), then the nontrivial statements in that for-loop count as 
     logical source lines of code.[^1]
4. All statements in Garbage OS written in high-level languages, excluding function calls, are limited to two side-effects.
   - Here, a side effect includes an assignment within an expression. `i = j++` has two side effects.
5. High level language code shall not see instruction-set specific differences - those differences shall be handled by assembly
   code.


[^1]: This paragraph means that both of the following implementations of strcpy have the length in logical source lines of code
since, in the second implementation, the final statement of the for-loops parenthetical has more than one side-effect. The
second implementation violates the style guide, however.
```C
char *strcpy(char *dst, char *src) {
    for(size_t i = 0; src[i]; i++) dst[i] = src[i];
}
```
```C
char *strcpy(char *dst, char *src) {
    for(char *dst_pos = dst; src; *dst_pos = *src, src++, dst_pos++);
}
```

[^2]: For example, the following two implementations of a maximum function in x86 assembly have a cyclomatic complexity of 2.:
```ASM
max:
  mov eax, ecx
  cmp eax, edx
  jae .skip
  mov eax, edx
.skip:
  ret
```
and
```ASM
max:
  mov eax, ecx
  cmp eax, edx
  cmovb eax, edx
  ret
```
