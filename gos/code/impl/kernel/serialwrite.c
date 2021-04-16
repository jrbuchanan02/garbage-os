/**
 * @file serialwrite.c
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com) (github: @natara1939344)
 * @brief implements writing to serial port
 * @version 0.1
 * @date 2021-04-15
 * 
 * @copyright Copyright (C) 2021 under the LGPL 2.1
 * 
 */

#include "../../intf/kernel/serialwrite.h"

#ifndef EFI_API
// do something!
#pragma message "You messed up includes"
#endif



EFI_SYSTEM_TABLE *efi_table;
EFI_RUNTIME_SERVICES *runtime_table;
EFI_BOOT_SERVICES *boot_table; // I think those may have ended already tho.
EFI_SERIAL_IO_PROTOCOL *serial_thingy;
EFI_GUID serial_io_guid = SERIAL_IO_PROTOCOL;

/**
 * @brief see stringstuff.c
 * @see stringstuff.c's strlen function
 * @param s the string
 * @return uint64_t the length
 */
uint64_t strlen(char *s) {
    uint64_t ret = 0LLU;
    for (; s[ret]; ret++);
    return ret;
}

/**
 * @brief Grabs a handle to the serial io port. Praying that boto services have
 * not been terminated yet.
 * @return TRUE (1) on successful exit, FALSE (0) on failure
 */
BOOLEAN startup_service()
{
    return FALSE;
}

