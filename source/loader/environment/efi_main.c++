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

efi::system_table     *table;
efi::boot_services    *boot_services;
efi::runtime_services *runtime_services;

void message_about_corrupt_runtime_services( ) noexcept;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

extern "C" EFI_API efi::status efi_main( efi::handle image, efi::system_table *system_table ) noexcept {
    //  ensure environment is safe.
    table = system_table;
    // no point in remembering where boot / runtime services are if the system
    // table cannot properly tell us.
    if ( !efi::is_table_checksum_valid( (efi::table_header *) system_table ) ) { loader::hang_abort( ); }
    boot_services = table->boot_services;
    // the console output is actually a boot-services driver. If boot services
    // is not safe, then console output is not safe.
    if ( !efi::is_table_checksum_valid( (efi::table_header *) boot_services ) ) { loader::hang_abort( ); }
    runtime_services = table->runtime_services;
    if ( !efi::is_table_checksum_valid( (efi::table_header *) runtime_services ) ) {
        loader::hang_abort_with_message( &message_about_corrupt_runtime_services );
    }
    //   ensure environment is supported
    //   construct an efi_environment and set environment::current_environment
    //   hand control to the setup subsystem.
    ::loader_main( &local_environment );

    return ~efi::status( 0 );
}

#pragma GCC diagnostic pop

/**
 * @brief A function to use with loader::hang_abort_with_message to tell the user
 * that Garbage OS found a possibly bad Runtime Services table.
 *
 */
void message_about_corrupt_runtime_services( ) noexcept {
    efi::simple_text_output     *stdout     = table->console_out;
    constexpr static efi::char16 message [] = u"The Runtime Services on your machine did not add up. Garbage OS cannot "
                                              u"continue since it "
                                              u"does not know if your firmware is safe for it to use.";

    stdout->output_string( (efi::char16 *) message );
}