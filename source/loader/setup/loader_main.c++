/**
 * @file loader_main.c++
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Entry point for the loader.
 * @version 1
 * @date 2023-04-07
 *
 * @copyright Copyright (C) 2023. Intellectual property of the author(s) listed
 * above.
 *
 */

#include <loader/environment/environment.h++>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void loader_main ( loader::environment const *const local )
{
    //  create and apply a virtual memory map
    //  locate, validate, and load the kernel from disk
    //  find, validate, and perform some initial processing on system device
    //  drivers
}
#pragma GCC diagnostic pop