/**
 * @file main.c
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Actual Main entry point.
 * @version 1
 * @date 2022-03-21
 *
 * @copyright Copyright (C) 2022. Intellectual property of the author(s) listed above.
 *
 */
#include <kernel/entry/main.h>

#include <stdint.h>

/**
 * @brief Entry point specifically for UEFI operating systems where we are
 * loaded directly by the device.
 * @details Exits boot-services after gathering necessary information and calling
 * the important functions.
 * @param image the image we loaded from
 * @param systemTable the EFI system table
 * @return EFI_STATUS Misnomer: we don't (intentionally) return from this function.
 */
EFI_STATUS efi_main(EFI_HANDLE image, EFI_SYSTEM_TABLE *systemTable)
{

    // join up with all other paths
    kmain();
}

/**
 * @brief Entry point specifically for GRUB Multiboot systems.
 * @details allows clobbering of the mb(2) information structure after gathering
 * all necessary information and calling the important functions.
 * @param magic the magic number
 * @param addr the address of the information structure.
 */
void _kmain(uint32_t magic, intmax_t addr)
{

    // join up with all other paths
    kmain();
}

void kmain()
{
    // the main loop in our operating system
    while (1)
    {
        // wait
        // service task
        // logging information.
    }
}