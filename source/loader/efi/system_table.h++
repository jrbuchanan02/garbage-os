#ifndef GOS_LOADER_EFI_SYSTEM_TABLE_HPP
#define GOS_LOADER_EFI_SYSTEM_TABLE_HPP

#include <cstdint>

#include <loader/efi/boot_services.h++>
#include <loader/efi/configuration_table.h++>
#include <loader/efi/handle.h++>
#include <loader/efi/macros.h++>
#include <loader/efi/runtime_services.h++>
#include <loader/efi/simple_text_input_protocol.h++>
#include <loader/efi/simple_text_output_protocol.h++>
#include <loader/efi/table_header.h++>

namespace efi {
    EFI_STRUCT system_table : public table_header {
        char16_t *firmware_vendor;
        std::uint32_t firmware_revision;
        handle console_in_handle;
        simple_text_input_protocol *con_in;
        handle console_out_handle;
        simple_text_output_protocol *con_out;
        handle standard_error_handle;
        simple_text_output_protocol *std_err;
        struct boot_services *boot_services;
        struct runtime_services *runtime_services;
        std::size_t configuration_table_entries;
        struct configuration_table *configuration_table;
    };
}

#endif // ifndef GOS_LOADER_EFI_SYSTEM_TABLE_HPP