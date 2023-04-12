/**
 * @file crc32.h++
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief An implementation of the CRC32 function.
 * @version 1
 * @date 2023-04-07
 *
 * @copyright Copyright (C) 2023. Intellectual property of the author(s) listed
 * above.
 *
 */

#ifndef UTILITY_FUNCTIONS_CRC32_HPP
#define UTILITY_FUNCTIONS_CRC32_HPP

#include <cstdint>

namespace crc {
    using crc_register = std::uintmax_t;

    struct crc_parameters {
        crc_register polynomial;
        crc_register init_value;
        crc_register output_xor;
        crc_register bit_count   : 7;
        std::uint8_t reflect_in  : 1;
        std::uint8_t reflect_out : 1;
    };

    enum struct type {
        ieee_802_3,    // ethernet, UEFI, pkzip
        arc,
        xmodem,
        citt,
        // used to avoid throwing an exception. a default value that isn't
        // really useful in the CRC and always returns 0.
        garbage_in_garbage_out,
    };

    constexpr crc_parameters get_parameters( type const crc_type ) {
        constexpr crc_parameters params [] = {
                {0x04C1'1DB7, 0xFFFF'FFFF, 0xFFFF'FFFF, 32, 1, 1},
                {0x0000'8005, 0x0000'0000, 0x0000'0000, 16, 1, 1},
                {0x0000'8408, 0x0000'0000, 0x0000'0000, 16, 1, 1},
                {0x0000'1021, 0x0000'FFFF, 0x0000'0000, 16, 1, 1},
                {0x0000'0000, 0x0000'0000, 0x0000'0000, 32, 0, 0}
        };

        constexpr type types_to_params [] = {
                type::ieee_802_3,
                type::arc,
                type::xmodem,
                type::citt,
                type::garbage_in_garbage_out,
        };

        for ( std::size_t i = 0; i < sizeof( params ) / sizeof( params [ 0 ] ); i++ ) {
            if ( crc_type == types_to_params [ i ] ) { return params [ i ]; }
        }
        return get_parameters( type::garbage_in_garbage_out );
    }

    constexpr crc_register reflect( crc_register number, crc_register bits_to_reflect ) {
        crc_register result = 0;
        for ( crc_register i = 0; i < bits_to_reflect; i++ ) {
            if ( number & ( 1 << i ) ) { result |= ( 1 << ( bits_to_reflect - i - 1 ) ); }
        }
        return result;
    }

    constexpr crc_register width_mask( crc_register bit_count ) {
        return ( ( ( crc_register( 1 ) << ( bit_count - 1 ) ) - 1 ) << 1 ) | 1;
    }

    constexpr crc_register crc( type const &crc_type, void const *const &data, std::size_t const &length ) {
        crc_parameters const      params = get_parameters( crc_type );
        std::uint8_t const *const bytes  = (std::uint8_t const *const &) data;
        crc_register              result = params.init_value;

        for ( std::size_t i = 0; i < length; i++ ) {
            crc_register byte    = bytes [ i ];
            crc_register top_bit = crc_register( 1 ) << ( params.bit_count - 1 );
            if ( params.reflect_in ) { byte = reflect( byte, 8 ); }
            result ^= ( byte << ( params.bit_count - 8 ) );
            for ( std::size_t j = 0; j < 8; j++ ) {
                if ( result & top_bit ) {
                    result = ( result << 1 ) ^ params.polynomial;
                } else {
                    result <<= 1;
                }
                result &= width_mask( params.bit_count );
            }
        }

        if ( params.reflect_out ) { result = reflect( result, params.bit_count ); }
        return result ^ params.output_xor;
    }

    static_assert( reflect( 0, 0 ) == 0 );
    static_assert( reflect( 0, 32 ) == 0 );
    static_assert( reflect( 0x0F, 8 ) == 0xF0 );

    static_assert( width_mask( 1 ) == 0x1 );
    static_assert( width_mask( 2 ) == 0x3 );
    static_assert( width_mask( 3 ) == 0x7 );
    static_assert( width_mask( 4 ) == 0xF );
    static_assert( width_mask( 8 ) == 0xFF );
    static_assert( width_mask( 16 ) == 0xFFFF );
    static_assert( width_mask( 32 ) == 0xFFFF'FFFF );
    static_assert( width_mask( 64 ) == 0xFFFF'FFFF'FFFF'FFFF );

}    // namespace crc

#endif