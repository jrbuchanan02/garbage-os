/**
 * @file multi_part_integer.h++
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief An integer made of other integers.
 * @version 1
 * @date 2023-04-11
 *
 * @copyright Copyright (C) 2023. Intellectual property of the author(s) listed
 * above.
 *
 */

#ifndef UTILITY_FUNCITONS_MULTI_PART_INTEGER_HPP
#define UTILITY_FUNCITONS_MULTI_PART_INTEGER_HPP

#include <concepts>
#include <cstdint>

template <std::unsigned_integral word> struct uintby2;

template <typename type, typename... args>
concept native_unsigned_or_uintby2 =
        std::unsigned_integral<type> || std::same_as<type, uintby2<decltype( type::low_part )>>;

/**
 * @brief An unsigned integer comprised of two distinct, native integers.
 * Often 32-bit systems need to use a 64-bit integer and on occaision 64-bit
 * systems need ot use a 128-bit integer. This structure behaves almost like
 * a native integer but it does not satisfy things like std::unsigned_integral.
 * @tparam word
 */
template <std::unsigned_integral word> struct uintby2 {
    word low_part                                             = 0;
    word high_part                                            = 0;

    constexpr static std::size_t bit_count                    = 8 * sizeof( word );
    constexpr static word        highest_bit                  = word( 1 ) << ( bit_count - 1 );

    constexpr uintby2( ) noexcept                             = default;
    constexpr ~uintby2( ) noexcept                            = default;
    constexpr uintby2( uintby2 const & ) noexcept             = default;
    constexpr uintby2( uintby2 && ) noexcept                  = default;
    constexpr uintby2 &operator= ( uintby2 const & ) noexcept = default;
    constexpr uintby2 &operator= ( uintby2 && ) noexcept      = default;

    constexpr uintby2( std::unsigned_integral auto const &x ) noexcept {
        low_part = word( x );
        if ( sizeof( word ) < sizeof( x ) ) {
            high_part = word( x >> ( 8 * sizeof( word ) ) );
        } else {
            high_part = 0;
        }
    }

    consteval uintby2( int const &x ) noexcept {
        low_part = word( x );
        if ( sizeof( word ) < sizeof( x ) ) {
            high_part = word( x >> ( 8 * sizeof( word ) ) );
        } else {
            high_part = 0;
        }
    }

    constexpr uintby2 &operator= ( std::unsigned_integral auto const &x ) noexcept {
        low_part = word( x );
        if ( sizeof( word ) < sizeof( x ) ) {
            high_part = word( x >> ( 8 * sizeof( word ) ) );
        } else {
            high_part = 0;
        }
        return *this;
    }

    constexpr uintby2 &operator= ( std::unsigned_integral auto &&x ) noexcept {
        *this = x;
        return *this;
    }

    constexpr uintby2 operator+ ( native_unsigned_or_uintby2 auto const &x ) const noexcept {
        return add( *this, uintby2 { x } );
    }

    constexpr uintby2 operator- ( native_unsigned_or_uintby2 auto const &x ) const noexcept {
        return sub( *this, uintby2 { x } );
    }

    constexpr uintby2 operator* ( native_unsigned_or_uintby2 auto const &x ) const noexcept {
        return mul( *this, uintby2 { x } );
    }

    constexpr uintby2 operator/ ( native_unsigned_or_uintby2 auto const &x ) const noexcept {
        return div( *this, uintby2 { x } );
    }

    constexpr uintby2 operator% ( native_unsigned_or_uintby2 auto const &x ) const noexcept {
        return mod( *this, uintby2 { x } );
    }

    constexpr uintby2 operator<< ( native_unsigned_or_uintby2 auto const &x ) const noexcept {
        // zeroes out number.
        if ( x > 16 * sizeof( word ) ) { return 0; }
        // eliminates high part entirely.
        if ( x > 8 * sizeof( word ) ) {
            uintby2 result { *this };
            result.high_part = low_part;
            result.low_part  = 0;
            return result << ( x - 8 * sizeof( word ) );
        }
        // i = x - x is shorthand to get i to be both mutable and
        // able to fit x.
        uintby2 result { *this };
        for ( auto i = x - x; i < x; i++ ) {
            result.high_part <<= 1;
            if ( result.low_part & result.highest_bit ) { result.high_part |= 1; }
            result.low_part <<= 1;
        }
        return result;
    }

    constexpr uintby2 &operator<<= ( native_unsigned_or_uintby2 auto const &x ) noexcept {
        return *this = *this << x;
    }

    constexpr uintby2 &operator+= ( native_unsigned_or_uintby2 auto const &x ) noexcept {
        return *this = *this + x;
    }

    constexpr uintby2 &operator++ ( int ) noexcept {
        return *this += uintby2 { 1 };
    }

    constexpr auto operator<=> ( native_unsigned_or_uintby2 auto const &x ) const noexcept {
        uintby2 as_same { x };
        if ( high_part == as_same.high_part ) {
            return low_part <=> as_same.low_part;
        } else {
            return high_part <=> as_same.high_part;
        }
    }

private:
    /**
     * @brief Checks if adding two numbers would carry the 1.
     *
     * @param lhs
     * @param rhs
     * @return
     */
    constexpr static bool carries( word const &lhs, word const &rhs ) {
        bool carry = false;
        for ( std::size_t i = 0; i < 8 * sizeof( word ); i++ ) {
            word         left_bit  = lhs & ( word( 1 ) << i );
            word         right_bit = rhs & ( word( 1 ) << i );
            std::uint8_t one_count = 0;
            if ( left_bit ) { one_count++; }
            if ( right_bit ) { one_count++; }
            if ( carry ) { one_count++; }

            if ( one_count >= 2 ) { carry = true; }
        }
        return carry;
    }

    constexpr static uintby2 add( uintby2 const &lhs, uintby2 const &rhs ) noexcept {
        uintby2 result { lhs };
        // we need to check the carry from specificalyl the final bit in the array.
        bool    carry = carries( lhs.low_part, rhs.low_part );
        if ( carry ) { result.high_part++; }
        result.low_part  += rhs.low_part;
        result.high_part += rhs.high_part;
        return result;
    }

    constexpr static uintby2 sub( uintby2 const &lhs, uintby2 const &rhs ) noexcept {
        uintby2 result { lhs };
        // check if the low part rolls "under" (i.e., borrows a 1). If so, then
        // borrow the 1.
        bool    carry = carries( lhs.low_part, ~rhs.low_part + 1 );
        if ( carry ) { result.high_part--; }
        result.low_part  -= rhs.low_part;
        result.high_part -= rhs.high_part;
        return result;
    }

    constexpr static uintby2 mul( uintby2 const &lhs, uintby2 const &rhs ) noexcept {
        uintby2 result { 0 };
        for ( std::size_t i = 0; i < bit_count; i++ ) {
            uintby2 mask { 1 };
            mask                 <<= i;
            uintby2 bit_i_state    = mask & lhs;
            bool    is_bit_i_set   = false;
            if ( bit_i_state != 0 ) { is_bit_i_set = true; }

            if ( is_bit_i_set ) { result += ( rhs << i ); }
        }
        return result;
    }

    constexpr static uintby2 divmod( uintby2 const &lhs, uintby2 const &rhs, uintby2 &mod ) noexcept {
        // division here is like a bit-stream
        //      1011101
        // 101->1010000   -> 1
        //      0001101
        //       101000   -> 0
        //      0001101
        //        10100   -> 0
        //      0001101
        //         1010   -> 1
        //      0000011
        //          101   -> 0
        //     remainder 11

        // check for division by zero and explicitly get the same behavior.
        uintby2 divide_by_zero_result;
        if ( rhs == 0 ) { divide_by_zero_result = lhs.low_part / rhs.low_part; }

        // lhs -> number being divided
        // rhs -> number dividing
        uintby2 result { 0 };
        uintby2 intermediate { lhs };
        for ( std::size_t i = bit_count; i > 0; i++ ) {
            std::size_t shift    = i - 1;
            uintby2     multiple = rhs << shift;
            if ( multiple <= intermediate ) {
                result       += uintby2 { 1 } << shift;
                intermediate -= multiple;
            }
        }
        mod = intermediate;
        return result;
    }

    constexpr static uintby2 div( uintby2 const &lhs, uintby2 const &rhs ) noexcept {
        return divmod( lhs, rhs, uintby2 { } );
    }

    constexpr static uintby2 mod( uintby2 const &lhs, uintby2 const &rhs ) noexcept {
        uintby2 result;
        // we don't care about the quotient.
        ( (void) divmod( lhs, rhs, result ) );
        return result;
    }

    constexpr static uintby2 bitwise_and( uintby2 const &lhs, uintby2 const &rhs ) noexcept {
        uintby2 result { 0 };
        result.low_part  = lhs.low_part & rhs.low_part;
        result.high_part = lhs.high_part & rhs.high_part;

        return result;
    }

    constexpr static uintby2 bitwise_or( uintby2 const &lhs, uintby2 const &rhs ) noexcept {
        uintby2 result { 0 };
        result.low_part  = lhs.low_part | rhs.low_part;
        result.high_part = lhs.high_part | rhs.high_part;

        return result;
    }

    constexpr static uintby2 bitwise_xor( uintby2 const &lhs, uintby2 const &rhs ) noexcept {
        uintby2 result { 0 };
        result.low_part  = lhs.low_part ^ rhs.low_part;
        result.high_part = lhs.high_part ^ rhs.high_part;

        return result;
    }
};

static_assert( native_unsigned_or_uintby2<uintby2<unsigned>> );

using composite_uint64  = uintby2<std::uint32_t>;
using composite_uint128 = uintby2<std::uint64_t>;

constexpr composite_uint128 operator""_128 ( unsigned long long int x ) {
    composite_uint128 result = x;
    return result;
}

#endif    //  ifndef UTILITY_FUNCITONS_MULTI_PART_INTEGER_HPP