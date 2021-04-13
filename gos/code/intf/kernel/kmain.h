/**
 * @file kmain.h
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com) (github: @natara1939344)
 * @brief File for what we include in the kmain.c file
 * @version 0.1
 * @date 2021-04-11
 * 
 * @copyright Copyright (C) 2021 under the LGPL version 2.1
 * 
 */

#include "bootboot.h"
#include "immintrin.h"
#include "stddef.h"
#ifndef KMAIN_H
#define KMAIN_H 1

#define TRUE 1
#define FALSE 0

typedef uint8_t boolean;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Information about the current environment
 * @note version type is changed to become null-terminated
 */
typedef struct {
    uint8_t debug_mode;
    uint64_t major_version;
    uint64_t minor_version;
    uint64_t fixed_version;
    uint64_t build_version;
    unsigned char version_type[];
} environment_t;

environment_t parse_configuration_file(unsigned char env[4096]);

/**
 * @brief bootboot structure
 * 
 */
extern volatile BOOTBOOT bootboot;
/**
 * @brief environment, from the environment file specified by the configuration file
 * passed to mkbootimg
 */
extern unsigned char environment[4096];

extern uint8_t fb;

#ifdef __cplusplus
}
#endif // ifdef __cplusplus
#endif // KMAIN_H