/**
 * @file utility_functions.c++
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Tests items in the utility functions.
 * @version 1
 * @date 2023-04-11
 *
 * @copyright Copyright (C) 2023. Intellectual property of the author(s) listed above.
 *
 */

#include <utility_functions/crc32.h++>
#include <utility_functions/multi_part_integer.h++>

#include <iostream>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <build/generated/crc32results.h++>
#pragma GCC diagnostic pop

bool test_crc32( );
bool test_uintby2( );

int main( int const argc, char const *const *const argv ) {
    std::cout << "Test called with commandline:\n";
    for ( int i = 0; i < argc; i++ ) { std::cout << argv [ i ] << " "; }
    std::cout << "\n";

    bool any_case_failed = false;
    if ( test_crc32( ) ) {
        std::cout << "CRC32 test passed.\n";
    } else {
        std::cout << "CRC32 test failed. See above for details.\n";
        any_case_failed = true;
    }

    if ( test_uintby2( ) ) {
        std::cout << "Unsigned Integer by 2's passed.\n";
    } else {
        std::cout << "Unsigned Integer by 2's failed. See above for details.\n";
        any_case_failed = true;
    }

    if ( any_case_failed ) {
        return -1;
    } else {
        return 0;
    }
}

bool test_crc32( ) {
    // test the algorithm to ensure that it actually is a CRC32 and that it's
    // interpreting the generator polynomials and masks as intended.
    bool failure = false;

    for ( std::size_t i = 0; i < value_count; i++ ) {
        std::uint32_t crc32_efi  = ieee_crc32 [ i ];
        std::string   value      = values [ i ];
        std::uint32_t calculated = crc::crc( crc::type::ieee_802_3, value.c_str( ), value.size( ) );
        if ( calculated != crc32_efi ) {
            // special case since it actually came up in development.
            // the check value of the IEEE (and coincincidentally the UEFI)
            // CRC-32 parameters, the ASCII string 123456789 has a checksum
            // of 0xCBF43926. Due to some oddity in how the build file wrote the
            // result, it put the wrong checksum for this value.
            if ( value == std::string( "123456789" ) and calculated == 0xCBF4'3926 ) { continue; }
            std::cout << std::hex;
            std::cout << "Expected: " << crc32_efi << " calculated: " << calculated << " for crc32 of "
                      << value.c_str( ) << "\n";
            failure = true;
        }
    }

    // fail by default
    if ( failure ) {
        return false;
    } else {
        return true;
    }
}

bool test_uintby2( ) {
    // ensure that all functionality works properly.
    // ensure that the code gives the correct results
    composite_uint128 uint128 = 0_128;

    bool case_failed          = false;
    if ( uint128.high_part != 0 ) {
        std::cout << "uint128 did not clear high part when set to zero\n";
        case_failed = true;
    }
    if ( uint128.low_part != 0 ) {
        std::cout << "uint128 did not clear low part when set to zero\n";
        case_failed = true;
    }

    composite_uint128 x, y, z;
    x = 0xFFFF'FFFF'FFFF'F000;
    y = 0x0000'0000'0000'1000;

    z = x + y;

    if ( z.high_part != 1 ) {
        std::cout << "Adding " << x.low_part << " and " << y.low_part << " did not carry the 1.\n";
        case_failed = true;
    }

    x   = 0xFFFF'FFFF'FFFF'FFFF;
    x <<= 64_128;
    x  += 0x0000'0000'0000'0000_128;
    y   = 1;

    if ( case_failed ) {
        return false;
    } else {
        return true;
    }
}