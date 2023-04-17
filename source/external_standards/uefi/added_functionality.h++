/**
 * @file added_functionality.h++
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Defines added functionality and functions not part of the UEFI standard but part of
 * Garbage OS's way of talking to the standard.
 * @version 1
 * @date 2023-04-13
 *
 * This file defines convenience methods for the EFI standard. Logically, the argument
 * could be made to put added functionality in the same file as efi.h++, but I decided
 * to make the efi.h++ file *only* contain the data behind the implementation.
 *
 * @copyright This file is part of Garbage OS. Garbage OS is Copyright (C) 2023
 * Joshua Buchanan and published under the LGPL license version 2.1. You are
 * free to use this source code in your project even if your code is not open
 * source.
 *
 */
#ifndef EXTERNAL_STANDARDS_UEFI_ADDED_FUNCTIONALITY_HPP
#define EXTERNAL_STANDARDS_UEFI_ADDED_FUNCTIONALITY_HPP

#include <external_standards/uefi/efi.h++>
#include <utility_functions/crc32.h++>

namespace efi {

    /**
     * @brief Equality comparison operator for efi::guid
     *
     * @param lhs efi::guid
     * @param rhs efi::guid
     * @return whether lhs and rhs have equal contents.
     */
    constexpr bool operator== ( guid const &lhs, guid const &rhs ) noexcept {
        if ( lhs.data1 != rhs.data1 ) { return false; }
        if ( lhs.data2 != rhs.data2 ) { return false; }
        if ( lhs.data3 != rhs.data3 ) { return false; }
        for ( std::size_t i = 0; i < 8; i++ ) {
            if ( lhs.data4 [ i ] != rhs.data4 [ i ] ) { return false; }
        }
        return true;
    }

    /**
     * @brief Calculates a CRC-32 checksum of a given table_header (or system-table, boot-services, or runtime-services)
     * and comapres it with the checksum field in the header to check if the table can be considered valid.
     *
     * @param table the table to check
     * @return true if the table has a valid checksum
     * @return false if the table does not have a valid checksum
     */
    constexpr bool is_table_checksum_valid( table_header *const &table ) noexcept {
        uint32 table_checksum  = table->crc;
        table->crc             = 0;
        uint32 actual_checksum = crc::crc( crc::type::ieee_802_3, table, table->size );
        table->crc             = table_checksum;
        return actual_checksum == table_checksum;
    }
}    // namespace efi

#endif    // ifndef EXTERNAL_STANDARDS_UEFI_ADDED_FUNCTIONALITY_HPP