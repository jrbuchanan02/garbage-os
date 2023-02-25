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

#include <stdint.h>
EFI_FILE_PROLOGUE

//  EFIAPI defined as the same calling conventions used by the EFI
//  standard.
#if defined( __X64__ ) || defined( __X32__ )
    //  Use the MS ABI for x86
  #define EFIAPI __attribute__ ( ( ms_abi ) )
#elif defined( __A32__ )
    //  Ensure that we're calling functions to interpret them as ARM code
    //  (as opposed to thumb code)
  #define EFIAPI __attribute__ ( ( target ( "arm" ) ) )
#else
  #define EFIAPI
#endif    //  if defined(__X64__) || defined(__X32__)

//  defines (non-GUID)
#define EVT_TIMER                         0x80000000
#define EVT_RUNTIME                       0x40000000
#define EVT_NOTIFY_WAIT                   0x00000100
#define EVT_NOTIFY_SIGNAL                 0x00000200
#define EVT_SIGNAL_EXIT_BOOT_SERVICES     0x00000201
#define EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE 0x00000202

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

//  typedefs

typedef void      *efi_handle;
typedef void      *efi_event;
typedef uintmax_t  efi_status;
typedef uintmax_t  efi_uintn;
typedef efi_uintn  efi_tpl;
typedef uint8_t    efi_uint8;
typedef uint8_t    efi_boolean;
typedef uint16_t   efi_uint16;
typedef uint16_t   efi_char16;
typedef uint32_t   efi_uint32;
typedef uint64_t   efi_uint64;
typedef int32_t    efi_int32;
typedef efi_uint64 efi_physical_address;
typedef efi_uint64 efi_virtual_address;

//  struct declarations
struct _efi_guid;

struct _efi_table_header;
struct _efi_system_table;
struct _efi_runtime_services;
struct _efi_boot_services;
struct _efi_configuration_table;

struct _efi_device_path_protocol;
struct _efi_simple_text_input_protocol;
struct _efi_simple_text_output_protocol;


struct _efi_simple_text_output_mode;

struct _efi_open_protocol_information_entry;

struct _efi_memory_descriptor;

//  enum declarations
enum _efi_timer_delay;

enum _efi_allocate_type;
enum _efi_memory_type;

enum _efi_interface_type;
enum _efi_locate_search_type;
//  struct typedefs
typedef struct _efi_guid efi_guid;

typedef struct _efi_table_header        efi_table_header;
typedef struct _efi_system_table        efi_system_table;
typedef struct _efi_runtime_services    efi_runtime_services;
typedef struct _efi_boot_services       efi_boot_services;
typedef struct _efi_configuration_table efi_configuration_table;

typedef struct _efi_device_path_protocol        efi_device_path_protocol;
typedef struct _efi_simple_text_input_protocol  efi_simple_text_input_protocol;
typedef struct _efi_simple_text_output_protocol efi_simple_text_output_protocol;

typedef struct _efi_simple_text_output_mode simple_text_output_mode;

typedef struct _efi_open_protocol_information_entry
        efi_open_protocol_information_entry;

typedef struct _efi_memory_descriptor efi_memory_descriptor;

typedef struct _efi_graphics_output_protocol efi_graphics_output_protocol;
typedef struct _efi_graphics_output_mode_information
        efi_graphics_output_mode_information;

//  enum typedefs
typedef enum _efi_timer_delay efi_timer_delay;

typedef enum _efi_allocate_type efi_allocate_type;
typedef enum _efi_memory_type   efi_memory_type;

typedef enum _efi_interface_type     efi_interface_type;
typedef enum _efi_locate_search_type efi_locate_search_type;
//  callback defines

//  entry point
typedef efi_status ( EFIAPI *efi_image_entry_point ) ( efi_handle,
                                                       efi_system_table * );

//  events
typedef void ( EFIAPI *efi_event_notify ) ( efi_event, void * );
typedef efi_status ( EFIAPI *efi_create_event ) ( efi_uint32,
                                                  efi_tpl,
                                                  efi_event_notify,
                                                  void *,
                                                  efi_event * );
typedef efi_status ( EFIAPI *efi_create_event_ex ) ( efi_uint32,
                                                     efi_tpl,
                                                     efi_event_notify,
                                                     void const *,
                                                     efi_guid const *,
                                                     efi_event * );
typedef efi_status ( EFIAPI *efi_close_event ) ( efi_event );
typedef efi_status ( EFIAPI *efi_signal_event ) ( efi_event );
typedef efi_status ( EFIAPI *efi_wait_for_event ) ( efi_uintn,
                                                    efi_event *,
                                                    efi_uintn * );
typedef efi_status ( EFIAPI *efi_check_event ) ( efi_event );
typedef efi_status ( EFIAPI *efi_set_timer ) ( efi_event,
                                               efi_timer_delay,
                                               efi_uint64 );

//  task privledge level things
typedef efi_tpl ( EFIAPI *efi_raise_tpl ) ( efi_tpl );
typedef efi_tpl ( EFIAPI *efi_restore_tpl ) ( efi_tpl );

//  memory management services
typedef efi_status ( EFIAPI *efi_allocate_pages ) ( efi_allocate_type,
                                                    efi_memory_type,
                                                    efi_uintn,
                                                    efi_physical_address * );
typedef efi_status ( EFIAPI *efi_free_pages ) ( efi_physical_address,
                                                efi_uintn );
typedef efi_status ( EFIAPI *efi_get_memory_map ) ( efi_uintn *,
                                                    efi_memory_descriptor *,
                                                    efi_uintn *,
                                                    efi_uintn *,
                                                    efi_uint32 * );
typedef efi_status ( EFIAPI *efi_allocate_pool ) ( efi_memory_type,
                                                   efi_uintn,
                                                   void ** );
typedef efi_status ( EFIAPI *efi_free_pool ) ( void * );

//  protocol interface things
typedef efi_status ( EFIAPI *efi_install_protocol_interface ) (
        efi_handle *,
        efi_guid *,
        efi_interface_type,
        void * );
typedef efi_status ( EFIAPI *efi_uninstall_protocol_interface ) ( efi_handle,
                                                                  efi_guid *,
                                                                  void * );
typedef efi_status ( EFIAPI *efi_reinstall_protocol_interface ) ( efi_handle,
                                                                  efi_guid *,
                                                                  void *,
                                                                  void * );
typedef efi_status ( EFIAPI *efi_register_protocol_notify ) ( efi_guid *,
                                                              efi_event,
                                                              void ** );
typedef efi_status ( EFIAPI *efi_locate_handle ) ( efi_locate_search_type,
                                                   efi_guid *,
                                                   void *,
                                                   efi_uintn *,
                                                   efi_handle * );
typedef efi_status ( EFIAPI *efi_handle_protocol ) ( efi_handle,
                                                     efi_guid *,
                                                     void ** );
typedef efi_status ( EFIAPI *efi_locate_device_path ) (
        efi_guid *,
        efi_device_path_protocol **,
        efi_handle * );
typedef efi_status ( EFIAPI *efi_open_protocol ) ( efi_handle,
                                                   efi_guid *,
                                                   void **,
                                                   efi_handle,
                                                   efi_handle,
                                                   efi_uint32 );
typedef efi_status ( EFIAPI *efi_close_protocol ) ( efi_handle,
                                                    efi_guid *,
                                                    efi_handle,
                                                    efi_handle );
typedef efi_status ( EFIAPI *efi_open_protocol_information ) (
        efi_handle,
        efi_guid *,
        efi_open_protocol_information_entry **,
        efi_uintn * );
typedef efi_status ( EFIAPI *efi_connect_controller ) (
        efi_handle,
        efi_handle *,
        efi_device_path_protocol *,
        efi_boolean );
typedef efi_status ( EFIAPI *efi_disconnect_controller ) ( efi_handle,
                                                           efi_handle,
                                                           efi_handle );
typedef efi_status ( EFIAPI *efi_protocols_per_handle ) ( efi_handle,
                                                          efi_guid ***,
                                                          efi_uintn * );
typedef efi_status ( EFIAPI *efi_locate_handle_buffer ) ( efi_locate_search_type,
                                                          efi_guid *,
                                                          void *,
                                                          efi_uintn *,
                                                          efi_handle ** );
typedef efi_status ( EFIAPI *efi_locate_protocol ) ( efi_guid *,
                                                     void *,
                                                     void ** );
typedef efi_status ( EFIAPI *efi_install_multiple_protocol_interfaces ) (
        efi_handle *,
        ... );
typedef efi_status ( EFIAPI *efi_uninstall_multiple_protocol_interfaces ) (
        efi_handle,
        ... );

typedef efi_status ( EFIAPI *efi_image_load ) ( efi_boolean,
                                                efi_handle,
                                                efi_device_path_protocol *,
                                                void *,
                                                efi_uintn,
                                                efi_handle * );
typedef efi_status ( EFIAPI *efi_image_start ) ( efi_handle,
                                                 efi_uintn *,
                                                 efi_char16 ** );
typedef efi_status ( EFIAPI *efi_image_unload ) ( efi_handle );
typedef efi_status ( EFIAPI *efi_exit ) ( efi_handle,
                                          efi_status,
                                          efi_uintn,
                                          efi_char16 * );
typedef efi_status ( EFIAPI *efi_exit_boot_services ) ( efi_handle, efi_uintn );

typedef efi_status ( EFIAPI *efi_set_watchdog_timer ) ( efi_uintn,
                                                        efi_uint64,
                                                        efi_uintn,
                                                        efi_char16 * );
typedef efi_status ( EFIAPI *efi_stall ) ( efi_uintn );

typedef efi_status ( EFIAPI *efi_copy_mem ) ( void *, void *, efi_uintn );
typedef efi_status ( EFIAPI *efi_set_mem ) ( void *, efi_uintn, efi_uint8 );

typedef efi_status ( EFIAPI *efi_get_next_monotonic_count ) ( efi_uint64 * );

typedef efi_status ( EFIAPI *efi_install_configuration_table ) ( efi_guid *,
                                                                 void * );

typedef efi_status ( EFIAPI *efi_calculate_crc32 ) ( void *,
                                                     efi_uintn,
                                                     efi_uint32 * );

typedef efi_status ( EFIAPI *efi_text_reset ) (
        efi_simple_text_output_protocol *,
        efi_boolean );
typedef efi_status ( EFIAPI *efi_text_string ) (
        efi_simple_text_output_protocol *,
        efi_char16 * );
typedef efi_status ( EFIAPI *efi_text_test_string ) (
        efi_simple_text_output_protocol *,
        efi_char16 * );
typedef efi_status ( EFIAPI *efi_text_query_mode ) (
        efi_simple_text_output_protocol *,
        efi_uintn,
        efi_uintn *,
        efi_uintn * );
typedef efi_status ( EFIAPI *efi_text_set_mode ) (
        efi_simple_text_output_protocol *,
        efi_uintn );
typedef efi_status ( EFIAPI *efi_text_clear_screen ) (
        efi_simple_text_output_protocol * );
typedef efi_status ( EFIAPI *efi_text_set_cursor_position ) (
        efi_simple_text_output_protocol *,
        efi_uintn,
        efi_uintn );
typedef efi_status ( EFIAPI *efi_text_enable_cursor ) (
        efi_simple_text_output_protocol *,
        efi_boolean );
//  GUID defines.

#define ACPI_NEW_GUID                                                          \
  {                                                                            \
    0x8868E871, 0xE4F1, 0x11D3,                                                \
    {                                                                          \
      0xBC, 0x22, 0x00, 0x80, 0xC7, 0x3C, 0x88, 0x81                           \
    }                                                                          \
  }
#define ACPI_OLD_GUID                                                          \
  {                                                                            \
    0xEB9D2D30, 0X2D88, 0X11D3,                                                \
    {                                                                          \
      0x9A, 0x16, 0x00, 0x90, 0x27, 0x3F, 0xC1, 0x4D                           \
    }                                                                          \
  }

//  end guid defines

//  struct declarations.

struct _efi_guid
{
    efi_uint32 data_group_1;
    efi_uint16 data_group_2;
    efi_uint16 data_group_3;
    efi_uint8  data_group_4 [ 8 ];
};

/**
 * A table header containing information about the table that contains it.
 */
struct _efi_table_header
{
    efi_uint64 signature;
    efi_uint32 revision;
    efi_uint32 header_size;
    efi_uint32 crc32;
    efi_uint32 reserved;
};

struct _efi_system_table
{
    efi_table_header                 header;
    efi_char16                      *firmware_vendor;
    efi_uint32                       firmware_revision;
    efi_handle                       console_in_handle;
    efi_simple_text_input_protocol  *con_in;
    efi_handle                       console_out_handle;
    efi_simple_text_output_protocol *con_out;
    efi_handle                       standard_error_handle;
    efi_simple_text_output_protocol *std_err;
    efi_runtime_services            *runtime_services;
    efi_boot_services               *boot_services;
    efi_uintn                        number_of_table_entries;
    efi_configuration_table         *configuration_table;
};

struct _efi_runtime_services
{
    efi_table_header header;
};

struct _efi_boot_services
{
    efi_table_header header;

    efi_raise_tpl   raise_tpl;
    efi_restore_tpl restore_tpl;

    efi_allocate_pages allocate_pages;
    efi_free_pages     free_pages;
    efi_get_memory_map get_memory_map;
    efi_allocate_pool  allocate_pool;
    efi_free_pool      free_pool;

    efi_create_event   create_event;
    efi_set_timer      set_timer;
    efi_wait_for_event wait_for_event;
    efi_signal_event   signal_event;
    efi_close_event    close_event;
    efi_check_event    check_event;

    efi_install_protocol_interface   install_protocol_interface;
    efi_reinstall_protocol_interface reinstall_protocol_interface;
    efi_uninstall_protocol_interface uninstall_protocol_interface;
    efi_handle_protocol              handle_protocol;
    void                            *reserved;
    efi_register_protocol_notify     register_protocol_notify;
    efi_locate_handle                locate_handle;
    efi_locate_device_path           locate_device_path;
    efi_install_configuration_table  install_configuration_table;

    efi_image_load         load_image;
    efi_image_start        start_image;
    efi_exit               exit;
    efi_image_unload       unload_image;
    efi_exit_boot_services exit_boot_services;

    efi_get_next_monotonic_count get_next_monotonic_count;
    efi_stall                    stall;
    efi_set_watchdog_timer       set_watchdog_timer;

    efi_connect_controller    connect_controller;
    efi_disconnect_controller disconnect_controller;

    efi_open_protocol             open_protocol;
    efi_close_protocol            close_protocol;
    efi_open_protocol_information open_protocol_information;

    efi_protocols_per_handle protocols_per_handle;
    efi_locate_handle_buffer locate_handle_buffer;
    efi_locate_protocol      locate_protocol;

    efi_install_multiple_protocol_interfaces
            install_multiple_protocol_interfaces;
    efi_uninstall_multiple_protocol_interfaces
            uninstall_multiple_protocol_interfaces;

    efi_calculate_crc32 calculate_crc32;

    efi_copy_mem        copy_mem;
    efi_set_mem         set_mem;
    efi_create_event_ex create_event_ex;
};

struct _efi_memory_descriptor
{
    efi_uint32           type;
    efi_physical_address physical_start;
    efi_virtual_address  virtual_start;
    efi_uint64           number_of_pages;
    efi_uint64           attribute;
};

struct _efi_configuration_table
{
    efi_guid vendor_guid;
    void    *vendor_table;
};

struct _efi_simple_text_output_protocol
{
    efi_text_reset               reset;
    efi_text_string              output_string;
    efi_text_test_string         test_string;
    efi_text_query_mode          query_mode;
    efi_text_set_mode            set_attribute;
    efi_text_clear_screen        clear_screen;
    efi_text_set_cursor_position set_cursor_position;
    efi_text_enable_cursor       enable_cursor;
    simple_text_output_mode     *mode;
};

struct _efi_simple_text_output_mode
{
    efi_int32   max_mode;
    efi_int32   mode;
    efi_int32   attribute;
    efi_int32   cursor_column;
    efi_int32   cursor_row;
    efi_boolean cursor_visible;
};
//  enum declarations
enum _efi_timer_delay
{
    EFI_TIMER_CANCEL,
    EFI_TIMER_PERIODIC,
    EFI_TIMER_RELATIVE,
};

enum _efi_allocate_type
{
    EFI_ALLOCATE_ANY_PAGES,
    EFI_ALLOCATE_MAX_ADDRESS,
    EFI_ALLOCATE_ADDRESS,
    EFI_MAX_ALLOCATE_TYPE,
};

enum _efi_memory_type
{
    EFI_RESERVED_MEMORY_TYPE,
    EFI_LOADER_CODE,
    EFI_LOADER_DATA,
    EFI_BOOT_SERVICES_CODE,
    EFI_BOOT_SERVICES_DATA,
    EFI_RUNTIME_SERVICES_CODE,
    EFI_RUNTIME_SERVICES_DATA,
    EFI_CONVENTIONAL_MEMORY,
    EFI_UNUSABLE_MEMORY,
    EFI_ACPI_RECLAIM_MEMORY,
    EFI_ACPI_MEMORY_NVS,
    EFI_MEMORY_MAPPED_IO,
    EFI_MEMMORY_MAPPED_IO_PORT_SPACE,
    EFI_PAL_CODE,
    EFI_PERSISTENT_MEMORY,
    EFI_UNACCEPTED_MEMORY_TYPE,
    EFI_MAX_MEMORY_TYPE,
};

enum _efi_interface_type
{
    EFI_NATIVE_INTERFACE,
};

enum _efi_locate_search_type
{
    EFI_ALL_HANDLES,
    EFI_BY_REGISTER_NOTIFY,
    EFI_BY_PROTOCOL,
};

int efi_guid_eq_check ( efi_guid *, efi_guid * );
EFI_FILE_EPILOGUE
#endif    //  ifndef KERNEL_LOADERS_EFI_H