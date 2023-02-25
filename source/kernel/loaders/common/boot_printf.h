/**
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Definitions (then declarations) of common functions for each of the
 * loaders.
 * @version 1
 * @date 2023-02-20
 * 
 * @copyright Copyright (C) 2023. Intellecutal property of the author(s) listed
 * above. 
*/
#ifndef KERNEL_LOADERS_COMMON_BOOT_PRINTF_H
#define KERNEL_LOADERS_COMMON_BOOT_PRINTF_H

#include <stdint.h>


typedef void (*some_putc)(char);

void puts ( some_putc, char const *const restrict str );
void putd ( some_putc, intmax_t );
void puto ( some_putc, uintmax_t );
void putx ( some_putc, uintmax_t, char );
void putu ( some_putc, uintmax_t );


void bprintf(some_putc, char const *const restrict, ...);

#endif // ifndef KERNEL_LOADERS_COMMON_BOOT_PRINTF_H