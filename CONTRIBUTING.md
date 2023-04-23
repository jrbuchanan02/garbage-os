# Contributing to Garbage OS

Before continuing, Garbage OS's code, with rare exception, is written exclusively
in the C++ programming language and complies with C++'s Standard 20. That is, the
following things are slightly different from classic C++. All cross platform code
is required to be written in C++ and all performance-critical code should be 
written in C++[^2].

1. The `register` keyword was deprecated in C++17 and will now issue a warning. GCC
still accepts the register keyword, however, my understanding is that the compiler
is now free do decide whether a value is actually used as a register on the processor.
2. `throw()` has been entirely replaced with `noexcept`. Many functions in Garbage
OS cannot throw an exception anyhow (since there is nothing to handle an exception
being thrown when there is no standard library).
3. Concepts! `concept`, `requires` etc. (this is why C++20 was chosen) C++ 20 
introduces a feature to require specific behavior from template variables. The 
best part of concepts, however, is that when a template substitution fails, 
the compiler simply states that it does not satisfy the concept and why 
(instead of explaining why it can't make a new template instantiation and then 
going through every existing instantiation and why it cannot use that one 
instead, there's a reason why I manually set my IDE's console scrollback to more
than 9000 lines).
4. Functions can be marked `constexpr`, meaning the compiler is free to evaluate
them during compile-time if enough information exists and that the function is 
implied to have a result that does not depend on the external state of the 
program. Plain-old-data structures and other records with fully-constexpr 
constructors and destructors can be freely used in a `constexpr` context.
5. The apostrophe works also as a delimiter in long numeric literals, instead of
having to write out "1000000000" for one billion, we are now able to write out
"1'000'000'000" for one billion, which makes the numeric literal much more readable.
6. User defined literals, we can define our own literal-suffixes. For example, 
Garbage OS might define a literal called `_pages` that allows us to specify say
3 pages in terms of bytes by writing `3_pages` as opposed to 3 * 4096 or 12288.

Some of Garbage OS's code is also written in the assembly language. In the case
of the various forms of x86 assembly, Garbage OS uses NASM. In other cases,
Garbage OS may use other fields. Garbage OS requires similarly defensive programming
practices in assembly. However, certain "extreme" software environments may make
defensive programming impossible. Since NASM is not case sensitive, here are
some conventions on casing:
1. All instructons, register names, labels, etc. are lowercase
2. All macros, pseudo-instructions, etc. are UPPERCASE

Garbage OS also uses Python scripts to generate certain machine-generated files.
These python scripts are run individually with the guarantee that they are run
before the file(s) that need(s) the result gets built. These python scripts must
output files that clearly indicate their origin, reside in the directory build/generated
(and any additional sub-directories of generated, as appropriate), and be used 
to output code that is easy to define. In other words, it's a script that outputs
a file that compiled code will use. Nothing else.

For example, this script might output a header file for test cases. Alternatively,
this script might output assembly macros which allow a certain routine to exist
in assembly but remain cross-platform.

## Code Format

Garbage OS has a required code format. If you contribute to Garbage OS you are
required to write code that follows this code format.

1. there is a hard limit of 100 columns in a line.
2. all names are `snake_case` with the exception of macros which are `SNAKE_CASE`
(the macro names are capitalized.)
3. all indents are spaces. Preprocessed items (`#include`, `#define`, etc.) are
indented 2 spaces before the `#` if they reside within a `#if` block and are 
otherwise flush with the beginning of the line. All other indents are 4 spaces
per indent.
4. descriptive names are required. Acronyms, with an exception, must be written
out. The exception to the acronyms rule is if the acronym provides its own 
context, such as `guid`, `uuid`, and `atm`.
5. Multi-line `#define`s have right-aligned backslashes. The backslash occurs 
at the column limit.
6. Visiblity modifiers are indented with the record containing them (the p in 
`public:` is in the same column in the c in `class` that the `public` sits in.)
7. All enums are enum-class.
8. All structs are POD structs (they only contain variables, no methods).
9. All templates use concepts unless there is no semantic restriction on what
type can reside in the template. That is, the function `add(lhs, rhs)` must 
use a concept to enforce that `lhs` and `rhs` can, in fact, support addition
with one another and that the result will make sense. However, the template
`using callback = result (*)(args...)` should not specify a concept unless the
writer feels the need to specify that no member of `args` can be `void`.
10. Unless defined by an external standard (eg, ACPI, EFI), all content coupling
is banned. That is, you are not allowed to utilize a numeric offset into a data
structure without some assurance that you have the correct numeric offset.[^1]
11. All warnings are errors.
12. Other code formatting is followed automatically by the clang-format file,
however, there is one exception. Clang-format will on occaision not cause a line
break to preserve the assignment operator alignment. Assignment operators must
be aligned if they are sequential (no blank lines or only single-line comments
separating them). These separations can only be based on a logical distinction
between the assignments.

## How to Speak To the Chair-Computer Interface (the "Human")

Garbage OS works to be welcoming and accepting to all. This is reflected in the
code of conduct and in the general coding style. Additionally, any messages to
the person at the computer should also be accepting to all, even those who aren't
well-versed in computers.

### Error Messages

This section describes how to word an error message to the user.

1. Tell the user in precise, simple, and non-patronizing terms what went wrong. 
For example, instead of "Aborting: Checksum Mismatch", say something like, 
"Garbage OS found that the SSDT on your computer did not add up. This table is
required for Garbage OS to use your machine's hardware properly. So, Garbage OS
cannot continue the boot process."

2. Indicate what the problem is, do not accuse any party (software, hardware,
or user) of doing something wrong. For example, "Garbage OS was told that the 
floppy drive got swapped since the last time it read the drive" instead of 
"Garbage OS detected a fault in your floppy drive" or "Please do not remove
the Floppy Disk while it is being accessed". Error messages should not assign 
fault, simply describe what indicated the error-condition. 

3. Errors should optimally be 80 characters or less. This is simply a recommendation
as 80 character long messages reduces the risk of a line-break in the message
no-matter the system. If you cannot describe the error well in less than 81 
characters, do the best you can without writing an essay.

### Debugging Messages

This section describes how to word debugging messages to the person at the 
debugger. We can assume a more firm grasp of programming concepts in this 
debugging information, however, a precise and concise description of the information
remains of upmost importance.

1. For debugging messages that note an unexpected value, obviously, indicate 
what the value was and why it was unexpected. A good example would be in the 
ACPI standard's "RSDP" table, which never had a revision 1 (skipping straight
from zero to two). While a revision 1 table should be a valid revision 0 table,
it should still trigger a debugging message like "RSDP was revision 1, not 0 or >2". 



## Code Style

Garbage OS requires the practice of defensive programming. Here's what that
means:

1. All inputs to a code region, outputs from a code region, and inputs into 
another code region are checked before calling the function. If the inputs are 
invalid but trivially fixable, the inputs are fixed. If the inputs are invalid 
and not trivially fixable, the code treats the invalid inputs as an error.
2. All cases are considered possible code paths. For example:
```C++
if ( foo == true )
{
    //  do something
} else if ( foo == false )
{
    //  do something else
} else 
{
    //  error condition: the boolean variable was not true or false.
}
```
It should be noted, however, that in the above example if we needed foo to 
specifically be `true` or be `false`, then a simple `if-else` would do. This is
especially important for code that cannot fail.


## Error Handling

Garbage OS expects errors, here's how the code handles these errors:

1. In the loader environment: Loader errors automatically abort the loading 
process, that is, they are considered to reflect a "dangerous" firmware or
unsupported machine. On an abort, the loader outputs a detailed reason why the
abort happened (if possible), waits for three seconds (if possible), and either
requests control be given back to the firmware, resets the machine, or hangs the
processor (in order of preference). 
2. In the kernel environment: Once the loader hands control to the kernel, the
kernel is expected to do it's best to load. However, if the kernel detects a 
hardware failure (that does not immediately kill the kernel) or runs out of 
memory while loading, it will halt the processor.
3. In the API environment: API modules function more like conventional programs,
if these modules error out, they simply unload. The kernel then determines 
whether to ignore the error, restart the module, or panic. 
4. In the unification layer: the unification layer is not allowed to visibly
error out. It must handle and recover from all errors that present themselves.
5. Application Layer: if an application errors out, it closes and Garbage OS 
reclaims its allocated resources. This behavioris common to all modern operating
systems.

[^2]: You may write performance critical code in assembly -- if and only if you 
can provide proof that your code is faster and more maintainable than the 
compiler while complying with the same coding standards as the compiler. That
is, you must out-program the compiler (with the compiler not pulling any punches,
so to speak) before we use your code.

[^1]: content coupling refers to one part of a program interacting with another
part through knowledge of the exact format of the other part. For example, if 
I know that foo has the declaration:
```C++
class foo {
    int x;
    int y;
public:
    int get_x();
    int get_y();
};
```
and I want to modify x and y anyway, I could write the line:
```C++
((int *)foo)[0] = a;
((int *)foo)[1] = b;
```
and that would change the values of x and y to a and b. However, if I then make 
**ANY** change to the contents of foo, I would have to rewrite the code that 
modifies x and y. Moreover, if foo inherits from anything, the content-coupled
code has to know the exact size of all the types that foo inherits from!
In addition to creating less-readable code, content coupling automatically makes
code maintentance harder. Hence, the ban on it.