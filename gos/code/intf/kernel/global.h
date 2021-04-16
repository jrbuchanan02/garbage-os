/**
 * @file global.h
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com) (github: @natara1939344)
 * @brief contains the virtual addresses that the linker passes to the kernel's binary
 * @version 0.1
 * @date 2021-04-15
 * 
 * @copyright Copyright (C) 2021 under the LGPL 2.1
 * 
 */

#ifndef GLOBAL_H
#define GLOBAL_H 1

#include <stdint.h>

#ifndef MSVC // my c_cpp_properties.json thinks MSVC is defined. but its not.
#include <efi.h>
#endif
// recreate the definitions in the efi-libraries to make the scary red squiggily lines go away!
#if defined(___VSCODE___) && defined(MSVC)

#ifndef BOOLEAN
#define BOOLEAN uint8_t
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

typedef uint32_t (*LOCATE_HANDLE)(uint32_t, EFI_GUID*, void**, size_t*, void**);
typedef struct {
    EFI_BOOT_SERVICES *BootServices;
    EFI_RUNTIME_SERVICES *RuntimeServices;
} EFI_SYSTEM_TABLE;
typedef struct {
    LOCATE_HANDLE LocateHandle;
} EFI_BOOT_SERVICES;

typedef struct {
    uint8_t some_byte;
} EFI_HANDLE;

typedef struct {
    uint32_t one;
    uint32_t two;
    uint32_t three;
    uint32_t four;
    
} EFI_GUID;

typedef struct {
    uint8_t some_byte;
} EFI_RUNTIME_SERVICES;


typedef struct {
    uint8_t some_byte;
} EFI_SERIAL_IO_PROTOCOL;


#define SERIAL_IO_PROTOCOL {0, 0, 0, 0}

#endif

// recreate the definitions in bootboot.h to mak the scary red squiggily lines go away!
#if defined(___VSCODE___) && defined(MSVC)


typedef struct {
    uint32_t fb_scanline;
    uint32_t fb_width;
    uint32_t fb_height;

    // visual studio code didn't like typedef here?
    // this code exists only to drive away the scary red squggly lines.
    // it is not even compiled (eliminated during preprocessing) 
    union {
        struct {
            uint64_t efi_ptr;
        } x86_64;
        struct {
            uint64_t efi_ptr;
        } aarch64;
    } arch;
} BOOTBOOT;

#endif

#include "bootboot.h"

#ifdef __cplusplus
extern "C" {
#endif


extern volatile BOOTBOOT bootboot;
extern volatile unsigned char environment[4096];
extern volatile uint8_t fb;
extern volatile uint8_t mmio;


#ifdef __cplusplus
}
#endif
#endif