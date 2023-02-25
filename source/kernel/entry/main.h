/**
 * @file main.h
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Contains enough information for the loaders to call kmain.
 * @version 1
 * @date 2022-03-23
 *
 * @copyright Copyright (C) 2022. Intellectual property of the author(s) listed
 * above.
 *
 */
#ifndef KERNEL_ENTRY_MAIN_H
#define KERNEL_ENTRY_MAIN_H

#include <common/hal/acpi.h>

struct _kmain_magic;

enum _known_loaders
{
    EFI,
    BIOS,
};


typedef struct _kmain_magic kmain_magic;
typedef enum _known_loaders known_loaders;

void kmain ( void *const mmap, kmain_magic *const magic );

struct _kmain_magic
{
    known_loaders loader;
    acpi_rsdt    *rsdt;
    acpi_xsdt    *xsdt;
};

#endif    //  ifndef KERNEL_ENTRY_MAIN_H