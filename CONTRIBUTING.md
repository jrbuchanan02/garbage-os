# Contributions

## Code Format
### D
Follow the language's conventions. These are specified in the language's official website: https://dlang.org
### Assembly
Assembly should only be used in separate files where boot code resides. At the latest edit of this document, these files are 
src/boot/loader/header.s, src/boot/loader/start.s, src/boot/starter/main32.s, and src/boot/starter/main64.s. For other conditions
where direct, bare-metal access is required, use the inline assembler.

Regardless, these are the rules, as there are probably more dialects of assembler than languages

 - "Intel Syntax": operand destination, value. ex: add rax, rbx or add x1, x2
 - NASM dialect, if you don't nkow what that means (yet), you probably should not be attempting to edit the four aforementioned
 files. ^_^
## Where, when, and how to contribute
Here are some potential scenarios in which you may tell yourself these phrases

### "Yo! I found a bug / typo / security vulnurability in the operating system!"
1. look under issues, the issue is already documented, see "I want to help make DBOS better!"
2. If the issue is not already documented, make a new issue. If you know how to fix the issue, then go to "I want to help make DBOS better!"

### "I want to make DBOS better!"
1. look under issues, if you find an issue that you feel capable of fixing, begin fixing it via becoming a contributor
 (truth be told, I have no idea how to make contributions myself)
2. fix the issue
3. make a pull-request
4. watch your code become part of the operating system.
