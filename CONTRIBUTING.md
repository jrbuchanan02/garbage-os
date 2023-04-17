# Contributing to Garbage OS

## Code Format

Garbage OS has a required code format. If you contribute to Garbage OS you are
required to write code that follows this code format.

1. there is a hard limit of 80 columns in a line.
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
structure without some assurance that you have the correct numeric offset.[1]
11. All warnings are errors.
12. Other code formatting is followed automatically by the clang-format file,
however, there is one exception. Clang-format will on occaision not cause a line
break to preserve the assignment operator alignment. Assignment operators must
be aligned if they are sequential (no blank lines or only single-line comments
separating them). These separations can only be based on a logical distinction
between the assignments.

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
specifically be `true` or be `false`, then a simple `if-else` would do.

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

[1] content coupling refers to one part of a program interacting with another
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