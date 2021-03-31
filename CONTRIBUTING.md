# Contributions

As a general rule, feel free to contribute. Find an issue that you feel capable of addressing under issues, and, well, address it.

## Code Conventions

### D-lang

 - As a general rule, if your code violates the https://dlang.org conventions, you are killing puppies and making people sad.
 - In addition to compile-time errors, compile-time warnings (and, by extension, code that causes them) are heavily discouraged. The goal of an operating system is not to make "quick and dirty" code or "fast at all costs" code, but to make code that performs its job exactly as it should. 

### Assembler

#### When should I use the inline assember?

 - If your code *must* use assembly opcodes, use D's built-in inline assembler - if the version is, say, AARCH64 (which is not yet implemented), then you should also include code within a .s file in the appropriate location. 
 - If your code *requires an op-code not allowed in the inline-assembler*, place the function in an assembler file.
