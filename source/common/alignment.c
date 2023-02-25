/**
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief implements various functions that perform pointer alignment checking.
 * @version 1
 * @date 2023-02-23
 * @copyright Copyright (C) 2023. Intellectual property of the author(s) listed
 * above.
 */

#include <common/alignment.h>
#include <stdint.h>

int aligned_on_n_byte(void *pointer, uint8_t boundary)
{
    if ( bytes_off_n_byte_align(pointer, boundary) != 0)
    {
        return 0;
    } else
    {
        return 1;
    }
}

void *align_to_n_byte(void *pointer, uint8_t boundary)
{
    uintmax_t pointer_as_int = (uintmax_t)pointer;
    pointer_as_int -= bytes_off_n_byte_align(pointer, boundary);
    return (void *)pointer_as_int;
}

uintmax_t bytes_off_n_byte_align(void *pointer, uint8_t boundary)
{
    uintmax_t pointer_as_int = (uintmax_t)pointer;
    return pointer_as_int % boundary;
}