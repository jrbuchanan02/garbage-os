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

EFI_SYSTEM_TABLE *systemTablePointer;

EFI_HANDLE bootDisk;

EFI_STATUS EFIAPI efi_main ( EFI_HANDLE        imageHandle,
                             EFI_SYSTEM_TABLE *systemTable )
{
    //  setup things...
    systemTablePointer = systemTable;
    bootDisk           = imageHandle;

    //  1. Log that we have loaded into Garbage OS.
    //  2. Map memory (with plenty of debugging information)
    EFIMemoryDescriptor *mmap              = 0;
    UINTN                mmapSize          = 0;
    UINTN                mmapKey           = 0;
    UINTN                descriptorSize    = 0;
    UINT32               descriptorVersion = 0;
    // HACK: Assumes that all calls behave as we expect.
    systemTablePointer->BootServices->GetMemoryMap(&mmapSize, mmap, &mmapKey, &descriptorSize, &descriptorVersion);
    systemTablePointer->BootServices->AllocatePool(EfiConventionalMemory, mmapSize, &mmap);
    systemTablePointer->BootServices->GetMemoryMap(&mmapSize, mmap, &mmapKey, &descriptorSize, &descriptorVersion);
    
    //  3. Apply memory map and translate all necessary addresses
    //  4. Exit BootServices
    //  5. Halt processor and clear interrupts.

    kmain ( 0, 0 );
}