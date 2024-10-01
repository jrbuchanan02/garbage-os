#ifndef GOS_LOADER_EFI_CONFIGURATION_TABLE_HPP
#define GOS_LOADER_EFI_CONFIGURATION_TABLE_HPP

#include <loader/efi/guid.h++>
#include <loader/efi/macros.h++>

namespace efi {
    EFI_STRUCT alignas(8) configuration_table {
        guid vendor_guid;
        void *vendor_table;
    };
}

#endif // ifndef GOS_LOADER_EFI_CONFIGURATION_TABLE_HPP