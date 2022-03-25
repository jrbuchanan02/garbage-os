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

#include <kernel/loaders/efi/internal/eficonst.h>
#include <kernel/loaders/efi/internal/efidecl.h>
#include <kernel/loaders/efi/internal/efidef.h>

#define EFI_STRUCT(NAME, EFI) \
    struct _##NAME;           \
    typedef struct _##NAME EFI;

#define EFI_CALLBACK(EFI, RET, NAME, ...)   \
    typedef RET(EFIAPI *NAME)(__VA_ARGS__); \
    typedef NAME EFI;

#define STRUCT_PROLOGUE(NAME) \
    typedef struct _##NAME    \
    {
#define STRUCT_EPILOGUE(NAME) \
    }                         \
    NAME;

EFI_STRUCT(EFITableHeader, EFI_TABLE_HEADER)
EFI_STRUCT(EFISystemTable, EFI_SYSTEM_TABLE)

EFI_STRUCT(EFIBootServices, EFI_BOOT_SERVICES)
EFI_STRUCT(EFIRuntimeServices, EFI_RUNTIME_SERVICES)

EFI_STRUCT(EFIConfigurationTable, EFI_CONFIGURATION_TABLE)

EFI_STRUCT(EFISimpleTextInputProtocol, EFI_SIMPLE_TEXT_INPUT_PROTOCOL)
EFI_STRUCT(EFISimpleTextOutputProtocol, EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL)
EFI_STRUCT(EFIConfigurationTable, EFI_CONFIGURATION_TABLE)
/**
 * @brief A table header in the EFI standard. Each header has enough information
 * to tell us if the data structure is valid and where the data starts.
 *
 */
STRUCT_PROLOGUE(EFITableHeader)
UINT64 Signature;
UINT32 Revision;
UINT32 HeaderSize;
UINT32 CRC32;
UINT32 Reserved;
STRUCT_EPILOGUE(EFITableHeader)
/**
 * @brief The EFI System table.
 *
 */
STRUCT_PROLOGUE(EFISystemTable)
EFI_TABLE_HEADER Hdr;
CHAR16 *FirmwareVendor;
UINT32 FirmwareRevision;
EFI_HANDLE ConsoleInHandle;
EFI_SIMPLE_TEXT_INPUT_PROTOCOL *ConIn;
EFI_HANDLE ConsoleOutHandle;
EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
EFI_HANDLE StandardErrorHandle;
EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *StdErr;
EFI_RUNTIME_SERVICES *RuntimeServices;
EFI_BOOT_SERVICES *BootServices;
UINTN NumberOfTableEntries;
EFI_CONFIGURATION_TABLE *ConfigurationTable;
STRUCT_EPILOGUE(EFISystemTable)

// callbacks for the boot services table
EFI_CALLBACK(EFI_RAISE_TPL, VOID, EFIRaiseTPL)
EFI_CALLBACK(EFI_RESTORE_TPL, VOID, EFIRestoreTPL)

EFI_CALLBACK(EFI_ALLOCATE_PAGES, VOID, EFIAllocatePages)
EFI_CALLBACK(EFI_FREE_PAGES, VOID, EFIFreePages)
EFI_CALLBACK(EFI_GET_MEMORY_MAP, VOID, EFIGetMemoryMap)
EFI_CALLBACK(EFI_ALLOCATE_POOL, VOID, EFIAllocatePool)
EFI_CALLBACK(EFI_FREE_POOL, VOID, EFIFreePool)

EFI_CALLBACK(EFI_CREATE_EVENT, VOID, EFICreateEvent)
EFI_CALLBACK(EFI_SET_TIMER,  VOID, EFISetTimer)
EFI_CALLBACK(EFI_WAIT_FOR_EVENT, VOID, EFIWaitForEvent)
EFI_CALLBACK(EFI_SIGNAL_EVENT, VOID, EFISignalEvent)
EFI_CALLBACK(EFI_CLOSE_EVENT, VOID, EFICloseEvent)
EFI_CALLBACK(EFI_CHECK_EVENT, VOID, EFICheckEvent)
/**
 * @brief The Boot Services
 *
 */
STRUCT_PROLOGUE(EFIBootServices)
EFI_TABLE_HEADER Hdr;
// Task Priority Services (SINCE 1.0)
EFI_RAISE_TPL RaiseTPL;
EFI_RESTORE_TPL RestoreTPL;
// Memory Services (SINCE 1.0)
EFI_ALLOCATE_PAGES AllocatePages;
EFI_FREE_PAGES FreePages;
EFI_GET_MEMORY_MAP GetMemoryMap;
EFI_ALLOCATE_POOL AllocatePool;
EFI_FREE_POOL FreePool;
// Event & Timer Services (SINCE 1.0)
EFI_CREATE_EVENT CreateEvent;
EFI_SET_TIMER SetTimer;
EFI_WAIT_FOR_EVENT WaitForEvent;
EFI_SIGNAL_EVENT SignalEvent;
EFI_CLOSE_EVENT CloseEvent;
EFI_CHECK_EVENT CheckEvent;
// protocol handler services
STRUCT_EPILOGUE(EFIBootServices)

/**
 * @brief The Runtime Services
 * @warning Not all runtime services are available after exiting boot services.
 * The information on which services are unavailable after exiting boot is
 * stored within one of the configuration tables.
 */
STRUCT_PROLOGUE(EFIRuntimeServices)

STRUCT_EPILOGUE(EFIRuntimeServices)

/**
 * @brief A configuration table.
 *
 */
STRUCT_PROLOGUE(EFIConfigurationTable)
STRUCT_EPILOGUE(EFIConfigurationTable)

/**
 * @brief A protocol for simply reading in text.
 *
 */
STRUCT_PROLOGUE(EFISimpleTextInputProtocol)
STRUCT_EPILOGUE(EFISimpleTextInputProtocol)

/**
 * @brief A protocol for simply outputting text.
 *
 */
STRUCT_PROLOGUE(EFISimpleTextOutputProtocol)
STRUCT_EPILOGUE(EFISimpleTextOutputProtocol)
#endif // ifndef SOURCE_LOADERS_EFI_H