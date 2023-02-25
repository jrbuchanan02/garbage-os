/**
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Implements any and all helper-functions defined in efi.h. These 
 * functions are not in the standard, but most all programs including this file
 * will benefit from these functions.
 * @version 1
 * @date 2023-02-21
 * @copyright Copyright (C) 2023. Intellectual property of the author(s) listed
 * above.
*/

#include <kernel/loaders/efi/efi.h>

int efi_guid_eq_check ( efi_guid *lhs, efi_guid *rhs )
{
    if ( lhs->data_group_1 != rhs->data_group_1 ) { return 0; }
    if ( lhs->data_group_2 != rhs->data_group_2 ) { return 0; }
    if ( lhs->data_group_3 != rhs->data_group_3 ) { return 0; }
    for ( int i = 0; i < 8; i++ )
    {
        if ( lhs->data_group_4 [ i ] != rhs->data_group_4 [ i ] ) { return 0; }
    }
    return 1;
}