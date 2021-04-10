/**
 * @file stringstuff.h
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com) (github: @natara1939344)
 * @brief Everything to do with strings.
 * @version 0.1
 * @date 2021-04-10
 * 
 * @copyright Copyright (C) 2021 under the LGPL version 2.1
 * 
 */
#ifndef STRINGSTUFF_H
#define STRINGSTUFF_H 1
#include "../kernel/kmain.h"
#if defined(__cplusplus)
extern "C" {
#endif
#define ASCII_TERMINATOR '\0'
#define UTF08_TERMINATOR '\0'
#define UTF16_TERMINATOR 0x0000

typedef struct
{
    uint64_t count;
    char* strings[];
} split_result_t;

typedef enum
{
    REMOVE,
    KEEP_AT_END,
    KEEP_AT_START,
    KEEP_AS_OWN,
} delimiter_behavior_t;

boolean str_eq(char lhs[], char rhs[]);

uint64_t strlen(char string[]);

char* append(char string[], char append[]);

split_result_t split_along(
    char string[], 
    char delim, 
    delimiter_behavior_t behavior);

split_result_t split_along(
    char string[],
    char delims[],
    delimiter_behavior_t behavior);

split_result_t split_along(
    char string[],
    char delim);

split_result_t split_along(
    char string[],
    char delim[]);

#if defined (__cplusplus)
}
#endif
#endif