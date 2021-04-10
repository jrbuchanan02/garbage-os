/**
 * \file kmain.h
 * \author Joshua Buchanan
 * \brief stuff for the kmain.c file.
 */

#include "bootboot.h"

#ifndef KMAIN_H
#define KMAIN_H 1

#define TRUE 1
#define FALSE 0

typedef uint8_t boolean;

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
extern BOOTBOOT bootboot;
/**
 * @brief environment, from the environment file specified by the configuration file
 * passed to mkbootimg
 */
extern unsigned char environment[4096];
/**
 * @brief framebuffer.
 */
extern uint8_t framebuffer;


/**
 * @brief parses a null terminated string for a uint64_t encoded via decimal.
 * 
 * @param string the null terminated string to parse
 * @return uint64_t value found
 */
uint64_t parse_uint64_from_string(unsigned char* string)
{
    static const int8_t difference = 0 - '0';
    uint64_t output;
    boolean started = FALSE;

    uint64_t index = 0;

    while (*string) {
        if (*string < '0' || *string > '9') {
            if (started) {return output;}
            else {index++;continue;}
        }
        else {
            output *= 10;
            output += *string + difference;
            started = TRUE;
            index++;
        }
    }


    return output;
}

/**
 * @brief Constructs an environment_t from the environemnt as passed by the 
 * bootboot loader.
 * 
 * if the operating system is compiled with _DEBUG or DEBUG defined, the 
 * environment \b always sets the debug mode to true. 
 *  
 * 
 * @param env the text specified by the environment.
 * @return environment_t the constructed environment structure.
 */
environment_t parse_configuration_file(unsigned char env[4096])
{
    // current version has configuration information as listed:
    // - kernel location
    // - major version
    // - minor version
    // - fixed version
    // - build version
    // - version type (string)
    // - debug mode (y/N)

    environment_t output;
    static const unsigned char yes = 'y';
    static const unsigned char terminator = '\n';

    static const unsigned char* kernel_key = "kernel";
    static const unsigned char* major_version_key = "major_version";
    static const unsigned char* minor_version_key = "minor_version";
    static const unsigned char* fixed_version_key = "fixed_version";
    static const unsigned char* build_version_key = "build_version";
    static const unsigned char* typed_version_key = "typed_version";
    static const unsigned char* debug_mode_key = "debug_mode";

    for (uint16_t i = 0; i < 4096; i++) {
        if (!strcmp(&env[i], kernel_key)) {
            continue;
        }
        else if (!strcmp(&env[i], major_version_key)) {
            i += 15; // length of major version key + 1
            output.major_version = parse_uint64_from_string(&env[i]);
            continue;
        }
        else if (!strcmp(&env[i], minor_version_key)) {
            i += 15;
            output.minor_version = parse_uint64_from_string(&env[i]);
            continue;
        }
        else if (!strcmp(&env[i], fixed_version_key)) {
            i += 15;
            output.fixed_version = parse_uint64_from_string(&env[i]);
            continue;
        }
        else if (!strcmp(&env[i], build_version_key)) {
            i += 15;
            output.fixed_version = parse_uint64_from_string(&env[i]);
            continue;
        }
        else if (!strcmp(&env[i], typed_version_key)) {
            i += 15;
            int j;
            for (j = 0; env[i + j] != '\n'; j++)
                output.version_type[j] = env[i + j];
            continue;
        }
        else if (!strcmp(&env[i], debug_mode_key)) {
            i += 12;
            if (&env[i] == yes) {
                output.debug_mode = 1;
            }
        }
    }
#if defined(_DEBUG)
    output.debug_mode = 1;
#elif defined(DEBUG)
    output.debug_mode = 1;
#endif
    return output;
}

#ifdef __cplusplus
extern "C" {
#else



#endif // ifdef __cplusplus + else
#ifdef __cplusplus
}
#endif // ifdef __cplusplus
#endif // KMAIN_H