#ifndef GOS_LOADER_EFI_STATUS_HPP
#define GOS_LOADER_EFI_STATUS_HPP

#include <cstddef>

namespace efi {
    using status_y = std::size_t;

    enum struct status_type_e {
        success,    // only success code is code 0
        warning_code,   // warning codes are neither successes nor errors
        error_code, // error codes have the most significant bit set
    };

    status_type_e categorize_status(status_y);

    // function that returns true if a status is specifically an error code.
    bool error_code(status_y);

    // UEFI uses char16_t for simple text output protocol, so that's
    // what the loader uses to store error descriptions for standardized
    // errors.
    char16_t const* get_status_description(status_y);
}

#endif // ifndef GOS_LOADER_EFI_STATUS_HPP