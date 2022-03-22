/**
 * @file main.h
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Main header file. Contains the declaration of the kernel entry point.
 * @version 1
 * @date 2022-03-21
 *
 * @copyright Copyright (C) 2022. Intellectual property of the author(s) listed above.
 *
 */
#ifndef SRC_KERNEL_ENTRY_MAIN_H
#define SRC_KERNEL_ENTRY_MAIN_H

#include <efi/efi.h>
#include <efi/efiapi.h>

/**
 * @brief The entry point for the entire operating system. A union between the
 * different architectures' setup routines. Literal poetry in motion.
 * 
 */
void kmain();

#endif // ifndef SRC_KERNEL_ENTRY_MAIN_H