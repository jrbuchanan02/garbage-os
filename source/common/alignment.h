/**
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief declares various functions that perform pointer alignment checking.
 * @version 1
 * @date 2023-02-23
 * @copyright Copyright (C) 2023. Intellectual property of the author(s) listed
 * above.
 */

#ifndef SOURCE_COMMON_ALIGNMENT_H
#define SOURCE_COMMON_ALIGNMENT_H

#include <stdint.h>

/**
 * @fn aligned_on_n_byte
 * @brief Checks if a memory address is aligned on an n-byte boundary
 * @param pointer the memory address to check
 * @param boundary the amount of bytes that the pointer should be aligned to.
 * @return 0 pointer is not aligned on the n-byte boundary
 * @return 1 pointer is aligned on the n-byte boundary.
*/
int aligned_on_n_byte(void *pointer, uint8_t boundary);

/**
 * @fn align_to_n_byte
 * @brief Aligns a memory address to an n-byte boundary.
 * @note Always outputs a memory address less than or equal to the one passed
 * as an argument.
 * @param pointer the memory address to align
 * @param boundary the amount of bytes that the pointer should be aligned to.
*/
void *align_to_n_byte(void *pointer, uint8_t boundary);

/**
 * @fn bytes_off_n_byte_align 
 * @brief indicates how many bytes to subtract from a pointer to make it aligned
 * on some byte boundary.
 * @param pointer the memory address in question.
 * @param boundary the boundary to check for alignment with
 * @example bytes_off_n_byte_align(3, 2) returns 1
 * @example bytes_off_n_byte_align(6, 3) returns 0
*/
uintmax_t bytes_off_n_byte_align(void *pointer, uint8_t boundary);

#endif // ifndef SOURCE_COMMON_ALIGNMENT_H