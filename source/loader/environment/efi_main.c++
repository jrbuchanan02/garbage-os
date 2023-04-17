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
#include <external_standards/uefi/added_functionality.h++>
#include <external_standards/uefi/efi.h++>
#include <loader/abort/hang.h++>
#include <loader/environment/environment.h++>
#include <loader/setup/loader_main.h++>

namespace loader {
    class efi_environment : public environment { };
}    //  namespace loader

loader::efi_environment local_environment;

// efi::system_table     *table;
// efi::boot_services    *boot_services;
// efi::runtime_services *runtime_services;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

extern "C" EFI_API efi::status efi_main( efi::handle image, efi::system_table *system_table ) noexcept {
    //  ensure environment is safe.
    // table = system_table;
    if ( !efi::is_table_checksum_valid( (efi::table_header *) system_table ) ) { loader::hang_abort( ); }
    // boot_services    = table->boot_services;
    // runtime_services = table->runtime_services;
    //   ensure environment is supported
    //   construct an efi_environment and set environment::current_environment
    //   hand control to the setup subsystem.
    ::loader_main( &local_environment );

    return ~efi::status( 0 );
}

#pragma GCC diagnostic pop
