/**
 * @file flexible_array.h++
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief A macro that allows declaring a flexible-length array member in a
 * struct to prvent -Werror=pedantic from allowing program compilation.
 * @version 1
 * @date 2023-04-14
 *
 * @copyright Copyright (C) 2023. Intellectual property of the author(s) listed above.
 *
 */
#ifndef SOURCE_UTILITY_FUNCTIONS_FLEXIBLE_ARRAY_HPP
#define SOURCE_UTILITY_FUNCTIONS_FLEXIBLE_ARRAY_HPP

#ifndef FLEXIBLE_ARRAY
#  define FLEXIBLE_ARRAY( TYPE, VARIABLE )                                                                             \
      _Pragma( "GCC diagnostic push" ) _Pragma( "GCC diagnostic ignored \"-Wpedantic\"" ) TYPE VARIABLE [];            \
      _Pragma( "GCC diagnostic pop" )
#endif    // ifndef FLEXIBLE_ARRAY_OF

#endif    // ifndef SOURCE_UTILITY_FUNCTIONS_FLEXIBLE_ARRAY_HPP