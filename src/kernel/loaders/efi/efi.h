/**
 * @file efi.h
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief defines the things for EFI
 * @version 1
 * @date 2022-03-23
 * @note complies (or will comply) with the UEFI standard version 2.9
 * @copyright Copyright (C) 2022. Intellectual property of the author(s) listed above.
 *
 */
#ifndef SOURCE_LOADERS_EFI_H
#define SOURCE_LOADERS_EFI_H

#include <stdint.h>

// uncomment these lines for visual-studio intellisense.
// #define __AARCH64__
// #define __ARM__
// #define __IA32__
// #define __IA64__
// #define __X86_64__
// Immaginary architectures. For use in Visual Studio's intellisense only.
#define __FL_64__ // friendly, Little Endian, 64-bit
// #define __FL_32__ // friendly, Little Endian, 32-bit
// #define __FB_64__ // friendly, Big Endian, 64-bit
// #define __FB_32__ // friendly, Big Endian, 32-bit
// #define __EL_64__ // evil, Little Endian, 64-bit
// #define __EL_32__ // evil, Little Endian, 32-bit
// #define __EB_64__ // evil, Big Endian, 64-bit
// #define __EB_32__ // evil, Big Endian, 32-bit

// arch bits -> suffix of (U)INT to make (U)INTN
// endian -> 0 means little endian
// evil -> 1 means that the architecture is notoriously difficult to develop a
// compiler for or that the assembly code is notoriously difficult to read and
// understand.
#if defined(__AARCH64__)
#define ARCH_BITS 64
#define ENDIAN 0
#define EVIL 0
#elif defined(__ARM__)
#define ARCH_BITS 32
#define ENDIAN 0
#define EVIL 0
#elif defined(__IA32__)
#define ARCH_BITS 32
#define ENDIAN 0
#define EVIL 0
#elif defined(__IA64__)
#define ARCH_BITS 64
#define ENDIAN 0
#define EVIL 1
#elif defined(__X86_64__)
#define ARCH_BITS 64
#define ENDIAN 0
#define EVIL 0
#elif defined(__FL_64__)
#define ARCH_BITS 64
#define ENDIAN 0
#define EVIL 0
#elif defined(__FL_32__)
#define ARCH_BITS 32
#define ENDIAN 0
#define EVIL 0
#elif defined(__FB_64__)
#define ARCH_BITS 64
#define ENDIAN 1
#define EVIL 0
#elif defined(__FB_32__)
#define ARCH_BITS 32
#define ENDIAN 1
#define EVIL 0
#elif defined(__EL_64__)
#define ARCH_BITS 64
#define ENDIAN 0
#define EVIL 1
#elif defined(__EL_32__)
#define ARCH_BITS 32
#define ENDIAN 0
#define EVIL 1
#elif defined(__EB_64__)
#define ARCH_BITS 64
#define ENDIAN 1
#define EVIL 1
#elif defined(__EB_32__)
#define ARCH_BITS 32
#define ENDIAN 1
#define EVIL 1
#else
#error "Unknown Architecture"
#endif // if defined(__AARCH64__)

#define ___EFI_INT_DEF(N)        \
    typedef uint##N##_t UINT##N; \
    typedef int##N##_t INT##N;

___EFI_INT_DEF(8)
___EFI_INT_DEF(16)
___EFI_INT_DEF(32)
___EFI_INT_DEF(64)

#if ARCH_BITS >= 128

___EFI_INT_DEF(128)

#endif // ARCH_BITS >= 128

#undef ___EFI_INT_DEF

#if ARCH_BITS == 128
typedef UINT128 UINTN;
typedef INT128 INTN;
#elif ARCH_BITS == 64
typedef UINT64 UINTN;
typedef INT64 INTN;
#elif ARCH_BITS == 32
typedef UINT32 UINTN;
typedef INT32 INTN;
#else
#error "Unknown Architecture!"
#endif // if ARCH_BITS == 128

#if defined(BOOLEAN)
#undef BOOLEAN
#endif // if defined(BOOLEAN)

#define BOOLEAN UINT8

#if defined(TRUE)
#undef TRUE
#endif // if defined(TRUE)
#define TRUE ((BOOLEAN)(1))

#if defined(FALSE)
#undef FALSE
#endif // if defined(FALSE)
#define FALSE ((BOOLEAN)(0))

#if defined(CHAR8)
#undef CHAR8
#endif // if defined(CHAR8)
#define CHAR8 char

#if defined(CHAR16)
#undef CHAR16
#endif // if defined(CHAR16)
#define CHAR16 short

#if defined(VOID)
#undef VOID
#endif // if defined(VOID)
#define VOID void

#if defined(EFI_GUID)
#undef EFI_GUID
#endif // if defined(EFI_GUID)
typedef struct
{
    UINT32 first;
    UINT16 middle;
    UINT8 end[8];
} _efiGUID;
#define EFI_GUID _efiGUID

#if defined(EFI_STATUS)
#undef EFI_STATUS
#endif // if defined(EFI_STATUS)
#define EFI_STATUS UINTN

#if defined(EFI_HANDLE)
#undef EFI_HANDLE
#endif // if defined(EFI_HANDLE)
#define EFI_HANDLE VOID *

#if defined(EFI_EVENT)
#undef EFI_EVENT
#endif // if defined(EFI_EVENT)
#define EFI_EVENT VOID *

#if defined(EFI_LBA)
#undef EFI_LBA
#endif // if defined(EFI_LBA)
#define EFI_LBA UINT64

#if defined(EFI_TPL)
#undef EFI_TPL
#endif // if defined(EFI_TPL)
#define EFI_TPL UINTN

#if defined(EFI_MAC_ADDRESS)
#undef EFI_MAC_ADDRESS
#endif // if defined(EFI_MAC_ADDRESS)
#define EFI_MAC_ADDRESS

#if defined(EFI_IPv4_ADDRESS)
#undef EFI_IPv4_ADDRESS
#endif // if defined(EFI_IPv4_ADDRESS)
#define EFI_IPv4_ADDRESS

#if defined(EFI_IPv6_ADDRESS)
#undef EFI_IPv6_ADDRESS
#endif // if defined(EFI_IPv6_ADDRESS)
#define EFI_IPv6_ADDRESS

#if defined(EFI_IP_ADDRESS)
#undef EFI_IP_ADDRESS
#endif // if defined(EFI_IP_ADDRESS)
#define EFI_IP_ADDRESS

// modifiers
#if defined(IN)
#undef IN
#endif // if defined(IN)
#define IN

#if defined(OUT)
#undef OUT
#endif // if defined(OUT)
#define OUT

#if defined(OPTIONAL)
#undef OPTIONAL
#endif // if defined(OPTIONAL)
#define OPTIONAL

#if defined(CONST)
#undef CONST
#endif // if defined CONST
#define CONST const

// calling convention things
#define EFIAPI __attribute__((ms_abi))

// signatures and other numerical constants we should know
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma region "Constants and Signatures"
#pragma GCC diagnostic pop

#define EFI_SYSTEM_TABLE_SIGNATURE 0x5453595320494249
#define EFI_2_90_SYSTEM_TABLE_REVISION ((2 << 16) | (90))
#define EFI_2_80_SYSTEM_TABLE_REVISION ((2 << 16) | (80))
#define EFI_2_70_SYSTEM_TABLE_REVISION ((2 << 16) | (70))
#define EFI_2_60_SYSTEM_TABLE_REVISION ((2 << 16) | (60))
#define EFI_2_50_SYSTEM_TABLE_REVISION ((2 << 16) | (50))
#define EFI_2_40_SYSTEM_TABLE_REVISION ((2 << 16) | (40))
#define EFI_2_31_SYSTEM_TABLE_REVISION ((2 << 16) | (31))
#define EFI_2_30_SYSTEM_TABLE_REVISION ((2 << 16) | (30))
#define EFI_2_20_SYSTEM_TABLE_REVISION ((2 << 16) | (20))
#define EFI_2_10_SYSTEM_TABLE_REVISION ((2 << 16) | (10))
#define EFI_2_00_SYSTEM_TABLE_REVISION ((2 << 16) | (00))
#define EFI_1_10_SYSTEM_TABLE_REVISION ((1 << 16) | (10))
#define EFI_1_02_SYSTEM_TABLE_REVISION ((1 << 16) | (02))
#define EFI_SPECIFICATION_VERSION EFI_SYSTEM_TABLE_REVISION
#define EFI_SYSTEM_TABLE_REVISION EFI_2_90_SYSTEM_TABLE_REVISION

#define EFI_BOOT_SERVICES_SIGNATURE 0x56524553544f4f42
#define EFI_BOOT_SERVICES_REVISION EFI_SPECIFICATION_VERSION

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma endregion "Constants and Signatures"
#pragma GCC diagnostic pop

// defines for structures. These defines are kinda like typedefs, but instead
// we can have a callback and a struct defined similarly.

#if defined(EFI_TABLE_HEADER)
#undef EFI_TABLE_HEADER
#endif // if defined(EFI_TABLE_HEADER)
struct EFITableHeader;
#define EFI_TABLE_HEADER struct EFITableHeader

#if defined(EFI_SYSTEM_TABLE)
#undef EFI_SYSTEM_TABLE
#endif // if defined(EFI_SYSTEM_TABLE)
struct EFISystemTable;
#define EFI_SYSTEM_TABLE struct EFISystemTable

#if defined(EFI_SIMPLE_TEXT_INPUT_PROTOCOL)
#undef EFI_SIMPLE_TEXT_INPUT_PROTOCOL
#endif // if defined(EFI_SIMPLE_TEXT_INPUT_PROTOCOL)
struct EFISimpleTextInputProtocol;
#define EFI_SIMPLE_TEXT_INPUT_PROTOCOL struct EFISimpleTextInputProtocol

#if defined(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL)
#undef EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL
#endif // if defined(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL)
struct EFISimpleTextOutputProtocol;
#define EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL struct EFISimpleTextOutputProtocol

#if defined(EFI_BOOT_SERVICES)
#undef EFI_BOOT_SERVICES
#endif // if defined(EFI_BOOT_SERVICES)
struct EFIBootServices;
#define EFI_BOOT_SERVICES struct EFIBootServices

#if defined(EFI_RUNTIME_SERVICES)
#undef EFI_RUNTIME_SERVICES
#endif // if defined(EFI_RUNTIME_SERVICES)
struct EFIRuntimeServices;
#define EFI_RUNTIME_SERVICES struct EFIRuntimeServices

#if defined(EFI_CONFIGURATION_TABLE)
#undef EFI_CONFIGURATION_TABLE
#endif // if defined(EFI_CONFIGURATION_TABLE)
struct EFIConfigurationTable;
#define EFI_CONFIGURATION_TABLE struct EFIConfigurationTable

#if defined(EFI_RAISE_TPL)
#undef EFI_RAISE_TPL
#endif // if defined(EFI_RAISE_TPL)
/*EFIRaiseTPL typedef goes here.*/
#define EFI_RAISE_TPL EFIRaiseTPL

#if defined(EFI_RESTORE_TPL)
#undef EFI_RESTORE_TPL
#endif // if defined(EFI_RESTORE_TPL)
/*EFIRestoreTPL typedef goes here.*/
#define EFI_RESTORE_TPL EFIRestoreTPL

#if defined(EFI_ALLOCATE_PAGES)
#undef EFI_ALLOCATE_PAGES
#endif // if defined(EFI_ALLOCATE_PAGES)
/*EFIAllocatePages typedef goes here.*/
#define EFI_ALLOCATE_PAGES EFIAllocatePages

/**
 * @brief EFI-defined Table Header.
 *
 */
struct EFITableHeader
{
    /**
     * @brief Signature of the table. Known signatures are defined above in the
     * header.
     *
     */
    UINT64 Signature;
    /**
     * @brief Number corresponding to the revision of this table.
     *
     */
    UINT32 Revision;
    /**
     * @brief Equal to what sizeof(struct EFITableHeader) is if we have an
     * identical table header to the one defined by the standard. since the
     * header is, in fact, a header, this field is important since it tells us
     * the offset of the data in the table.
     *
     */
    UINT32 HeaderSize;
    /**
     * @brief CRC32 for either the header or the  whole table (I forget which)
     *
     */
    UINT32 CRC32;
    /**
     * @brief Reserved. Don't touch it!
     *
     */
    UINT32 Reserved;
};

/**
 * @brief The System Table. Not only does this need to be preserved for any
 * driver we load, but it's also given to us in a glorious transfer of control
 * by the BIOS.
 *
 */
struct EFISystemTable
{
    /**
     * @brief The header.
     *
     */
    EFI_TABLE_HEADER Hdr;
    /**
     * @brief The name of the vendor. Intel's IA-32 and x86-64 specific headers
     * define this name as L"Intel", but I think other names are possible :P
     *
     */
    CHAR16 *FirmwareVendor;
    /**
     * @brief Revision of the firmware.
     *
     */
    UINT32 FirmwareRevision;
    /**
     * @brief (Probs a C++ class) The magic memory address that allows the
     * EFISimpleTextInputProtocol to read numbers from the user.
     * @note The standard requires this and ConIn to be present and not able to
     * crash the system even when there is no console input.
     */
    EFI_HANDLE ConsoleInHandle;
    /**
     * @brief The input protocol.
     * @note The standard requires this and ConsoleInHandle to be present and not able to
     * crash the system even when there is no console input.
     */
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL *ConIn;
    EFI_HANDLE ConsoleOutHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
    EFI_HANDLE StandardErrorHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *StdErr;
    EFI_RUNTIME_SERVICES *RuntimeServices;
    EFI_BOOT_SERVICES *BootServices;
    UINTN NumberOfTableEntries;
    EFI_CONFIGURATION_TABLE *ConfigurationTable;
};

struct EFIBootServices
{
    EFI_TABLE_HEADER Hdr;

    // SINCE 1.02 (earliest known revision)
};

#endif // ifndef SOURCE_LOADERS_EFI_H