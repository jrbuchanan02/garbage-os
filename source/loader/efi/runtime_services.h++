#ifndef GOS_LOADER_EFI_RUNTIME_SERVICES_HPP
#define GOS_LOADER_EFI_RUNTIME_SERVICES_HPP

#include <loader/efi/macros.h++>
#include <loader/efi/table_header.h++>

namespace efi {
    EFIAPI EFI_STRUCT runtime_services : public table_header {

    };
}

#endif // ifndef GOS_LOADER_EFI_RUNTIME_SERVICES_HPP