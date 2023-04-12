/**
 * @file loader_main.h++
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Declares the publically visible functions in the loader_main source
 * file for use in other parts of the loader, namely the environment subsystem.
 * @version 1
 * @date 2023-04-08
 *
 * @copyright Copyright (C) 2023. Intellectual property of the author(s) listed
 * above.
 *
 */

#ifndef LOADER_SETUP_LOADER_MAIN_HPP
#define LOADER_SETUP_LOADER_MAIN_HPP

#include <loader/environment/environment.h++>

void loader_main ( loader::environment const *const );

#endif    //  ifndef LOADER_SETUP_LOADER_MAIN_HPP