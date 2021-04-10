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

uint64_t strlen(char string[]) {
    uint64_t output = 0;
    for (;string[output]; output++);
    return output;
}

char* append(char string[], char append[]) {
    char* out = malloc(strlen(string) + strlen(append) - 1);

    uint64_t outlen = 0;
    uint64_t index;
    for (index = 0; string[index]; index++) {
        out[index] = string[index];
        outlen++;
    }
    // also grab the null terminator
    for (index = 0; append[index - 1]; index++) {
        out[index + outlen] = append[index];
    }
    return out;
}

