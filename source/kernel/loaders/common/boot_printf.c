/**
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief defines the bprintf function declared in boot_printf.h
 * @version 1
 * @date 2023-02-20
 *
 * @copyright Copyright (C) 2023. Intellectual property of the author(s) listed
 * above.
 */

#include <kernel/loaders/common/boot_printf.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>

void puts ( some_putc, char const *const restrict str );
void putd ( some_putc, intmax_t );
void puto ( some_putc, uintmax_t );
void putx ( some_putc, uintmax_t, char );
void putu ( some_putc, uintmax_t );

#define MAX_DIGITS_DEC 19
#define MAX_DIGITS_HEX 16
#define MAX_DIGITS_OCT 22

#define DIGITS_DEC 10
#define DIGITS_HEX 16
#define DIGITS_OCT 8

#define MAX_DIVIDE_DEC 1000000000000000000LL
#define MAX_DIVIDE_HEX 0x1000000000000000
#define MAX_DIVIDE_OCT 01000000000000000000000LLU

#define PUTN_CHECK_ZERO( NUMBER, BUFFER, WRITTEN, FIRST_NONZERO, LABEL )       \
  if ( NUMBER == 0 )                                                           \
  {                                                                            \
    BUFFER [ WRITTEN++ ] = '0';                                                \
    FIRST_NONZERO        = 0;                                                  \
    goto LABEL;                                                                \
  }
#define PUTN_GET_DIGIT( DIGIT, A, K, MAX_K, DIGITS )                           \
  if ( K == MAX_K )                                                            \
  {                                                                            \
    DIGIT = ( char ) ( A / K );                                                \
  } else {                                                                     \
    DIGIT = ( char ) ( ( a % ( DIGITS * K ) ) / K );                           \
  }
#define PUTN_ADD_TO_BUFFER( B, W, D ) B [ W++ ] = D;

void bprintf ( some_putc putc, char const *const restrict fmt, ... )
{
    register uintmax_t i;
    //  register uintmax_t n;

    //  char next_in_format;

    va_list args;

    va_start ( args, fmt );

    for ( i = 0; fmt [ i ] != '\0'; i++ )
    {
        if ( fmt [ i ] == '%' )
        {
        } else {
            putc ( fmt [ i ] );
        }
    }
}

void puts ( some_putc putc, char const *const restrict str )
{
    register uintmax_t i = 0;
    for ( ; str [ i ]; i++ ) { putc ( str [ i ] ); }
}

void putd ( some_putc putc, intmax_t i )
{
    static intmax_t const m = MAX_DIVIDE_DEC / DIGITS_DEC;

    register intmax_t a = i, j, k = m;
    register intmax_t first_nonzero = -1, written = 0;
    //  1 extra for null
    //  1 extra for minus sign.
    signed char       buffer [ MAX_DIGITS_DEC + 2 ];
    signed char       digit;

    PUTN_CHECK_ZERO ( i, buffer, written, first_nonzero, putd_join )
    //  check only for putd
    if ( a < 0 )
    {
        a = -a;
        PUTN_ADD_TO_BUFFER ( buffer, written, '-' )
    }

    for ( j = 0; j < MAX_DIGITS_DEC - 1; j++, k /= DIGITS_DEC )
    {
        if ( k == m )
        {
            digit = ( char ) ( a / k );
        } else {
            digit = ( char ) ( ( a % ( DIGITS_DEC * k ) ) / k );
        }

        if ( digit < 0 || digit > 9 )
        {
            digit = '?';
        } else {
            if ( first_nonzero == -1 && digit != 0 ) { first_nonzero = j; }
            digit += '0';
        }

        PUTN_ADD_TO_BUFFER ( buffer, written, digit );
    }

putd_join:
    PUTN_ADD_TO_BUFFER ( buffer, written, '\0' )
    puts ( putc, ( char * ) ( buffer + first_nonzero ) );
}

void puto ( some_putc putc, uintmax_t i )
{
    static uintmax_t const m = MAX_DIVIDE_OCT / DIGITS_OCT;

    register uintmax_t a = i, j, k = m;
    register intmax_t  first_nonzero = -1, written = 0;
    //  oct defined as having 21 max digits.
    signed char        buffer [ MAX_DIGITS_OCT + 1 ];
    signed char        digit;

    PUTN_CHECK_ZERO ( i, buffer, written, first_nonzero, puto_join )

    for ( j = 0; j < MAX_DIGITS_OCT - 1; j++, k /= DIGITS_OCT )
    {
        if ( k == m )
        {
            digit = ( char ) ( a / k );
        } else {
            digit = ( char ) ( ( a % ( DIGITS_OCT * k ) ) / k );
        }

        if ( digit < 0 || digit > 7 )
        {
            digit = '?';
        } else {
            if ( first_nonzero == -1 && digit != 0 ) { first_nonzero = j; }
            digit += '0';
        }

        PUTN_ADD_TO_BUFFER ( buffer, written, digit )
    }

puto_join:
    PUTN_ADD_TO_BUFFER ( buffer, written, '\0' )
    puts ( putc, ( char * ) ( buffer + first_nonzero ) );
}

void putx ( some_putc putc, uintmax_t i, char h )
{
    static uintmax_t const m = MAX_DIVIDE_HEX / DIGITS_HEX;

    register uintmax_t a = i, j, k = m;
    register intmax_t  first_nonzero = -1, written = 0;
    signed char        buffer [ MAX_DIGITS_HEX + 1 ];
    signed char        digit;

    PUTN_CHECK_ZERO ( i, buffer, written, first_nonzero, putx_join )

    for ( j = 0; j < DIGITS_HEX - 1; j++, k /= DIGITS_HEX )
    {
        if ( k == m )
        {
            digit = ( char ) ( a / k );
        } else {
            digit = ( char ) ( ( a % ( DIGITS_HEX * k ) ) / k );
        }

        if ( digit < 0 || digit > 0xF )
        {
            digit = '?';
        } else {
            if ( first_nonzero == -1 && digit != 0 ) { first_nonzero = j; }
            if ( digit < 10 )
            {
                digit += '0';
            } else {
                //  offset of 10 because the first hex digit that is a letter
                //  is (normally) A, 10 in decimal.
                digit += h - 10;
            }
        }

        PUTN_ADD_TO_BUFFER ( buffer, written, digit )
    }

putx_join:
    PUTN_ADD_TO_BUFFER ( buffer, written, '\0' )
    puts ( putc, ( char * ) ( buffer + first_nonzero ) );
}

void putu ( some_putc putc, uintmax_t i )
{
    static uintmax_t const m = MAX_DIVIDE_DEC / DIGITS_DEC;

    register uintmax_t a = i, j, k = m;
    register intmax_t  first_nonzero = -1, written = 0;
    //  1 extra for null terminator, no minus sign possible in an unsigned
    //  number.
    signed char        buffer [ MAX_DIGITS_DEC + 1 ];
    signed char        digit;

    PUTN_CHECK_ZERO ( i, buffer, written, first_nonzero, putu_join )

    for ( j = 0; j < MAX_DIGITS_DEC - 1; j++, k /= DIGITS_DEC )
    {
        if ( k == m )
        {
            digit = ( char ) ( a / k );
        } else {
            digit = ( char ) ( ( a % ( DIGITS_DEC * k ) ) / k );
        }

        if ( digit < 0 || digit > 9 )
        {
            digit = '?';
        } else {
            if ( first_nonzero == -1 && digit != 0 ) { first_nonzero = j; }
            digit += '0';
        }

        PUTN_ADD_TO_BUFFER ( buffer, written, digit )
    }
putu_join:
    PUTN_ADD_TO_BUFFER ( buffer, written, '\0' )
    puts ( putc, ( char * ) ( buffer + first_nonzero ) );
}