#ifndef GOS_LOADER_EFI_EVENT_HPP
#define GOS_LOADER_EFI_EVENT_HPP

#include <cstdint>

#include <loader/efi/macros.h++>

namespace efi {
    using event_y = void *;

    using event_type_y = std::uint32_t;

    EFIAPI using event_notify_function = void (*)(event_y, void *);
}

#endif // ifndef GOS_LOADER_EFI_EVENT_HPP