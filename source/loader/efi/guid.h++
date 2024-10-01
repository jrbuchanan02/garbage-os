#ifndef GOS_LOADER_EFI_GUID_HPP
#define GOS_LOADER_EFI_GUID_HPP

#include <cstdint>

#include <loader/efi/macros.h++>

namespace efi {
    // defined here in the EFI format (using data1-4)
    struct alignas(8) guid {
        std::uint32_t data1;
        std::uint16_t data2;
        std::uint16_t data3;
        std::uint8_t data4[8];
    };

    static_assert(sizeof(guid) == 16);
    static_assert(alignof(guid) == 8);
}

#endif // ifndef GOS_LOADER_EFI_GUID_HPP