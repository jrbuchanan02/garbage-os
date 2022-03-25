/**
 * @file efidef.h
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Defines for the basic types in EFI
 * @version 1
 * @date 2022-03-25
 *
 * @copyright Copyright (C) 2022. Intellectual property of the author(s) listed above.
 *
 */
#ifndef SOURCE_KERNEL_LOADERS_EFI_INTERNAL_EFIDEF_H
#define SOURCE_KERNEL_LOADERS_EFI_INTERNAL_EFIDEF_H

#include <metaarch.h>

#include <stdint.h>

#define BOOLEAN UINT8
#define TRUE 1
#define FALSE 0

#define __INTDEF(N)            \
    typedef int##N##_t INT##N; \
    typedef uint##N##_t UINT##N;

#define __INTREF(N) INT##N
#define __UINTREF(N) UINT##N

__INTDEF(8)
__INTDEF(16)
__INTDEF(32)
__INTDEF(64)
typedef __int128 INT128;
typedef unsigned __int128 UINT128;

#if SIZE_BITS == 128
typedef __INTREF(128) INTN;
typedef __UINTREF(128) UINTN;
#elif SIZE_BITS == 64
typedef __INTREF(64) INTN;
typedef __UINTREF(64) UINTN;
#elif SIZE_BITS == 32
typedef __INTREF(32) INTN;
typedef __UINTREF(32) UINTN;
#elif SIZE_BITS == 16
typedef __INTREF(16) INTN;
typedef __UINTREF(16) UINTN;
#elif SIZE_BITS == 8
typedef __INTREF(8) INTN;
typedef __UINTREF(8) UINTN;
#else
#error "Bad size bits value!"
#endif // if SIZE_BITS == 128

typedef void VOID;

typedef char CHAR8;
typedef short CHAR16;

typedef VOID *EFI_HANDLE;

typedef UINTN EFI_STATUS;

#endif // ifndef SOURCE_KERNEL_LOADERS_EFI_INTERNAL_EFIDEF_H