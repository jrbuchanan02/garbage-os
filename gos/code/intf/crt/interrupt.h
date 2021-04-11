/**
 * @file interrupt.h
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com) (github: @natara1939344)
 * @brief "API" to define / call interrupts
 * @version 0.1
 * @date 2021-04-11
 * 
 * @copyright Copyright (C) 2021 under the LGPL version 2.1
 * 
 */

#ifndef INTERRUPT_H
#define INTERRUPT_H 1

#include "stdint.h"

#define B(x) (uint8_t)x
// TODO: Add more interrupts and the reserved exceptions
#define INTERRUPT_FAIL_FAST B(0xff)


#endif