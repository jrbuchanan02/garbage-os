#ifndef GOS_TYPES_POINTERS_HPP
#define GOS_TYPES_POINTERS_HPP

// this header contains some using declarations which
// hopefully make pointer types easier to read and understand.

namespace types {
    // pointer type which indicates a C-compatible reference
    template<typename type>
    using ref = type *;
    // pointer type which indicates a degreaded array (i.e., it's an array)
    template<typename type>
    using array = type *;
    // poitner type which indicates ownership (whoever "owns" the owner
    // has to either free it when they are done or give it to someone else
    // expecting an owner)
    template<typename type>
    using owner = type *;
    // pointer type which indicates no ownership. do not delete viewers.
    template<typename type>
    using viewer = type *;
}

#endif // ifndef GOS_TYPES_POINTERS_HPP