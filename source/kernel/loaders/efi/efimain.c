/**
 * @file efimain.c
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Entry point for all EFI builds.
 * @version 1
 * @date 2022-03-23
 *
 * @copyright Copyright (C) 2022. Intellectual property of the author(s) listed
 * above.
 *
 */
#include <kernel/entry/main.h>
#include <kernel/loaders/efi/efi.h>

struct _efi_memory_map;

EFI_SYSTEM_TABLE     *system_table_pointer;
EFI_BOOT_SERVICES    *boot_services;
EFI_RUNTIME_SERVICES *runtime_services;

EFI_HANDLE boot_disk;

EFI_STATUS EFIAPI efi_main ( EFI_HANDLE        image_handle,
                             EFI_SYSTEM_TABLE *system_table )
{
    //  setup things...
    system_table_pointer = system_table;
    boot_disk            = image_handle;

    boot_services    = system_table_pointer->BootServices;
    runtime_services = system_table_pointer->RuntimeServices;

    kmain ( 0, 0 );

    return -1;
}
