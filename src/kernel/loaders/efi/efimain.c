/**
 * @file efimain.c
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Entry point for all EFI builds.
 * @version 1
 * @date 2022-03-23
 *
 * @copyright Copyright (C) 2022. Intellectual property of the author(s) listed above.
 *
 */
#include <loaders/efi/efi.h>

#include <kernel/entry/main.h>

EFI_SYSTEM_TABLE *systemTablePointer;

EFI_HANDLE bootDisk;


EFI_STATUS EFIAPI efi_main(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable)
{
    // setup things...
    systemTablePointer = systemTable;
    bootDisk = imageHandle;

    // 1. Log that we have loaded into Garbage OS.

    kmain(0, 0);
}