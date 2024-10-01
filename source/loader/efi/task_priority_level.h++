#ifndef GOS_LOADER_EFI_TASK_PRIORITY_LEVEL_HPP
#define GOS_LOADER_EFI_TASK_PRIORITY_LEVEL_HPP

#include <cstddef>

namespace efi {
    enum struct task_priority_level_e : std::size_t {
        application = 4,
        callback = 8,
        notify = 16,
        high_level = 31,
    };
}

#endif // ifndef GOS_LOADER_EFI_TASK_PRIORITY_LEVEL_HPP