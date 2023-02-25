/**
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Contains common information about paging and how many pages to
 * allocate some amount of bytes
 * @version 1
 * @date 2023-02-20
 * @copyright Copyright (C) 2023. Intellectual property of the author(s) listed
 * above.
 */

#ifndef KERNEL_LOADERS_COMMON_MEMORY_SIZES_H
#define KERNEL_LOADERS_COMMON_MEMORY_SIZES_H

#include <stdint.h>

uintmax_t pages_from_bytes ( uintmax_t );

uintmax_t pages_from_bytes_ex ( uintmax_t, uintmax_t );

/**
 * Several standards have types defined as C structs where the size of the item
 * should be the size the item says it is (not sizeof(X)). This function
 * performs the addition to get the next item of an array of such a type.
 */
void *get_next ( void *, uintmax_t );
/**
 * Several standards have types defined as C structs where the size of the item
 * should be the size the item says it is (not sizeof(X)). This function
 * performs the addition to get the previous item of an array of such a type.
 */
void *get_prev ( void *, uintmax_t );

/**
 * Several standards have types defined as C structs where the size of the item
 * should be the size the item says it is (not sizeof(X)). This function 
 * performs the addition to get n items ahead (positive number) or behind
 * (negative number) of the current address.
*/
void *index_relative(void *, uintmax_t, intmax_t);

#endif    //  ifndef KERNEL_LOADERS_COMMON_MEMORY_SIZES_H