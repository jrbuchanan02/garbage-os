/**
 * @file hang.h++
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief A type of abort where the system hangs forever.
 * @version 1
 * @date 2023-04-13
 *
 * @copyright Copyright (C) 2023. Intellectual property of the author(s) listed above.
 *
 */
#ifndef SOURCE_LOADER_ABORT_HANG_HPP
#define SOURCE_LOADER_ABORT_HANG_HPP

namespace loader {

    using message_function = void ( * )( ) noexcept;

    void __attribute__( ( noreturn ) ) hang_abort( ) noexcept {
        while ( true ) { }
    }

    void __attribute__( ( noreturn ) ) hang_abort_with_message( message_function const &message_to_send ) noexcept {
        message_to_send( );
        hang_abort( );
    }

}    // namespace loader

#endif    // ifndef SOURCE_LOADER_ABORT_HANG_HPP