#ifndef ASM_HPP
#define ASM_HPP

#include <kernel/types.h++>

extern "C" {
    // does exactly what you think it does.
    [[noreturn]] void halt(void);
    // does exactly what you think it does.
    std::size_t get_this_thread_id(void);
}

#endif // ifndef ASM_HPP