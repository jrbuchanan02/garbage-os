/**
 * @file added_functionality.c++
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief The non-header code for the added_functionality.h++ file.
 * @version 1
 * @date 2023-07-16
 * 
 * @copyright This file is part of Garbage OS. Garbage OS is Copyright (C) 2023 Joshua Buchanan and published under the LGPL license version 2.1. You are free to use this source code in your project even if your code is not open source.
 * 
 */

#include <external_standards/uefi/added_functionality.h++>

constexpr bool efi::operator==(guid const &lhs, guid const &rhs) noexcept {
    if ( lhs.data1 != rhs.data1 ) { return false; }
    if ( lhs.data2 != rhs.data2 ) { return false; }
    if ( lhs.data3 != rhs.data3 ) { return false; }
    for ( std::size_t i = 0; i < 8; i++ ) {
        if ( lhs.data4 [ i ] != rhs.data4 [ i ] ) { return false; }
    }
    return true;
}

bool efi::is_table_checksum_valid(table_header *const &table ) noexcept {
    uint32 table_checksum  = table->crc;
    table->crc             = 0;
    uint32 actual_checksum = crc::crc( crc::type::ieee_802_3, table, table->size );
    table->crc             = table_checksum;
    return actual_checksum == table_checksum;
}