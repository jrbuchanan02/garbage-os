#ifndef KERNEL_TYPES_HPP
#define KERNEL_TYPES_HPP

#if __has_include(<cstddef>)
#include <cstddef>
#else
namespace std {
    using size_t = __SIZE_TYPE__;
}
#endif

#endif // ifndef KERNEL_TYPES_HPP