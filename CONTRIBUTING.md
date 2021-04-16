# Contributions

As a general rule, feel free to contribute. Find an issue that you feel capable of addressing under issues, and, well, address it.

## What is there to do? 

You can find the list of bugs [here] and the list of general to-do's [here]. Anything not yet listed on those links will be at the issues page [here](https://github.com/Natara1939344/garbage-os/issues). As long as you follow the [Code Conventions](#code-conventions), your changes may be accepted.

## Code Conventions

### Generally Speaking...

This section will sound overly pedantic, but, that's because this entire file is overly pedantic.

 - Make your code compile without warnings - from personal experience, if you have a warning in your code, it's either a bug or puppy-killing hack.
 - Keep your code clean - from personal experience, clean code runs faster and is easier to debug. Even if the enhanced speed is a fallacy, the ease of debugging much better suits the exigence of an operating system (to be a stable platform on which code can run without worrying about the intricacies of each individual machine). 
 - If it's not your code, don't copyright it. 


### C
C files are split into sources and into headers. Here are the naming conventions that GOS follows:

1. use typedef for structs, enums, and unions (when they are named of course). EX:
```
typedef struct {
    int64_t long_x;
    int32_t int_y;
} foo_t;


typedef enum {
    ENUM_CONDITION_1,
    ENUM_CONDITION_2,
    // -- SNIP --
    ENUM_CONDITION_n,
} some_enum_t;
```
2. keep all names snake_case. One can see the use of snake case in the typedef example
3. Ensure that all typedefs end with the `_t` suffix, as shown in the typedef example. 
4. Use typedefs for defining callbacks. EX:
```
typedef void (*some_callback_t)();
```
5. If your file should be part of the operating system, but should not *load with the kernel*, it belongs in gos/code/impl/os/ (for source files) or gos/code/intf/os (for header files). Ensure that it has a main function. EX: (from gc.c and gc.h from 2021-04-12). 
FILE: gc.h
```
/**
 * @file gc.h
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com) (github: @natara1939344)
 * @brief important header stuff for gc.c
 * @version 0.1
 * @date 2021-04-12
 * 
 * @copyright Copyright (C) 2021 under the LGPL version 2.1
 * 
 */
#ifndef GC_H
#define GC_H 1

#include "stdlib.h"
#include "stdint.h"
#include "../../intf/crt/error_codes.h"
#ifdef __cplusplus
extern "C" {
#endif


void gc_monitor();

int main(void);

#ifdef __cplusplus
}
#endif
#endif
```
FILE: gc.c
```
/**
 * @file gc.c
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com) (github: @natara1939344)
 * @brief This file will house the primary section of the garbage collector.
 * @version 0.1
 * @date 2021-04-11
 * 
 * @copyright Copyright (C) 2021 under the LGPL version 2.1
 * 
 */

#include "../../intf/os/gc.h"

int main(void) {
    return ERROR_CODE_NOT_IMPLEMENTED;
}
```
6. If your code implements something from the C Standard Library: 
    + Ensure that it complies with the *latest ISO-C Standard*
    + The file goes in gos/code/impl/crt/ if it is a source file, gos/code/intf/crt if it is a header file.
    + This code does not need a main function, unless the ISO-C Standard specifically states it must (I don't think it will, but I haven't read the standard)
7. If your code is part of the kernel and **loads with the initial ramdisk**, then it goes in gos/code/impl/kernel/. It will have an identically named header file (with a .h extension) in gos/code/intf/kernel. The kernel files will *only* include the header file of the same name. In other words, the source file is purely the implementation of all declared functions. 
#### How to declare your header.
For a hypothetical header, foo.h, this will be its code, replace First with your first name, Last with your last name, email@domain.com with your email, and @firstlast1234 with your github handle. Excluding the BOOTBOOT loader and other areas not written by GOS contributors, the operating system is copyright entirely under the author's (or authors') names with the LGPL version 2.1. The version of each file is independent and starts at 0.1.

```
/**
 * @file foo.h
 * @author First Last (email@domain.com) (github: @firstlast1234)
 * @brief Now is the time for all good men
 * @version x.y
 * @date yyyy-mm-dd
 * 
 * @copyright Copyright (C) yyyy under the LGPL version 2.1
 */
#ifndef FOO_H
#define FOO_H 1

// includes go here

#ifdef __cplusplus
extern "C" {
#endif

// code declarations, typedefs, and other header-worthy stuff goes here

#ifdef __cplusplus
}
#endif
#endif
```
### C++
C++ files will have *slightly* different conventions compared to C files.
#### File Extensions
##### Header Files
Unless the file will be included by a C source file, the header file extension for all C++ source files will be .h++. If the file needs to be included by a C source file, the header file will be a .h file. 
##### Source Files
Without exception, all C++ files used in GOS will have the .c++ extension.
#### Code Formatting
While at the time of writing there are no C++ source files in GOS, here are the rules:
1. Use PascalCase for objects (classes, structs, unions, enums, namespaces, concepts, etc.) that will *not* interface with C or Assembly (in other words, not marked `extern "C"`)
2. Use camelCase for variables and methods that will *not* interface with C or Assembly (in other words, not marked `extern "C"`)
3. Use snake_case for anything that will interface with C or Assembly (in other words, marked `extern "C"`)
4. Keep lines down to 80 columns.
5. Use inline namespaces to separate different versions of an API. The current version should be kept inlined, previous and experimental versions kept outlined. These experimental versions should be named `Version_X_Y`, where X and Y are the file's version for that particular namespace's code. 
6. pImpl will be used for all classes declared in .h++ files. The structure name will be `Implementation` and the `unique_ptr` will be `pImplementation`. 
##### When outlining your code
As one may infer from monstrosities such as `std::unique_ptr<std::map<std::unique_ptr<std::function<void (std::tuple<unsigned long long int, unsigned long long int, const std::wstring** const const>)>>, std::unique_ptr<std::function<void (std::tuple<unsigned long long int, unsigned long long int, const std::wstring** const const>)>>>>`, limiting lines to 80 columns will result in outlining your functions. Here are the rules for when to insert a line break and when to not insert a line break:
1. The curly brackets of compound statements will remain on their own line. EX:
```
void foo()
{

}

class Bar
{

};

enum List
{

};
```
2. When templates are outlined, the less than sign that begins them remains with the template name. The final greater than sign that ends the function name must be on the same line as the declaration. Items within the template are indented. EX:
```
std::unique_ptr<
    std::map<
        std::function<void (
            std::tuple<
                unsigned long long int,
                unsigned long long int,
                const std::wstring** const const
            >
        )>,
        std::function<void (
            std::tuple<
                unsigned long long int,
                unsigned long long int,
                const std::wstring** const const
            >
        )>
    >
> functionPointerMapPointer;
```
3. Functions follow the same rules as templates when outlined. Their return type may be outlined as well. Namespaces may have line breaks (without indents) after the two colons. EX:
```
std::map<
    std::vector<
        std::array<
            std::list<
                const volatile 
                unsigned long long int*** 
                const volatile const volatile const volatile
            >,
            50
        >
    >,
    extern "C" struct some_foo_structure_t
> getFooFromBar(
    const volatile Location::
    Position::ReallyLongNamespaceName::
    WhyAmIAddingYetMoreNamespaceMembers::Bar*& const volatile someBar
) noexcept
{
    return someBar->foos;
}
```
#### Header File syntax
Like with the C example, here is an example foo.h++ file and an example foo.c++ file.

FILE: foo.h++
```
/**
 * @file foo.h++
 * @author First Last (email@domain.com) (github: @firstlast1234)
 * @brief Now is the time for all good men
 * @version x.y
 * @date yyyy-mm-dd
 * 
 * @copyright Copyright (C) yyyy under the LGPL version 2.1
 */
#pragma once
#include <memory>

inline namespace Version_X_Y
{   
    /**
     * @class Foo
     * @author First Last (email@domain.com) (github: @firstlast1234)
     * @brief Implements abcd
     * @version x.y
     * @date yyyy-mm-dd
     * 
     */
    class Foo
    {
    public:
        Foo() noexcept;
        Foo(const Foo&) noexcept;
        Foo(const Foo&&) noexcept;
        virtual ~Foo() noexcept;
        
        /**
         * @brief Gives the answer
         * 
         * @param x - the question you will ask
         * @return The secret to life, the unvierse, and everything
         */
        int foo(const int x) const noexcept;
        /**
         * @brief takes the sum of a four dimensional, jagged matrix
         * @param fourDimensionalMatrix - four dimensional matrix to take the
         * sum of
         * @param dimensionOne length of the first dimension - how many rows 
         * long is the matrix?
         * @param dimensionTwos length of the second dimension - how many 
         * columns per row in the matrix?
         * @param dimensionThrees length of the third dimension - how many 
         * elements deep is each column?
         * @param dimensionFours length of the fourth dimension - how many 
         * items exist in each member of the third dimension?
         * @return the sum
         */
        int bar(
            const int**** fourDimensionalMatrix, 
            const size_t dimensionOne, 
            const size_t* dimensionTwos, 
            const size_t** dimensionThrees, 
            const size_t*** dimensionFours
        ) const noexcept;
    private:
        struct Implementation;
        std::unique_ptr<Implementation> pImplementation;
    };
}

namespace Version_A_B
{
    /**
     * @class Foo
     * @author First Last (email@domain.com) (github: @firstlast1234)
     * @brief Implements abcd
     * @version a.b
     * @date yyyy-mm-dd
     * 
     */
    class Foo
    {
    public:
        Foo() noexcept;
        Foo(const Foo&) noexcept;
        Foo(const Foo&&) noexcept;
        virtual ~Foo() noexcept;

        int baz(const int) const noexcept;
        int foo(const int) const noexcept;
    private:
        struct Implementation;
        std::unique_ptr<Implementation> pImplementation;
    };
}
```

FILE: foo.c++
```
/**
 * @file foo.c++
 * @author First Last (email@domain.com) (github: @firstlast1234)
 * @brief Now is the time for all good men
 * @version x.y
 * @date yyyy-mm-dd
 * 
 * @copyright Copyright (C) yyyy under the LGPL version 2.1
 */

#include <foo.h++> // note that this one line will likely change!

struct Version_X_Y::Foo::Implementation
{
    int some_x;
    unsigned int someOtherVariable;

    int doBar(
        const int**** matrix, 
        const size_t y, 
        const size_t* x, 
        const size_t** z, 
        const size_t*** w
    ) const noexcept
    {
        int sum = 0;
        for (int i = 0; i < y; i++) {
            for (int j = 0; j < x[i]; j++) {
                for (int k = 0; k < z[i][j]; k++) {
                    for (int m = 0; m < w[i][j][k]; m++) {
                        sum += matrix[i][j][k][m];
                    }
                }
            }
        }
        return sum;
    }

    int doFoo(const int x) const noexcept 
    {
        if (x) {
            return 42;
        }
        else {
            return doFoo(x++);
        }
    }
};

struct Version_A_B::Foo::Implementation
{
    int doBaz(const int z) {
        if (!z) {
            return ~42;
        }
        else {
            return doBax(z++);
        }
    }

    int doFoo(const int x) {
        if (x) {
            return 42;
        }
        else {
            return doFoo(x++);
        }
    }
};

struct Version_X_Y::Foo::Foo() noexcept
: pImplementation(new Implementation())
{
    // intentionally blank
}

/* -- snip -- */

```

Note that the snip above, a lot like in the Rust documentation / tutorials, means that there is more to the hypothetical source file. 

### D
Follow the D code conventions. When interfacing with C++ code, use `extern(C++)` (as opposed to marking both parts of the interface as `extern(C)`, or, for the C++ side, `extern "C"`)
### Assembly / Assembler
#### File Extensions
Since, whenever assembly is required, GOS will use the GNU assembler, there are some features within it that we can use - hence the clarifications here.
1. Include files will use the C's .h files. Assembly files using the GNU defines to cause different compilation events based on whether a C, C++, or Assembly source includes the file.
2. Source files will have a .S extension (capital S, in case if the font does not make that clear). 
3. We will use AT&T Syntax (the default) within the GNU assembler. 
#### Code Conventions
1. Keep your code clean. While this statement applies to the other programming languages out there, when GOS uses Assembly, it will *not be for performance*, it will be for *implementing techniques impossible in higher level programming languages*. In other words, remain especially neat with your code while writing in Assembly - as it needs to remain human readable in a generally more difficult to read language. 
2. record which registers are preserved, clobbered, and any known register states at the entry of your function. Doing this *should* help in debugging the code. 