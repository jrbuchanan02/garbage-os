/**
 * @file efi.h
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief defines the things for EFI. EFI is an extensive standard, so this is
 * an extensive file.
 * @version 1
 * @date 2022-03-23
 * @note complies (or will comply) with the UEFI standard version 2.9
 * @copyright Copyright (C) 2022. Intellectual property of the author(s) listed
 * above.
 *
 */
#ifndef KERNEL_LOADERS_EFI_H
#define KERNEL_LOADERS_EFI_H

#include <metaarch.h>
EFI_FILE_PROLOGUE

#include <kernel/loaders/efi/internal/eficonst.h>
#include <kernel/loaders/efi/internal/efidecl.h>
#include <kernel/loaders/efi/internal/efidef.h>

#define EFI_STRUCT( NAME, EFI )                                                \
  struct __attribute__ ( ( packed ) ) _##NAME;                                 \
  typedef struct _##NAME EFI;

#define EFI_ENUM( NAME, EFI )                                                  \
  enum _##NAME;                                                                \
  typedef enum _##NAME EFI;

#define EFI_CALLBACK( EFI, RET, NAME, ... )                                    \
  typedef RET ( EFIAPI *NAME ) ( __VA_ARGS__ );                                \
  typedef NAME EFI;

// clang-format off
#define STRUCT_PROLOGUE(NAME) typedef struct __attribute__ ( ( packed ) ) _##NAME {
#define STRUCT_EPILOGUE(NAME) } NAME;
#define ENUM_PROLOGUE(NAME) typedef enum _##NAME { 
#define ENUM_EPILOGUE(NAME) } NAME;
// clang-format on

EFI_STRUCT ( EFIGuid, EFI_GUID )

EFI_STRUCT ( EFITableHeader, EFI_TABLE_HEADER )
EFI_STRUCT ( EFISystemTable, EFI_SYSTEM_TABLE )

EFI_STRUCT ( EFIBootServices, EFI_BOOT_SERVICES )
EFI_STRUCT ( EFIRuntimeServices, EFI_RUNTIME_SERVICES )

EFI_STRUCT ( EFIConfigurationTable, EFI_CONFIGURATION_TABLE )

EFI_STRUCT ( EFISimpleTextInputProtocol, EFI_SIMPLE_TEXT_INPUT_PROTOCOL )
EFI_STRUCT ( EFISimpleTextOutputProtocol, EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL )
EFI_STRUCT ( EFIConfigurationTable, EFI_CONFIGURATION_TABLE )

EFI_STRUCT ( EFIRTPropertiesTable, EFI_RT_PROPERTIES_TABLE )
EFI_STRUCT ( EFIPropertiesTable, EFI_PROPERTIES_TABLE )
EFI_STRUCT ( EFIMemoryAttributesTable, EFI_MEMORY_ATTRIBUTES_TABLE )

EFI_STRUCT ( EFIMemoryDescriptor, EFI_MEMORY_DESCRIPTOR )

EFI_STRUCT ( EFIDevicePathProtocol, EFI_DEVICE_PATH_PROTOCOL )

EFI_STRUCT ( EFIOpenProtocolInformationEntry,
             EFI_OPEN_PROTOCOL_INFORMATION_ENTRY )

EFI_STRUCT ( EFIVariableAuthentication3CertID,
             EFI_VARIABLE_AUTHENTICATION_3_CERT_ID )

EFI_STRUCT ( EFIVariableAuthentication, EFI_VARIBLE_AUTHENTICATION )
EFI_STRUCT ( WinCertificate, WIN_CERTIFICATE )
EFI_STRUCT ( WinCertificateUEFIGUID, WIN_CERTIFICATE_UEFI_GUID )
EFI_STRUCT ( EFIVariableAuthentication2, EFI_VARIABLE_AUTHENTICATION_2 )
EFI_STRUCT ( EFIVariableAuthentication3, EFI_VARIABLE_AUTHENTICATION_3 )
EFI_STRUCT ( EFIVariableAuthentication3Nonce,
             EFI_VARIABLE_AUTHENTICATION_3_NONCE )

EFI_STRUCT ( EFITime, EFI_TIME )

EFI_STRUCT ( EFITimeCapabilities, EFI_TIME_CAPABILITIES )

EFI_STRUCT ( EFICapsuleBlockDescriptor, EFI_CAPSULE_BLOCK_DESCRIPTOR )

EFI_STRUCT ( EFICapsuleHeader, EFI_CAPSULE_HEADER )

EFI_STRUCT ( EFICapsuleTable, EFI_CAPSULE_TABLE )

EFI_STRUCT ( EFIMemoryRange, EFI_MEMORY_RANGE )
EFI_STRUCT ( EFIMemoryRangeCapsule, EFI_MEMORY_RANGE_CAPSULE )

EFI_STRUCT ( EFIMemoryRangeCapsuleResult, EFI_MEMORY_RANGE_CAPSULE_RESULT )

EFI_STRUCT ( EFICapsuleResultVariableHeader,
             EFI_CAPSULE_RESULT_VARIABLE_HEADER )

EFI_STRUCT ( EFICapsuleResultVariableFMP, EFI_CAPSULE_RESULT_VARIABLE_FMP )

EFI_STRUCT ( EFICapsuleResultVariableJSON, EFI_CAPSULE_RESULT_VARIABLE_JSON )

EFI_STRUCT ( EFILoadedImageProtocol, EFI_LOADED_IMAGE_PROTOCOL )
EFI_STRUCT ( EFIDevicePathProtocol, EFI_DEVICE_PATH_PROTOCOL )
EFI_STRUCT ( EFIDevicePathUtilitiesProtocol,
             EFI_DEVICE_PATH_UTILITIES_PROTOCOL )
EFI_STRUCT ( EFIDevicePathToTextProtocol, EFI_DEVICE_PATH_TO_TEXT_PROTOCOL )
EFI_STRUCT ( EFIDevicePathFromTextProtoco, EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL )

EFI_ENUM ( EFITimerDelay, EFI_TIMER_DELAY )

EFI_ENUM ( EFIAllocateType, EFI_ALLOCATE_TYPE )
EFI_ENUM ( EFIMemoryType, EFI_MEMORY_TYPE )

EFI_ENUM ( EFIInterfaceType, EFI_INTERFACE_TYPE )

EFI_ENUM ( EFILocateSearchType, EFI_LOCATE_SEARCH_TYPE )

EFI_ENUM ( EFIResetType, EFI_RESET_TYPE )

STRUCT_PROLOGUE ( EFIMemoryDescriptor )
    UINT32               Type;
    EFI_PHYSICAL_ADDRESS PhysicalStart;
    EFI_VIRTUAL_ADDRESS  VirtualStart;
    UINT64               NumberOfPages;
    UINT64               Attribute;
STRUCT_EPILOGUE ( EFIMemoryDescriptor )

/**
 * @brief Globally Unique Identifer.
 * @note What happens when we have to describe something with more than
 * 2^128 possible interpretations?
 */
STRUCT_PROLOGUE ( EFIGuid )
    UINT32 Data1;
    UINT16 Data2;
    UINT16 Data3;
    UINT8  Data4 [ 8 ];
STRUCT_EPILOGUE ( EFIGuid )

STRUCT_PROLOGUE ( EFITime )
    UINT16 Year;      //  1900 - 9999
    UINT8  Minth;     //  1 - 12
    UINT8  Day;       //  1 - 31
    UINT8  Hour;      //  1 - 23
    UINT8  Minute;    //  1 - 59
    UINT8  Second;    //  1 - 59
    UINT8  Pad1;
    UINT32 Nanosecond;    //  1 - 999999999
    UINT16 TimeZone;      //  -1440 - 1440 or 2047
    UINT8  Daylight;
    UINT8  Pad2;
STRUCT_EPILOGUE ( EFITime )

STRUCT_PROLOGUE ( EFITimeCapabilities )
    UINT32  Resolution;
    UINT32  Accuracy;
    BOOLEAN SetsToZero;
STRUCT_EPILOGUE ( EFI_TIME_CAPABILITIES )

STRUCT_PROLOGUE ( EFICapsuleBlockDescriptor )
    UINT64 Length;
    union
    {
        EFI_PHYSICAL_ADDRESS DataBlock;
        EFI_PHYSICAL_ADDRESS ContinuationPointer;
    } Union;
STRUCT_EPILOGUE ( EFICapsuleBlockDescriptor )

STRUCT_PROLOGUE ( EFICapsuleHeader )
    EFI_GUID CapsuleGuid;
    UINT32   HeaderSize;
    UINT32   Flag;
    UINT32   CapsuleImageSize;
STRUCT_EPILOGUE ( EFICapsuleHeader )

STRUCT_PROLOGUE ( EFICapsuleTable )
    UINT32 CapsuleArrayNumber;
    VOID  *CapsulePtr [];    //  note UEFI defines this as CapsulePtr[1], but
                             //  also as a variable length array
STRUCT_EPILOGUE ( EFICapsuleTable )

STRUCT_PROLOGUE ( EFIMemoryRange )
    EFI_PHYSICAL_ADDRESS Address;
    UINT64               Length;
STRUCT_EPILOGUE ( EFIMemoryRange )

STRUCT_PROLOGUE ( EFIMemoryRangeCapsule )
    EFI_CAPSULE_HEADER Header;
    UINT32             OsRequestedMemoryType;
    UINT64             NumberOfMemoryRanges;
    EFI_MEMORY_RANGE   MemoryRanges [];
STRUCT_EPILOGUE ( EFIMemoryRangeCapsule )

STRUCT_PROLOGUE ( EFIMemoryRangeCapsuleResult )
    UINT64 FirmwareMemoryRequirement;
    UINT64 NumberOfMemoryRanges;
STRUCT_EPILOGUE ( EFIMemoryRangeCapsuleResult )

STRUCT_PROLOGUE ( EFICapsuleResultVariableHeader )
    UINT32     VariableTotalSize;
    UINT32     Reserved;    //  UEFI says this is for alignment.
    EFI_GUID   CapsuleGuid;
    EFI_TIME   CapsuleProcessed;
    EFI_STATUS CapsuleStatus;
STRUCT_EPILOGUE ( EFICapsuleResultVariableHeader )

STRUCT_PROLOGUE ( EFICapsuleResultVariableFMP )
    UINT16   Version;
    UINT8    PayloadIndex;
    UINT8    UpdateImageIndex;
    EFI_GUID UpdateImageTypeId;
    CHAR16   CapsuleFileName [];
    //  While EFI has both commented out, gcc gives up after the second
    //  variable- length array. Remember that both strings are null terminated.
    //  CHAR16   CapsuleTarget [];
STRUCT_EPILOGUE ( EFICapsuleResultVariableFMP )

STRUCT_PROLOGUE ( EFICapsuleResultVariableJSON )
    UINT32 Version;
    UINT32 CapsuleId;
    UINT32 RespLength;
    UINT8  Resp [];
STRUCT_EPILOGUE ( EFICapsuleResultVariableJSON )

/**
 * @brief A table header in the EFI standard. Each header has enough
 * information to tell us if the data structure is valid and where the data
 * starts.
 *
 */
STRUCT_PROLOGUE ( EFITableHeader )
    UINT64 Signature;
    UINT32 Revision;
    UINT32 HeaderSize;
    UINT32 CRC32;
    UINT32 Reserved;
STRUCT_EPILOGUE ( EFITableHeader )
/**
 * @brief The EFI System table.
 *
 */
STRUCT_PROLOGUE ( EFISystemTable )
    EFI_TABLE_HEADER                 Hdr;
    CHAR16                          *FirmwareVendor;
    UINT32                           FirmwareRevision;
    EFI_HANDLE                       ConsoleInHandle;
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL  *ConIn;
    EFI_HANDLE                       ConsoleOutHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
    EFI_HANDLE                       StandardErrorHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *StdErr;
    EFI_RUNTIME_SERVICES            *RuntimeServices;
    EFI_BOOT_SERVICES               *BootServices;
    UINTN                            NumberOfTableEntries;
    EFI_CONFIGURATION_TABLE         *ConfigurationTable;
STRUCT_EPILOGUE ( EFISystemTable )

//  callbacks for the boot services table
//  NOTE: Returns the previous TPL. Must be matched with a RestoreTPL call.
EFI_CALLBACK ( EFI_RAISE_TPL, EFI_TPL, EFIRaiseTPL, IN EFI_TPL )
//  NOTE: Must be matched with a RaiseTPL call
EFI_CALLBACK ( EFI_RESTORE_TPL, VOID, EFIRestoreTPL, IN EFI_TPL )

EFI_CALLBACK ( EFI_ALLOCATE_PAGES,
               EFI_STATUS,
               EFIAllocatePages,
               IN EFI_ALLOCATE_TYPE,
               IN EFI_MEMORY_TYPE,
               IN UINTN,
               IN OUT EFI_PHYSICAL_ADDRESS * )
EFI_CALLBACK ( EFI_FREE_PAGES,
               EFI_STATUS,
               EFIFreePages,
               IN EFI_PHYSICAL_ADDRESS,
               IN UINTN )
EFI_CALLBACK ( EFI_GET_MEMORY_MAP,
               EFI_STATUS,
               EFIGetMemoryMap,
               IN OUT UINTN *,
               OUT EFI_MEMORY_DESCRIPTOR *,
               OUT UINTN *,
               OUT UINTN *,
               OUT UINT32 * )
EFI_CALLBACK ( EFI_ALLOCATE_POOL,
               EFI_STATUS,
               EFIAllocatePool,
               IN EFI_MEMORY_TYPE,
               IN UINTN,
               OUT VOID ** )
EFI_CALLBACK ( EFI_FREE_POOL, EFI_STATUS, EFIFreePool, IN VOID * )

//  callback for the event notify
EFI_CALLBACK ( EFI_EVENT_NOTIFY, VOID, EFIEventNotify, IN EFI_EVENT, IN VOID * )

EFI_CALLBACK ( EFI_CREATE_EVENT,
               EFI_STATUS,
               EFICreateEvent,
               IN UINT32,
               IN EFI_TPL,
               IN EFI_EVENT_NOTIFY OPTIONAL,
               IN VOID *OPTIONAL,
               OUT EFI_EVENT * )
EFI_CALLBACK ( EFI_SET_TIMER,
               EFI_STATUS,
               EFISetTimer,
               IN EFI_EVENT,
               IN EFI_TIMER_DELAY,
               IN UINT64 )
EFI_CALLBACK ( EFI_WAIT_FOR_EVENT,
               EFI_STATUS,
               EFIWaitForEvent,
               IN UINTN,
               IN EFI_EVENT *,
               OUT UINTN * )
EFI_CALLBACK ( EFI_SIGNAL_EVENT, EFI_STATUS, EFISignalEvent, IN EFI_EVENT )
EFI_CALLBACK ( EFI_CLOSE_EVENT, EFI_STATUS, EFICloseEvent, IN EFI_EVENT )
//  NOTE: return value of EFI_NOT_READY means event is not in the signaled
//  state, but does not mean that the function errored out.
EFI_CALLBACK ( EFI_CHECK_EVENT, EFI_STATUS, EFICheckEvent, IN EFI_EVENT )

EFI_CALLBACK ( EFI_INSTALL_PROTOCOL_INTERFACE,
               EFI_STATUS,
               EFIInstallProtocolInterface,
               IN OUT EFI_HANDLE *,
               IN EFI_GUID *,
               IN EFI_INTERFACE_TYPE,
               IN VOID * )
EFI_CALLBACK ( EFI_REINSTALL_PROTOCOL_INTERFACE,
               EFI_STATUS,
               EFIReinstallProtocolInterface,
               IN EFI_HANDLE,
               IN EFI_GUID *,
               IN VOID *,
               IN VOID * )
EFI_CALLBACK ( EFI_UNINSTALL_PROTOCOL_INTERFACE,
               EFI_STATUS,
               EFIUninstallProtocolInterface,
               IN EFI_HANDLE,
               IN EFI_GUID *,
               IN VOID * )
//  NOTE: should probably be called "HandlesProtocol" since it asks the
//  handle if it supports the protocol.
EFI_CALLBACK ( EFI_HANDLE_PROTOCOL,
               EFI_STATUS,
               EFIHandleProtocol,
               IN EFI_HANDLE,
               IN EFI_GUID *,
               OUT VOID ** )
EFI_CALLBACK ( EFI_REGISTER_PROTOCOL_NOTIFY,
               EFI_STATUS,
               EFIRegisterProtocolNotify,
               IN EFI_GUID *,
               IN EFI_EVENT,
               OUT VOID ** )
EFI_CALLBACK ( EFI_LOCATE_HANDLE,
               EFI_STATUS,
               EFILocateHandle,
               IN EFI_LOCATE_SEARCH_TYPE,
               IN EFI_GUID *OPTIONAL,
               IN VOID *OPTIONAL,
               IN OUT UINTN *,
               OUT EFI_HANDLE * )
EFI_CALLBACK ( EFI_LOCATE_DEVICE_PATH,
               EFI_STATUS,
               EFILocateDevicePath,
               IN EFI_GUID *,
               IN OUT EFI_DEVICE_PATH_PROTOCOL **,
               OUT EFI_HANDLE * )
EFI_CALLBACK ( EFI_INSTALL_CONFIGURATION_TABLE,
               EFI_STATUS,
               EFIInstallConfigurationTable,
               IN EFI_GUID *,
               IN VOID * )

EFI_CALLBACK ( EFI_IMAGE_LOAD,
               EFI_STATUS,
               EFIImageLoad,
               IN BOOLEAN,
               IN EFI_HANDLE,
               IN EFI_DEVICE_PATH_PROTOCOL *,
               IN VOID *OPTIONAL,
               IN UINTN,
               OUT EFI_HANDLE * )
EFI_CALLBACK ( EFI_IMAGE_START,
               EFI_STATUS,
               EFIImageStart,
               IN EFI_HANDLE,
               OUT UINTN *,
               OUT CHAR16 **OPTIONAL )
EFI_CALLBACK ( EFI_EXIT,
               EFI_STATUS,
               EFIExit,
               IN EFI_HANDLE,
               IN EFI_STATUS,
               IN UINTN,
               IN CHAR16 *OPTIONAL )
EFI_CALLBACK ( EFI_IMAGE_UNLOAD, EFI_STATUS, EFIImageUnload, IN EFI_HANDLE )
EFI_CALLBACK ( EFI_EXIT_BOOT_SERVICES,
               EFI_STATUS,
               EFIExitBootServices,
               IN EFI_HANDLE,
               IN UINTN )

EFI_CALLBACK ( EFI_GET_NEXT_MONOTONIC_COUNT,
               EFI_STATUS,
               EFIGetNextMonotonicCount,
               OUT UINT64 * )
EFI_CALLBACK ( EFI_STALL, EFI_STATUS, EFIStall, IN UINTN )
EFI_CALLBACK ( EFI_SET_WATCHDOG_TIMER,
               EFI_STATUS,
               EFISetWatchdogTimer,
               IN UINTN,
               IN UINT64,
               IN UINTN,
               IN CHAR16 *OPTIONAL )

EFI_CALLBACK ( EFI_CONNECT_CONTROLLER,
               EFI_STATUS,
               EFIConnectController,
               IN EFI_HANDLE,
               IN EFI_HANDLE *OPTIONAL,
               IN EFI_DEVICE_PATH_PROTOCOL *OPTIONAL,
               IN BOOLEAN )
EFI_CALLBACK ( EFI_DISCONNECT_CONTROLLER,
               EFI_STATUS,
               EFIDisconnectController,
               IN EFI_HANDLE,
               IN EFI_HANDLE OPTIONAL,
               IN EFI_HANDLE OPTIONAL )

EFI_CALLBACK ( EFI_OPEN_PROTOCOL,
               EFI_STATUS,
               EFIOpenProtocol,
               IN EFI_HANDLE,
               IN EFI_GUID *,
               OUT VOID **OPTIONAL,
               IN EFI_HANDLE,
               IN EFI_HANDLE,
               IN UINT32 )
EFI_CALLBACK ( EFI_CLOSE_PROTOCOL,
               EFI_STATUS,
               EFICloseProtocol,
               IN EFI_HANDLE,
               IN EFI_GUID *,
               IN EFI_HANDLE,
               IN EFI_HANDLE )
EFI_CALLBACK ( EFI_OPEN_PROTOCOL_INFORMATION,
               EFI_STATUS,
               EFIOpenProtocolInformation,
               IN EFI_HANDLE,
               IN EFI_GUID *,
               OUT EFI_OPEN_PROTOCOL_INFORMATION_ENTRY **,
               OUT UINTN * )

EFI_CALLBACK ( EFI_PROTOCOLS_PER_HANDLE,
               EFI_STATUS,
               EFIProtocolsPerHandle,
               IN EFI_HANDLE,
               OUT EFI_GUID ***,
               OUT UINTN * )
EFI_CALLBACK ( EFI_LOCATE_HANDLE_BUFFER,
               EFI_STATUS,
               EFILocateHandleBuffer,
               IN EFI_LOCATE_SEARCH_TYPE,
               IN EFI_GUID *OPTIONAL,
               IN VOID *OPTIONAL,
               OUT UINTN *,
               OUT EFI_HANDLE ** )
EFI_CALLBACK ( EFI_LOCATE_PROTOCOL, VOID, EFILocateProtocol )
EFI_CALLBACK ( EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES,
               VOID,
               EFIInstallMultipleProtocolInterfaces,
               IN OUT EFI_HANDLE *,
               ... )
EFI_CALLBACK ( EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES,
               VOID,
               EFIUninstallMultipleProtocolInterfaces,
               IN EFI_HANDLE,
               ... )

EFI_CALLBACK ( EFI_CALCULATE_CRC32,
               EFI_STATUS,
               EFICalculateCRC32,
               IN VOID *,
               IN UINTN,
               OUT UINT32 * )

EFI_CALLBACK ( EFI_COPY_MEM, VOID, EFICopyMemm, IN VOID *, IN VOID *, IN UINTN )
EFI_CALLBACK ( EFI_SET_MEM, VOID, EFISetMem, IN VOID *, IN UINTN, IN UINT8 )

EFI_CALLBACK ( EFI_CREATE_EVENT_EX,
               EFI_STATUS,
               EFICreateEventEx,
               IN UINT32,
               IN EFI_TPL,
               IN EFI_EVENT_NOTIFY OPTIONAL,
               IN CONST VOID *OPTIONAL,
               IN CONST EFI_GUID *OPTIONAL,
               OUT EFI_EVENT * )

/**
 * @brief The Boot Services
 *
 */
STRUCT_PROLOGUE ( EFIBootServices )
    EFI_TABLE_HEADER                 Hdr;
    //  Task Priority Services (SINCE 1.0)
    EFI_RAISE_TPL                    RaiseTPL;
    EFI_RESTORE_TPL                  RestoreTPL;
    //  Memory Services (SINCE 1.0)
    EFI_ALLOCATE_PAGES               AllocatePages;
    EFI_FREE_PAGES                   FreePages;
    EFI_GET_MEMORY_MAP               GetMemoryMap;
    EFI_ALLOCATE_POOL                AllocatePool;
    EFI_FREE_POOL                    FreePool;
    //  Event & Timer Services (SINCE 1.0)
    EFI_CREATE_EVENT                 CreateEvent;
    EFI_SET_TIMER                    SetTimer;
    EFI_WAIT_FOR_EVENT               WaitForEvent;
    EFI_SIGNAL_EVENT                 SignalEvent;
    EFI_CLOSE_EVENT                  CloseEvent;
    EFI_CHECK_EVENT                  CheckEvent;
    //  protocol handler services (SINCE 1.0)
    EFI_INSTALL_PROTOCOL_INTERFACE   InstallProtocolInterface;
    EFI_REINSTALL_PROTOCOL_INTERFACE ReinstallProtocolInterface;
    EFI_UNINSTALL_PROTOCOL_INTERFACE UninstallProtocolInterface;
    EFI_HANDLE_PROTOCOL              HandleProtocol;
    VOID                            *Reserved;
    EFI_REGISTER_PROTOCOL_NOTIFY     RegisterProtocoLNotify;
    EFI_LOCATE_HANDLE                LocateHandle;
    EFI_LOCATE_DEVICE_PATH           LocateDevicePath;
    EFI_INSTALL_CONFIGURATION_TABLE  InstallConfigurationTable;
    //  Image Services (SINCE 1.0)
    EFI_IMAGE_LOAD                   LoadImage;
    EFI_IMAGE_START                  StartImage;
    EFI_EXIT                         Exit;
    EFI_IMAGE_UNLOAD                 UnloadImage;
    EFI_EXIT_BOOT_SERVICES           ExitBootServices;
    //  EFI Miscellaneous Services (SINCE 1.0)
    EFI_GET_NEXT_MONOTONIC_COUNT     GetNextMonotonicCount;
    EFI_STALL                        Stall;
    EFI_SET_WATCHDOG_TIMER           SetWatchdogTimer;
    //  DriverSupport Services (SINCE 1.1)
    EFI_CONNECT_CONTROLLER           ConnectController;
    EFI_DISCONNECT_CONTROLLER        DisconnectController;
    //  Open and Close Protocol Services (SINCE 1.1)
    EFI_OPEN_PROTOCOL                OpenProtocol;
    EFI_CLOSE_PROTOCOL               CloseProtocol;
    EFI_OPEN_PROTOCOL_INFORMATION    OpenProtocolInformation;
    //  Library Services (SINCE 1.1)
    EFI_PROTOCOLS_PER_HANDLE         ProtocolsPerHandle;
    EFI_LOCATE_HANDLE_BUFFER         LocateHandleBuffer;
    EFI_LOCATE_PROTOCOL              LocateProtocol;
    EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES
    InstallMultipleProtocolInterfaces;
    EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES
    UninstallMultipleProtocolInterfaces;
    //  32-Bit CRC Services (SINCE 1.1)
    EFI_CALCULATE_CRC32 CalculateCrc32;
    //  Miscellaneous Services (SINCE 1.1)
    EFI_COPY_MEM        CopyMem;
    EFI_SET_MEM         SetMem;
    //  Miscellaneous Services (SINCE 2.0)
    EFI_CREATE_EVENT_EX CreateEventEx;
STRUCT_EPILOGUE ( EFIBootServices )

//  callbacks for the runtime services structure
EFI_CALLBACK ( EFI_GET_TIME,
               EFI_STATUS,
               EFIGetTime,
               OUT EFI_TIME *,
               OUT EFI_TIME_CAPABILITIES *OPTIONAL )
EFI_CALLBACK ( EFI_SET_TIME, EFI_STATUS, EFISetTime, IN EFI_TIME * )
EFI_CALLBACK ( EFI_GET_WAKEUP_TIME,
               EFI_STATUS,
               EFIGetWakeupTime,
               OUT BOOLEAN *,
               OUT BOOLEAN *,
               OUT EFI_TIME * )
EFI_CALLBACK ( EFI_SET_WAKEUP_TIME,
               EFI_STATUS,
               EFISetWakeupTime,
               IN BOOLEAN,
               IN EFI_TIME *OPTIONAL )

EFI_CALLBACK ( EFI_SET_VIRTUAL_ADDRESS_MAP,
               EFI_STATUS,
               EFISetVirtualAddressMap,
               IN UINTN,
               IN UINTN,
               IN UINT32,
               IN EFI_MEMORY_DESCRIPTOR * )
EFI_CALLBACK ( EFI_CONVERT_POINTER,
               EFI_STATUS,
               EFIConvertPointer,
               IN UINTN,
               IN VOID ** )

EFI_CALLBACK ( EFI_GET_VARIABLE,
               EFI_STATUS,
               EFIGetVariable,
               IN CHAR16 *,
               IN EFI_GUID *,
               OUT UINT32 *OPTIONAL,
               IN OUT UINTN *,
               OUT VOID *OPTIONAL )
EFI_CALLBACK ( EFI_GET_NEXT_VARIABLE_NAME,
               EFI_STATUS,
               EFIGetNextVariableName,
               IN OUT UINTN *,
               IN OUT CHAR16 *,
               IN OUT EFI_GUID * )
EFI_CALLBACK ( EFI_SET_VARIABLE,
               EFI_STATUS,
               EFISetVariable,
               IN CHAR16 *,
               IN EFI_GUID *,
               IN UINT32,
               IN UINTN,
               IN VOID * )

EFI_CALLBACK ( EFI_GET_NEXT_HIGH_MONO_COUNT,
               EFI_STATUS,
               EFIGetNextHighMonotonicCount,
               OUT UINT32 * )
EFI_CALLBACK ( EFI_RESET_SYSTEM,
               VOID,
               EFIResetSystem,
               IN EFI_RESET_TYPE,
               IN EFI_STATUS,
               IN UINTN,
               IN VOID *OPTIONAL )

EFI_CALLBACK ( EFI_UPDATE_CAPSULE,
               EFI_STATUS,
               EFIUpdateCapsule,
               IN EFI_CAPSULE_HEADER **,
               IN UINTN,
               IN EFI_PHYSICAL_ADDRESS OPTIONAL )
EFI_CALLBACK ( EFI_QUERY_CAPSULE_CAPABILITIES,
               EFI_STATUS,
               EFIQueryCapsuleCapabilities,
               IN EFI_CAPSULE_HEADER **,
               IN UINTN,
               OUT UINT64 *,
               OUT EFI_RESET_TYPE * )

EFI_CALLBACK ( EFI_QUERY_VARIABLE_INFO,
               EFI_STATUS,
               EFIQueryVariableInfo,
               IN UINT32,
               OUT UINT64 *,
               OUT UINT64 *,
               OUT UINT64 * )

/**
 * @brief The Runtime Services
 * @warning Not all runtime services are available after exiting boot
 * services. The information on which services are unavailable after exiting
 * boot is stored within one of the configuration tables.
 */
STRUCT_PROLOGUE ( EFIRuntimeServices )
    EFI_TABLE_HEADER               Hdr;
    //  Time Services (SINCE 1.0)
    EFI_GET_TIME                   GetTime;
    EFI_SET_TIME                   SetTime;
    EFI_GET_WAKEUP_TIME            GetWakeupTime;
    EFI_SET_WAKEUP_TIME            SetWakeupTime;
    //  Memory-mapping Services (SINCE 1.0)
    EFI_SET_VIRTUAL_ADDRESS_MAP    SetVirtualAddressMap;
    EFI_CONVERT_POINTER            ConvertPointer;
    //  Variable Services (SINCE 1.0)
    EFI_GET_VARIABLE               GetVariable;
    EFI_GET_NEXT_VARIABLE_NAME     GetNextVariableName;
    EFI_SET_VARIABLE               SetVariable;
    //  Miscellaneous Services (SINCE 1.0)
    EFI_GET_NEXT_HIGH_MONO_COUNT   GetNextHighMonotonicCount;
    EFI_RESET_SYSTEM               ResetSystem;
    //  Capsule Services (SINCE 2.0)
    EFI_UPDATE_CAPSULE             UpdateCapsule;
    EFI_QUERY_CAPSULE_CAPABILITIES QueryCapsuleCapabilities;
    //  Miscellaneous (SINCE 2.0)
    EFI_QUERY_VARIABLE_INFO        QueryVariableInfo;
STRUCT_EPILOGUE ( EFIRuntimeServices )

/**
 * @brief A Configuration table
 *
 */
STRUCT_PROLOGUE ( EFIConfigurationTable )
    EFI_GUID VendorGuid;
    VOID    *VendorTable;
STRUCT_EPILOGUE ( EFIConfigurationTable )

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///  Protocol Interfaces begin here!!
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

STRUCT_PROLOGUE ( EFILoadedImageProtocol )
    UINT32                    Revision;
    EFI_HANDLE                ParentHandle;
    EFI_SYSTEM_TABLE         *SystemTable;
    //  source location
    EFI_HANDLE                DeviceHandle;
    EFI_DEVICE_PATH_PROTOCOL *FilePath;
    VOID                     *Reserved;
    //  load options
    UINT32                    LoadOptionsSize;
    VOID                     *LoadOptions;
    //  Location where image is loaded
    VOID                     *ImageBase;
    UINT64                    ImageSize;
    EFI_MEMORY_TYPE           ImageCodeType;
    EFI_MEMORY_TYPE           ImageDataType;
    EFI_IMAGE_UNLOAD          Unload;
STRUCT_EPILOGUE ( EFILoadedImageProtocol )

STRUCT_PROLOGUE ( EFIDevicePathProtocol )
    UINT8 Type;
    UINT8 SubType;
    UINT8 Length [ 2 ];
    UINT8 Bytes [];    //  amount of bytes with length specified in Length
STRUCT_EPILOGUE ( EFIDevicePathPtorocol )

EFI_CALLBACK ( EFI_DEVICE_PATH_UTILS_GET_DEVICE_PATH_SIZE,
               UINTN,
               EFIDevicePathUtilsGetDevicePathSize,
               IN EFI_DEVICE_PATH_PROTOCOL CONST * )
EFI_CALLBACK ( EFI_DEVICE_PATH_UTILS_DUP_DEVICE_PATH,
               EFI_DEVICE_PATH_PROTOCOL *,
               EFIDevicePathUtilsDuplicateDevicePath,
               IN EFI_DEVICE_PATH_PROTOCOL CONST * )
EFI_CALLBACK ( EFI_DEVICE_PATH_UTILS_APPEND_PATH,
               EFI_DEVICE_PATH_PROTOCOL *,
               EFIDevicePathUtilsAppendDevicePath,
               IN EFI_DEVICE_PATH_PROTOCOL CONST *,
               IN EFI_DEVICE_PATH_PROTOCOL CONST * )
EFI_CALLBACK ( EFI_DEVICE_PATH_UTILS_APPEND_NODE,
               EFI_DEVICE_PATH_PROTOCOL *,
               EFIDevicePathUtilsAppendDeviceNode,
               IN EFI_DEVICE_PATH_PROTOCOL CONST *,
               IN EFI_DEVICE_PATH_PROTOCOL CONST * )
EFI_CALLBACK ( EFI_DEVICE_PATH_UTILS_APPEND_INSTANCE,
               EFI_DEVICE_PATH_PROTOCOL *,
               EFIDevicePathUtilsAppendDevicePathInstance,
               IN EFI_DEVICE_PATH_PROTOCOL CONST *,
               IN EFI_DEVICE_PATH_PROTOCOL CONST * )
EFI_CALLBACK ( EFI_DEVICE_PATH_UTILS_GET_NEXT_INSTANCE,
               EFI_DEVICE_PATH_PROTOCOL *,
               EFIDevicePathUtilsGetNextDevicePathInstance,
               IN OUT EFI_DEVICE_PATH_PROTOCOL **,
               OUT UINTN *OPTIONAL )
EFI_CALLBACK ( EFI_DEVICE_PATH_UTILS_IS_MULTI_INSTANCE,
               BOOLEAN,
               EFIDevicePathUtilsIsDevicePathMultiInstance,
               IN EFI_DEVICE_PATH_PROTOCOL CONST * )
EFI_CALLBACK ( EFI_DEVICE_PATH_UTILS_CREATE_NODE,
               EFI_DEVICE_PATH_PROTOCOL *,
               EFIDevicePathUtilsCreateDeviceNode,
               IN UINT8,
               IN UINT8,
               IN UINT16 )

STRUCT_PROLOGUE ( EFIDevicePathUtilitiesProtocol )
    EFI_DEVICE_PATH_UTILS_GET_DEVICE_PATH_SIZE GetDevicePathSize;
    EFI_DEVICE_PATH_UTILS_DUP_DEVICE_PATH      DuplicateDevicePath;
    EFI_DEVICE_PATH_UTILS_APPEND_PATH          AppendDevicePath;
    EFI_DEVICE_PATH_UTILS_APPEND_NODE          AppendDeviceNode;
    EFI_DEVICE_PATH_UTILS_APPEND_INSTANCE      AppendDevicePathInstance;
    EFI_DEVICE_PATH_UTILS_GET_NEXT_INSTANCE    GetNextDevicePathInstance;
    EFI_DEVICE_PATH_UTILS_IS_MULTI_INSTANCE    IsDevicePathMultiInstance;
    EFI_DEVICE_PATH_UTILS_CREATE_NODE          CreateDeviceNode;
STRUCT_EPILOGUE ( EFIDevicePathUtilitiesProtocol )

EFI_CALLBACK ( EFI_DEVICE_PATH_TO_TEXT_NODE,
               CHAR16 *,
               EFIDevicePathToTextNode,
               IN EFI_DEVICE_PATH_PROTOCOL CONST *,
               IN BOOLEAN,
               IN BOOLEAN )
EFI_CALLBACK ( EFI_DEVICE_PATH_TO_TEXT_PATH,
               CHAR16 *,
               EFIDevicePathToTextPath,
               IN EFI_DEVICE_PATH_PROTOCOL CONST *,
               IN BOOLEAN,
               IN BOOLEAN )

STRUCT_PROLOGUE ( EFIDevicePathToTextProtocol )
    EFI_DEVICE_PATH_TO_TEXT_NODE ConvertDeviceNodeToText;
    EFI_DEVICE_PATH_TO_TEXT_PATH ConvertDevicePathToText;
STRUCT_EPILOGUE ( EFIDevicePathToTextProtocol )

EFI_CALLBACK ( EFI_DEVICE_PATH_FROM_TEXT_NODE,
               EFI_DEVICE_PATH_PROTOCOL *,
               EFIDevicePathFromTextNode,
               IN CHAR16 CONST * )
EFI_CALLBACK ( EFI_DEVICE_PATH_FROM_TEXT_PATH,
               EFI_DEVICE_PATH_PROTOCOL *,
               EFIDevicePathFromTextPath,
               IN CHAR16 CONST * )

STRUCT_PROLOGUE ( EFIDevicePathFomTextProtocol )
    EFI_DEVICE_PATH_FROM_TEXT_NODE ConvertTextToDeviceNode;
    EFI_DEVICE_PATH_FROM_TEXT_PATH ConvertTextToDevicePath;
STRUCT_EPILOGUE ( EFIDevicePathFomTextProtocol )

/**
 * @brief A protocol for simply reading in text.
 *
 */
STRUCT_PROLOGUE ( EFISimpleTextInputProtocol )
STRUCT_EPILOGUE ( EFISimpleTextInputProtocol )

/**
 * @brief A protocol for simply outputting text.
 *
 */
STRUCT_PROLOGUE ( EFISimpleTextOutputProtocol )
STRUCT_EPILOGUE ( EFISimpleTextOutputProtocol )

/**
 * @brief Runtime properties table. Describes what runtime services are
 * suddenly unavailable when we exit boot services. If the service is
 * unsupported and we call it, the service simply returns
 * EFI_UNSUPPORTED.
 *
 */
STRUCT_PROLOGUE ( EFIRTPropertiesTable )
    UINT16 Version;
    UINT16 Length;
    UINT32 RuntimeServicesSupported;
STRUCT_EPILOGUE ( EFIRTPropertiesTable )
/**
 * @brief EFIProperties table is a structure that is deprecated and will
 * be removed from the standard.
 * @note (Judging from the size and nameof the structure) Have I told
 * you the about the tradgedy of Darth Properties Table the wise? They
 * expected him to describe at least 64 properties and potentially
 * have 4.3 billion versions, but instead he only described one bit.
 */
STRUCT_PROLOGUE ( EFIPropertiesTable )
    UINT32 Version;
    UINT32 Length;
    UINT64 MemoryProtectionAttribute;
STRUCT_EPILOGUE ( EFIPropertiesTable )

STRUCT_PROLOGUE ( EFIMemoryAttributesTable )
    UINT32                Version;
    UINT32                NumberOfEntries;
    UINT32                DescriptorSize;
    UINT32                Reserved;
    EFI_MEMORY_DESCRIPTOR Entry [ 0 ];
STRUCT_EPILOGUE ( EFIMemoryAttributesTable )

STRUCT_PROLOGUE ( EFIDevicePathProtocol )
    UINT8 Type;
    UINT8 SubType;
    UINT8 Length [ 2 ];
STRUCT_EPILOGUE ( EFIDevicePathProtocol )

STRUCT_PROLOGUE ( EFIOpenProtocolInformationEntry )
    EFI_HANDLE AgendHandle;
    EFI_HANDLE ControllerHandle;
    UINT32     Attributes;
    UINT32     OpenCount;
STRUCT_EPILOGUE ( EFIOpenProtocolInformationEntry )

STRUCT_PROLOGUE ( EFIVariableAuthentication3CertID )
    UINT8  Type;
    UINT32 IdSize;
    UINT8  Id [ 0 ];
STRUCT_EPILOGUE ( EFIVariableAuthentication3CertID )

STRUCT_PROLOGUE ( WinCertificate )
    UINT32 dwLength;
    UINT16 wRevision;
    UINT16 wCertificateType;
    UINT8  bCertificate [];
STRUCT_EPILOGUE ( WinCertificate )

STRUCT_PROLOGUE ( WinCertificateUEFIGUID )
    WIN_CERTIFICATE Hdr;
    EFI_GUID        CertType;
    UINT8           CertData [];
STRUCT_EPILOGUE ( WinCertificateUEFIGUID )

STRUCT_PROLOGUE ( EFIVariableAuthentication )
    UINT64                    MonotonicCount;
    WIN_CERTIFICATE_UEFI_GUID AuthInfo;
STRUCT_EPILOGUE ( EFIVariableAuthentication )

STRUCT_PROLOGUE ( EFIVariableAuthentication2 )
    EFI_TIME                  TimeStap;
    WIN_CERTIFICATE_UEFI_GUID AuthInfo;
STRUCT_EPILOGUE ( EFIVariableAuthentication2 )

STRUCT_PROLOGUE ( EFIVariableAuthentication3 )
    UINT8  Version;
    UINT8  Type;
    UINT32 MetadataSize;
    UINT32 Flags;
STRUCT_EPILOGUE ( EFIVariableAuthentication3 )

STRUCT_PROLOGUE ( EFIVariableAuthentication3Nonce )
    UINT32 NonceSize;
    UINT8  Nonce [ 0 ];
STRUCT_EPILOGUE ( EFIVariableAuthentication3Nonce )

ENUM_PROLOGUE ( EFITimerDelay )
    TimerCancel,              //  cancel the event's timer
            TimerPeriodic,    //  trigger event timer periodically
            TImerRelative     //  trigger in TriggerTime * 100ns
ENUM_EPILOGUE ( EFITimerDelay )

ENUM_PROLOGUE ( EFIAllocateType )
    AllocateAnyPages, AllocateMaxAddress, AllocateAddress, MaxAllocateType
ENUM_EPILOGUE ( EFIAllocateType )

ENUM_PROLOGUE ( EFIMemoryType )
    EfiReservedMemoryType, EfiLoaderCode, EfiLoaderData, EfiBootServicesCode,
            EfiBootServicesData, EfiRuntimeServicesCode, EfiRuntimeServicesData,
            EfiConventionalMemory, EfiUnusableMemory, EfiACPIReclaimMemory,
            EfiACPIMemoryNVS, EfiMemoryMappedIO, EfiMemoryMappedIOPortSpace,
            EfiPalCode, EfiPersistentMemory, EfiUnacceptedMemoryType,
            EfiMaxMemoryType
ENUM_EPILOGUE ( EFIMemoryType )

ENUM_PROLOGUE ( EFIInterfaceType )
    EFI_NATIVE_INTERFACE
ENUM_EPILOGUE ( EFIInterfaceType )

ENUM_PROLOGUE ( EFILocateSearchType )
    AllHandles, ByRegisterNotify, ByProtocol
ENUM_EPILOGUE ( EFILocateSearchType )

ENUM_PROLOGUE ( EFIResetType )
    EfiResetCold, EfiResetWarm, EfiResetShutdown, EfiResetPlatformSpecific,
ENUM_EPILOGUE ( EFIResetType )
EFI_FILE_EPILOGUE
#endif    //  ifndef KERNEL_LOADERS_EFI_H