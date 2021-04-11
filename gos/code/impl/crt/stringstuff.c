/**
 * @file stringstuff.c
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com) (github: @natara1939344)
 * @brief Implementation of all things string!
 * @version 0.1
 * @date 2021-04-10
 * 
 * @copyright Copyright (C) 2021 under the LGPL version 2.1
 * 
 */

#include "../../intf/crt/stringstuff.h"

/**
 * @brief indicates whether two strings are equal
 * 
 * @param lhs left hand side
 * @param rhs right hand side
 * @return TRUE when lhs and rhs are idendical
 * @return FALSE when lhs and rhs are not identical.
 */
boolean str_eq(char lhs[], char rhs[]) {
    uint64_t index = 0LU;
    while (lhs[index] && rhs[index]) {
        if (lhs[index] != rhs[index])
            return FALSE;
        index++;
    }
    if (lhs[index] == UTF08_TERMINATOR && rhs[index] == UTF08_TERMINATOR)
        return TRUE;
    else
        return FALSE;
}

/**
 * @brief Parses string until finding a null character
 * 
 * @param string the string to parse - literally a character array
 * @return uint64_t the length of string
 */
uint64_t strlen(char string[]) {
    uint64_t output = 0;
    for (;string[output]; output++);
    return output;
}

