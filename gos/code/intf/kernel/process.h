/**
 * @file process.h
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com) (github: @natara1939344)
 * @brief This file will help manage processes and task switching.
 * @version 0.1
 * @date 2021-04-11
 * 
 * @copyright Copyright (C) 2021 under the LGPL version 2.1
 * 
 */
#ifndef PROCESS_H
#define PROCESS_H 1

#include "stdint.h"

#define ULTIMATE_TASK_PRIORITY          0
#define KERNEL_TASK_RIORITY             1
#define HIGH_TASK_PRIORITY              2
#define DEFAULT_TASK_PRIORITY           3
#define LOW_TASK_PRIORITY               4

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Generic information about processes
 * @note incomplete.
 */
typedef struct {
    uint64_t process_id;
    uint64_t process_priority = 0xffffFFFFffffFFFLLU;
} process_t;

#ifdef __cplusplus
}
#endif
#endif