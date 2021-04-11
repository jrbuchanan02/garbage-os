/**
 * @file tables.h
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com) (github: @natara1939344)
 * @brief This file will contain the definition and methods to load / create / read 
 * the tables that the processor uses internally: IDT, GDT, and LDT
 * @version 0.1
 * @date 2021-04-11
 * 
 * @copyright Copyright (C) 2021 under the LGPL version 2.1
 * 
 */
#ifndef TABLES_H
#define TABLES_H 1

#include <stdint.h>
#include "../crt/interrupt.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief An interrupt function.
 * @note Must end with the iret opcode!
 */
typedef void (*interrupt_function)();

/**
 * @struct interrupt_t
 * @brief An interrupt
 * @note the function that interrupt_t must return using the 
 * "iret" opcode. While the x86 processor allows 65536 different
 * entries into the interrupt descriptor table, only 256 different
 * interrupts (including reserved ones) can actually be called.
 */
typedef struct {
    uint8_t corresponding_signal;
    interrupt_function calls;
} interrupt_t;

typedef struct {
    uint64_t addresses[1 << 15];
} processor_table_t;

#ifdef __cplusplus
}
#endif
#endif