#ifndef GOT_LOADER_EFI_TABLE_HEADER_HPP
#define GOT_LOADER_EFI_TABLE_HEADER_HPP

#include <cstdint>

#include <loader/efi/macros.h++>

namespace efi {

    enum struct signature_e : std::uint64_t {
        boot_services = 0x56524553544f4f42LLU,
        system_table = 0x5453595320494249LLU,
    };

    EFI_STRUCT table_header {
        signature_e signature;
        std::uint32_t revision;
        std::uint32_t header_size;
        std::uint32_t crc32;
        std::uint32_t reserved;
    }; 
    static_assert(sizeof(table_header) == 24);


}

#endif // ifndef GOT_LOADER_EFI_TABLE_HEADER_HPP