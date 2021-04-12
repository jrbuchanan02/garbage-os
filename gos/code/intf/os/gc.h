/**
 * @file gc.h
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com) (github: @natara1939344)
 * @brief important header stuff for gc.c
 * @version 0.1
 * @date 2021-04-12
 * 
 * @copyright Copyright (C) 2021 under the LGPL version 2.1
 * 
 */
#ifndef GC_H
#define GC_H 1

#include "stdlib.h"
#include "stdint.h"
#include "../../intf/crt/error_codes.h"
#ifdef __cplusplus
extern "C" {
#endif


void gc_monitor();

int main(void);

#ifdef __cplusplus
}
#endif
#endif