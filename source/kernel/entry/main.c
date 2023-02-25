/**
 * @file main.c
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Entry point for the Operating System
 * @version 1
 * @date 2022-03-23
 *
 * @copyright Copyright (C) 2022. Intellectual property of the author(s) listed
 * above.
 *
 */

#include <kernel/entry/main.h>
#include <kernel/machine/halt.h>

#ifndef INIT_ENTRIES_FOR_SDT
  #define INIT_ENTRIES_FOR_SDT( name )                                         \
    if ( name )                                                                \
    {                                                                          \
      name##_entries = name->entries;                                          \
      name##_exists  = 1;                                                      \
      name##_entry_count =                                                     \
              name->header.length - sizeof ( acpi_description_header );        \
    }
#endif    //  ifndef INIT_ENTRIES_FOR_SDT



/**
 * @brief Kernel Entry point. All architectures end up calling this function
 * after their respective loading and setup processes.
 *
 * @param mmap the location of the memory map. It's currently a void pointer
 * because the memory map type is not yet defined
 *
 * @param magic the location of the magic number which tells us what
 * specific environment we are in. It's a void pointer since the magic type
 * is not yet defined.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

void kmain ( void *const mmap, kmain_magic *const magic )
{
    while ( 1 )
    {
        //  logic.
    }

    //  halt the processor once done
    halt ( );
}
#pragma GCC diagnostic pop