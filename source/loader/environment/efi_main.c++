/**
 * @file efi_main.c++
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Entry point for the EFI environment.
 * @version 1
 * @date 2023-04-07
 *
 * @copyright Copyright (C) 2023. Intellectual property of the author(s) listed
 * above.
 *
 */

#include <external_standards/acpi/tables.h++>
#include <external_standards/uefi/efi.h++>
#include <loader/environment/environment.h++>
#include <loader/setup/loader_main.h++>
#include <utility_functions/crc32.h++>

namespace loader {
    class efi_environment : public environment {
    public:
    };
}    //  namespace loader

loader::efi_environment local_environment;

bool check_table_checksum( efi::table_header *table_with_header ) {
    std::uint32_t alleged_crc32 = table_with_header->crc32;
    table_with_header->crc32    = 0;
    // clang-format off
    std::uint32_t actual_crc32  = crc::crc(crc::type::ieee_802_3, table_with_header, table_with_header->header_size);
    // clang-format on
    table_with_header->crc32    = alleged_crc32;
    if ( actual_crc32 == alleged_crc32 ) {
        return true;
    } else {
        return false;
    }
}

enum class efi_firmware_safety {
    safe,
    unsafe_system_table,
    unsafe_boot_services,
    unsafe_runtime_services,
    hint_loader_corrupt,
};

efi_firmware_safety check_safety( efi::system_table *system_table ) {
    bool system_checksum = check_table_checksum( system_table );
    [[unlikely]] if ( system_checksum == false ) {
        return efi_firmware_safety::unsafe_system_table;
    } else [[unlikely]] if ( system_checksum != true ) {
        return efi_firmware_safety::hint_loader_corrupt;
    }
    bool boot_checksum = check_table_checksum( system_table->boot );
    [[unlikely]] if ( boot_checksum == false ) {
        return efi_firmware_safety::unsafe_boot_services;
    } else [[unlikely]] if ( boot_checksum != true ) {
        return efi_firmware_safety::hint_loader_corrupt;
    }

    bool runtime_checksum = check_table_checksum( system_table->runtime );
    [[unlikely]] if ( runtime_checksum == false ) {
        return efi_firmware_safety::unsafe_runtime_services;
    } else [[unlikely]] if ( runtime_checksum != true ) {
        return efi_firmware_safety::hint_loader_corrupt;
    }

    return efi_firmware_safety::safe;
}

void check_safety_and_handle( efi::system_table *system_table ) {
    efi_firmware_safety safety = check_safety( system_table );
    switch ( safety ) {
    case efi_firmware_safety::unsafe_system_table:
        //  hang the system
        while ( true ) { }
        break;
    case efi_firmware_safety::unsafe_boot_services:
        //  hang the system
        while ( true ) { }
        break;
    case efi_firmware_safety::unsafe_runtime_services:
        //  print an error message
        //  stall
        //  call boot services->exit
        break;
    case efi_firmware_safety::hint_loader_corrupt:
handle_corrupt_loader_hint:
        //  hang. If the loader is somehow corrupted, then we don't know
        //  if the firmware is actually safe!
        while ( true ) { }
        break;
    default:
        //  if none of the swtich-cases are correct, assume that the loader
        //  is corrupt by default.
        goto handle_corrupt_loader_hint;
        break;
    }
}

//  todo: replace these void *pointers with structures actually referring to
//  theese tables!
acpi::root_system_description_pointer_revision0 *rsdp         = nullptr;
void                                            *smbios_table = nullptr;

enum class system_support {
    missing_driver_tables,
    all_drivers_corrupted,
    located_driver_tables,
};

void store_rsdp_if_appropriate( efi::configuration_table config ) {
    using rsdp0       = acpi::root_system_description_pointer_revision0;
    rsdp0 *found_rsdp = (rsdp0 *) config.vendor_table;

    if ( config.vendor_guid == efi::acpi_rsdp_revision0_guid || config.vendor_guid == efi::acpi_rsdp_revision2_guid ) {
        if ( rsdp == nullptr || found_rsdp->revision > rsdp->revision ) { }
    }
}

void store_smbios_if_appropriate( efi::configuration_table config ) {
    if ( config.vendor_guid == efi::smbios3_table_guid ) { smbios_table = config.vendor_table; }
    if ( smbios_table == nullptr && config.vendor_guid == efi::smbios_table_guid ) {
        smbios_table = config.vendor_table;
    }
}

system_support check_system_support( std::unsigned_integral auto count, efi::configuration_table *configuration ) {
    if ( count == 0 ) { return system_support::missing_driver_tables; }

    for ( std::size_t i = 0; i < count; i++ ) {
        auto table = configuration [ i ];
        store_rsdp_if_appropriate( table );
        store_smbios_if_appropriate( table );
    }

    return system_support::located_driver_tables;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
extern "C" EFI_API efi::status efi_main( efi::handle image, efi::system_table *system_table ) {
    //  ensure environment is safe.
    check_safety_and_handle( system_table );
    //  ensure environment is supported
    //  construct an efi_environment and set environment::current_environment
    //  hand control to the setup subsystem.
    ::loader_main( &local_environment );

    return ~efi::status( 0 );
}
#pragma GCC diagnostic pop