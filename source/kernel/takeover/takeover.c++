/**
 * @file takeover.c++
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Takes over the system from the loader.
 * @version 1
 * @date 2023-04-08
 *
 * @copyright Copyright (C) 2023. Intellectual property of the author(s) listed
 * above.
 *
 */

#include <kernel/takeover/system.h++>

/**
 * @brief Takes over the system from the loader.
 * @note this function initially handles a cooperative multitasking systme and
 * does not return in the traditional way: instead of unloading "normally" this
 * module is simply omitted from the task switching routine once it signals to
 * the task module that it can start the pre-emptive multitasking phase.
 * @param system the system's state, defined in system.h++
 */
void begin_takeover ( takeover::system const *const system )
{
    takeover::immediate_state state;
    state.device_tables_compiled        = false;
    state.memory_management_ready       = false;
    state.preemptive_multitasking_ready = false;

    while ( ! state.preemptive_multitasking_ready )
    {
        //  call one of the other module's setup functions
        //  with the current system state.

        //  choose the next module.
    }

    //  signal to task swtiching module that it can take over

    //  wait for the first task switch
    while ( true ) { }
}