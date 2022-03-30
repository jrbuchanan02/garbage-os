/**
 * @file halt.c
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Implements the halt function.
 * @version 1
 * @date 2022-03-30
 * 
 * @copyright Copyright (C) 2022. Intellectual property of the author(s) listed above.
 * 
 */

#include <kernel/machine/halt.h>

void halt()
{
#if defined(__A64__)
    __asm__("wfi");
#elif defined(__X64__) || defined(__X32__)
    __asm__("cli");
    __asm__("hlt");
#else
    while(1){}
#endif
}