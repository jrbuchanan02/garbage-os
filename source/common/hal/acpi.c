/**
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief implements the functions required in the acpi.h file.
 * @version 1
 * @date 2023-02-21
 * @copyright Copyright (C) 2023. Intellectual property of the author(s) listed
 * above.
 */

#include <common/alignment.h>
#include <common/hal/acpi.h>
#include <stdint.h>

/**
 * @fn is_valid_rsdp_table
 * @brief Checks if an (alleged) RSDP pointer is valid.
 * @param table memory address of the suspected table.
 * @return Whether or not the table is valid and if so, what ACPI RSD table
 * revision it describes.
 */
acpi_rsdp_table_validity is_valid_rsdp_table ( void *table )
{
    char rsd_ptr [ 8 ] = "RSD PTR ";
    if ( ! aligned_on_n_byte ( table, 16 ) ) { return ACPIRSDP_INVALID; }
    acpi_old_rsdp *old_part = table;

    for ( int i = 0; i < 8; i++ )
    {
        if ( old_part->signature [ i ] != rsd_ptr [ i ] )
        {
            return ACPIRSDP_INVALID;
        }
    }

    uint8_t *table_bytes = table;
    uint8_t  table_sum   = 0;

    for ( int i = 0; i < 20; i++ ) { table_sum += table_bytes [ i ]; }

    if ( table_sum != 0 ) { return ACPIRSDP_INVALID; }

    //  now check if it's also a valid revision 2 table.
    acpi_new_rsdp *new_part = table;
    //  signature is already valid, just check if all the bytes in the table
    //  sum to zero.
    table_sum               = 0;
    for ( uint32_t i = 0; i < new_part->length; i++ )
    {
        table_sum += table_bytes [ i ];
    }
    if ( table_sum != 0 )
    {
        return ACPIRSDP_VALID_1;
    } else {
        return ACPIRSDP_VALID_2;
    }
}
