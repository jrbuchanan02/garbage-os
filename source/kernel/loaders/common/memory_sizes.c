/**
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Implements functions described in memory_sizes.h
 * @version 1
 * @date 2023-02-20
 * @copyright Copyright (C) 2023. Intellectual property of the author(s) listed
 * above.
 */

#include <kernel/loaders/common/memory_sizes.h>
#include <stdint.h>

uintmax_t pages_from_bytes_ex ( uintmax_t bytes, uintmax_t page_size )
{
    uintmax_t full_pages = bytes / page_size;
    uintmax_t part_page  = ( bytes % page_size == 0 ) ? 0 : 1;
    return full_pages + part_page;
}

uintmax_t pages_from_bytes ( uintmax_t bytes )
{
    return pages_from_bytes_ex ( bytes, 4096 );
}

void *get_next ( void *item, uintmax_t size )
{
    return ( void * ) ( ( ( uintmax_t ) item ) + size );
}

void *get_prev ( void *item, uintmax_t size )
{
    return ( void * ) ( ( ( uintmax_t ) item ) - size );
}

void *index_relative ( void *item, uintmax_t size, intmax_t jump )
{
    return ( void * ) ( ( ( uintmax_t ) item ) + jump * size );
}