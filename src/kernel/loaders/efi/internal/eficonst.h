/**
 * @file eficonst.h
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Constants in the EFI standard.
 * @version 1
 * @date 2022-03-25
 *
 * @copyright Copyright (C) 2022. Intellectual property of the author(s) listed
 * above.
 *
 */
#ifndef SOURCE_KERNEL_LOADERS_EFI_INTERNAL_EFICONST_H
#define SOURCE_KERNEL_LOADERS_EFI_INTERNAL_EFICONST_H

//  system table signature and the table revisions per version of the EFI
//  standard.
#define EFI_SYSTEM_TABLE_SIGNATURE     0x5453595320494249
#define EFI_2_90_SYSTEM_TABLE_REVISION ( ( 2 << 16 ) | ( 90 ) )
#define EFI_2_80_SYSTEM_TABLE_REVISION ( ( 2 << 16 ) | ( 80 ) )
#define EFI_2_70_SYSTEM_TABLE_REVISION ( ( 2 << 16 ) | ( 70 ) )
#define EFI_2_60_SYSTEM_TABLE_REVISION ( ( 2 << 16 ) | ( 60 ) )
#define EFI_2_50_SYSTEM_TABLE_REVISION ( ( 2 << 16 ) | ( 50 ) )
#define EFI_2_40_SYSTEM_TABLE_REVISION ( ( 2 << 16 ) | ( 40 ) )
#define EFI_2_31_SYSTEM_TABLE_REVISION ( ( 2 << 16 ) | ( 31 ) )
#define EFI_2_30_SYSTEM_TABLE_REVISION ( ( 2 << 16 ) | ( 30 ) )
#define EFI_2_20_SYSTEM_TABLE_REVISION ( ( 2 << 16 ) | ( 20 ) )
#define EFI_2_10_SYSTEM_TABLE_REVISION ( ( 2 << 16 ) | ( 10 ) )
#define EFI_2_00_SYSTEM_TABLE_REVISION ( ( 2 << 16 ) | ( 00 ) )
#define EFI_1_10_SYSTEM_TABLE_REVISION ( ( 1 << 16 ) | ( 10 ) )
#define EFI_1_02_SYSTEM_TABLE_REVISION ( ( 1 << 16 ) | ( 02 ) )
#define EFI_SPECIFICATION_VERSION      EFI_SYSTEM_TABLE_REVISION
#define EFI_SYSTEM_TABLE_REVISION      EFI_2_90_SYSTEM_TABLE_REVISION

//  boot services table signature and revision
#define EFI_BOOT_SERVICES_SIGNATURE 0x56524553544f4f42
#define EFI_BOOT_SERVICES_REVISION  EFI_SPECIFICATION_VERSION

//  runtime services table signature and revision
#define EFI_RUNTIME_SERVICES_SIGNATURE 0x56524553544e5552
#define EFI_RUNTIME_SERVICES_REVISION  EFI_SPECIFICATION_VERSION

//  industry standard GUIDs

#define ACPI_TABLE_GUID                                                        \
  {                                                                            \
    0xeb9d2d30, 0x2d88, 0x11d3,                                                \
    {                                                                          \
      0x9a, 0x16, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d                           \
    }                                                                          \
  }
#define SAL_SYSTEM_TABLE_GUID                                                  \
  {                                                                            \
    0xeb9d2d32, 0x2d88, 0x11d3,                                                \
    {                                                                          \
      0x9a, 0x16, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d                           \
    }                                                                          \
  }
#define SMBIOS_TABLE_GUID                                                      \
  {                                                                            \
    0xeb9d2d31, 0x2d88, 0x11d3,                                                \
    {                                                                          \
      0x9a, 0x16, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d                           \
    }                                                                          \
  }
#define SMBIOS3_TABLE_GUID                                                     \
  {                                                                            \
    0xf2fd1544, 0x9794, 0x4a2c,                                                \
    {                                                                          \
      0x99, 0x2e, 0xe5, 0xbb, 0xcf, 0x20, 0xe3, 0x94                           \
    }                                                                          \
  }
#define MPS_TABLE_GUID                                                         \
  {                                                                            \
    0xeb9d2d2f, 0x2d88, 0x11d3,                                                \
    {                                                                          \
      0x9a, 0x16, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d                           \
    }                                                                          \
  }
//
//  ACPI 2.0 or newer tables should use EFI_ACPI_TABLE_GUID
//
#define EFI_ACPI_TABLE_GUID                                                    \
  {                                                                            \
    0x8868e871, 0xe4f1, 0x11d3,                                                \
    {                                                                          \
      0xbc, 0x22, 0x00, 0x80, 0xc7, 0x3c, 0x88, 0x81                           \
    }                                                                          \
  }
#define EFI_ACPI_20_TABLE_GUID EFI_ACPI_TABLE_GUID
#define ACPI_TABLE_GUID                                                        \
  {                                                                            \
    0xeb9d2d30, 0x2d88, 0x11d3,                                                \
    {                                                                          \
      0x9a, 0x16, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d                           \
    }                                                                          \
  }
#define ACPI_10_TABLE_GUID ACPI_TABLE_GUID
//  JSON Configuration tables
#define EFI_JSON_CONFIG_DATA_TABLE_GUID                                        \
  {                                                                            \
    0x87367f87, 0x1119, 0x41ce,                                                \
    {                                                                          \
      0xaa, 0xec, 0x8b, 0xe0, 0x11, 0x1f, 0x55, 0x8a                           \
    }                                                                          \
  }
#define EFI_JSON_CAPSULE_DATA_TABLE_GUID                                       \
  {                                                                            \
    0x35e7a725, 0x8dd2, 0x4cac,                                                \
    {                                                                          \
      0x80, 0x11, 0x33, 0xcd, 0xa8, 0x10, 0x90, 0x56                           \
    }                                                                          \
  }
#define EFI_JSON_CAPSULE_RESULT_TABLE_GUID                                     \
  {                                                                            \
    0xdbc461c3, 0xb3de, 0x422a,                                                \
    {                                                                          \
      0xb9, 0xb4, 0x98, 0x86, 0xfd, 0x49, 0xa1, 0xe5                           \
    }                                                                          \
  }
//  devicetree table
#define EFI_DTB_TABLE_GUID                                                     \
  {                                                                            \
    0xb1b621d5, 0xf19c, 0x41a5,                                                \
    {                                                                          \
      0x83, 0x0b, 0xd9, 0x15, 0x2c, 0x69, 0xaa, 0xe0                           \
    }                                                                          \
  }
//  runtime properties
#define EFI_RT_PROPERTIES_TABLE_GUID                                           \
  {                                                                            \
    0xeb66918a, 0x7eef, 0x402a,                                                \
    {                                                                          \
      0x84, 0x2e, 0x93, 0x1d, 0x21, 0xc3, 0x8a, 0xe9                           \
    }                                                                          \
  }

#define EFI_RT_PROPERTIES_TABLE_VERSION 0x1
#define EFI_RT_PROPERTIES_TABLE_LENGTH  0x8

#define EFI_RT_SUPPORTED_GET_TIME                      0x0001
#define EFI_RT_SUPPORTED_SET_TIME                      0x0002
#define EFI_RT_SUPPORTED_GET_WAKEUP_TIME               0x0004
#define EFI_RT_SUPPORTED_SET_WAKEUP_TIME               0x0008
#define EFI_RT_SUPPORTED_GET_VARIABLE                  0x0010
#define EFI_RT_SUPPORTED_GET_NEXT_VARIABLE_NAME        0x0020
#define EFI_RT_SUPPORTED_SET_VARIABLE                  0x0040
#define EFI_RT_SUPPORTED_SET_VIRTUAL_ADDRESS_MAP       0x0080
#define EFI_RT_SUPPORTED_CONVERT_POINTER               0x0100
#define EFI_RT_SUPPORTED_GET_NEXT_HIGH_MONOTONIC_COUNT 0x0200
#define EFI_RT_SUPPORTED_RESET_SYSTEM                  0x0400
#define EFI_RT_SUPPORTED_UPDATE_CAPSULE                0x0800
#define EFI_RT_SUPPORTED_QUERY_CAPSULE_CAPABILITIES    0x1000
#define EFI_RT_SUPPORTED_QUERY_VARIABLE_INFO           0x2000

//  EFI Properties Table
#define EFI_PROPERTIES_TABLE_VERSION                                    0x00010000
//  the only bit to have ever been used in the table.
#define EFI_PROPERTIES_RUNTIME_MEMORY_PROTECTION_NON_EXECUTABLE_PE_DATA 0x1

//  memory attributes table
#define EFI_MEMORY_ATTRIBUTES_TABLE_GUID                                       \
  {                                                                            \
    0xdcfa911d, 0x26eb, 0x469f,                                                \
    {                                                                          \
      0xa2, 0x20, 0x38, 0xb7, 0xdc, 0x46, 0x12, 0x20                           \
    }                                                                          \
  }
#define EFI_MEMORY_ATTRIBUTES_CURR_VER 1

#define EVT_TIMER                         0x80000000
#define EVT_RUNTIME                       0x40000000
#define EVT_NOTIFY_WAIT                   0x00000100
#define EVT_NOTIFY_SIGNAL                 0x00000200
#define EVT_SIGNAL_EXIT_BOOT_SERVICES     0x00000201
#define EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE 0x60000202

//  predefined EFI Event Group GUIDs
#define EFI_EVENT_GROUP_EXIT_BOOT_SERVICES                                     \
  {                                                                            \
    0x27abf055, 0xb1b8, 0x4c26, 0x80, 0x48, 0x74, 0x8f, 0x37, 0xba, 0xa2, 0xdf \
  }                                                                            \
  }
#define EFI_EVENT_GROUP_BEFORE_EXIT_BOOT_SERVICES                              \
  {                                                                            \
    0x8be0e274, 0x3970, 0x4b44,                                                \
    {                                                                          \
      0x80, 0xc5, 0x1a, 0xb9, 0x50, 0x2f, 0x3b, 0xfc                           \
    }                                                                          \
  }
#define EFI_EVENT_GROUP_VIRTUAL_ADDRESS_CHANGE                                 \
  {                                                                            \
    0x13fa7698, 0xc831, 0x49c7,                                                \
    {                                                                          \
      0x87, 0xea, 0x8f, 0x43, 0xfc, 0xc2, 0x51, 0x96                           \
    }                                                                          \
  }
#define EFI_EVENT_GROUP_MEMORY_MAP_CHANGE                                      \
  {                                                                            \
    0x78bee926, 0x692f, 0x48fd,                                                \
    {                                                                          \
      0x9e, 0xdb, 0x1, 0x42, 0x2e, 0xf0, 0xd7, 0xab                            \
    }                                                                          \
  }
#define EFI_EVENT_GROUP_READY_TO_BOOT                                          \
  {                                                                            \
    0x7ce88fb3, 0x4bd7, 0x4679,                                                \
    {                                                                          \
      0x87, 0xa8, 0xa8, 0xd8, 0xde, 0xe5, 0xd, 0x2b                            \
    }                                                                          \
  }
#define EFI_EVENT_GROUP_AFTER_READY_TO_BOOT                                    \
  {                                                                            \
    0x3a2a00ad, 0x98b9, 0x4cdf,                                                \
    {                                                                          \
      0xa4, 0x78, 0x70, 0x27, 0x77, 0xf1, 0xc1, 0xb                            \
    }                                                                          \
  }
#define EFI_EVENT_GROUP_RESET_SYSTEM                                           \
  {                                                                            \
    0x62da6a56, 0x13fb, 0x485a,                                                \
    {                                                                          \
      0xa8, 0xda, 0xa3, 0xdd, 0x79, 0x12, 0xcb, 0x6b                           \
    }                                                                          \
  }

#define TPL_APPLICATION 4
#define TPL_CALLBACK    8
#define TPL_NOTIFY      16
#define TPL_HIGH_LEVEL  31

#define EFI_MEMORY_UC            0x0000000000000001
#define EFI_MEMORY_WC            0x0000000000000002
#define EFI_MEMORY_WT            0x0000000000000004
#define EFI_MEMORY_WB            0x0000000000000008
#define EFI_MEMORY_UCE           0x0000000000000010
#define EFI_MEMORY_WP            0x0000000000001000
#define EFI_MEMORY_RP            0x0000000000002000
#define EFI_MEMORY_XP            0x0000000000004000
#define EFI_MEMORY_NV            0x0000000000008000
#define EFI_MEMORY_MORE_RELIABLE 0x0000000000010000
#define EFI_MEMORY_RO            0x0000000000020000
#define EFI_MEMORY_SP            0x0000000000040000
#define EFI_MEMORY_CPU_CRYPTO    0x0000000000080000
#define EFI_MEMORY_RUNTIME       0x8000000000000000

#define EFI_MEMORY_DESCRIPTOR_VERSION 1

#define EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL  0x00000001
#define EFI_OPEN_PROTOCOL_GET_PROTOCOL        0x00000002
#define EFI_OPEN_PROTOCOL_TEST_PROTOCOL       0x00000004
#define EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER 0x00000008
#define EFI_OPEN_PROTOCOL_BY_DRIVER           0x00000010
#define EFI_OPEN_PROTOCOL_EXCLUSIVE           0x00000020

//  variable attributes
#define EFI_VARIABLE_NON_VOLATILE                          0x00000001
#define EFI_VARIABLE_BOOTSERVICE_ACCESS                    0x00000002
#define EFI_VARIABLE_RUNTIME_ACCESS                        0x00000004
#define EFI_VARIABLE_HARDWARE_ERROR_RECORD                 0x00000008
//  this is deprecated
#define EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS            0x00000010
#define EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS 0x00000020
#define EFI_VARIABLE_APPEND_WRITE                          0x00000040
#define EFI_VARIABLE_ENHANCED_AUTHENTICATED_ACCESS         0x00000080

#define EFI_VARIABLE_AUTHENTICATION_3_CERT_ID_SHA256 1

#define EFI_VARIABLE_AUTHENTICATION_3_TIMESTAMP_TYPE 1
#define EFI_VARIABLE_AUTHENTICATION_3_NONCE_TYPE     2

#endif    //  ifndef SOURCE_KERNEL_LOADERS_EFI_INTERNAL_EFICONST_H