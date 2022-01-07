
#include<efi.h>
#include<efilib.h>

void logFirmwareVendor ( EFI_SYSTEM_TABLE const *const );

/**
 * @brief Panic routine. The big red button.
 *
 * @param message the error message.
 * @param imageHandle the image we loaded from
 * @param table the table.
 */
void panic ( WCHAR *const message , EFI_HANDLE imageHandle , EFI_SYSTEM_TABLE const *const table ) {
    table->BootServices->Exit ( imageHandle , EFI_ABORTED , StrLen ( message ) , message );
}

void listMemoryMap ( );


/**
 * \brief entry point for the operating system.
 * \param imageHandle a handle to the disk image that we loaded from
 * \param systemTable the UEFI System table.
 */
EFI_STATUS EFIAPI efi_main ( EFI_HANDLE imageHandle , EFI_SYSTEM_TABLE *systemTable ) {
    InitializeLib ( imageHandle , systemTable );
    logFirmwareVendor ( systemTable );
    listMemoryMap ( );

    return EFI_SUCCESS;
}


void logFirmwareVendor ( EFI_SYSTEM_TABLE const *const table ) {
    CHAR16 *vendorName = table->FirmwareVendor;
    Print ( L"Firmware vendor: %s\n" , vendorName );
}

void listMemoryMap ( table ) {
    Print ( L"Loading memory map...\n" );
    UINTN entries , memKey , descriptorSize;
    UINT32 descriptorVersion;

    EFI_MEMORY_DESCRIPTOR *descriptors = LibMemoryMap ( &entries , &memKey , &descriptorSize , &descriptorVersion );
    if ( descriptorVersion != 1 ) {
        Print ( L"For some reason, we have bad info on the memory descriptor.\n" );
    } else for ( UINTN i = 0; i < entries; i++ ) {
        CHAR16 physicalAddressBuffer [ 2 + 2 * sizeof ( void * ) ];
        CHAR16 virtualAddressBuffer [ 2 + 2 * sizeof ( void * ) ];
        Print ( L"Entry number %llu of %llu:\n" , i + 1 , entries );
        Print ( L"\tStart Address ( physical, virtual ):\n" );
        ValueToHex ( physicalAddressBuffer , ( descriptors + i * descriptorSize )->PhysicalStart );
        ValueToHex ( virtualAddressBuffer , ( descriptors + i * descriptorSize )->VirtualStart );
        Print ( L"\t\t 0x%s, 0x%s\n" , physicalAddressBuffer , virtualAddressBuffer );
        Print ( L"\tEnd Address ( physical, virtual ):\n" );
        UINTN pageCount = ( descriptors + i * descriptorSize )->NumberOfPages;
        ValueToHex ( physicalAddressBuffer , pageCount * ( descriptors + i * descriptorSize )->PhysicalStart );
        ValueToHex ( physicalAddressBuffer , pageCount * ( descriptors + i * descriptorSize )->VirtualStart );
        Print ( L"\t\t 0x%s, 0x%s\n" , physicalAddressBuffer , virtualAddressBuffer );
    }
}