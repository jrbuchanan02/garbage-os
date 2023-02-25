/**
 * @file efimain.c
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Entry point for all EFI builds.
 * @version 1
 * @date 2022-03-23
 *
 * @copyright Copyright (C) 2022. Intellectual property of the author(s) listed
 * above.
 *
 */
#include <common/hal/acpi.h>
#include <common/hal/framebuffer.h>
#include <kernel/entry/main.h>
#include <kernel/loaders/common/boot_printf.h>
#include <kernel/loaders/common/memory_sizes.h>
#include <kernel/loaders/efi/efi.h>
#include <stdlib.h>

efi_system_table     *system_table_pointer;
efi_boot_services    *boot_services;
efi_runtime_services *runtime_services;

efi_simple_text_output_protocol *stdout;

efi_memory_descriptor *memory_map;
efi_uintn              memory_map_key;
efi_uintn              descriptor_size;
efi_uint32             descriptor_version;
efi_uintn              descriptor_count;

efi_handle boot_disk;

acpi_meta_rsdp acpi_rsdp;

static kmain_magic magic;

void remember_variables ( efi_handle, efi_system_table * );
void greet_console ( void * );
void get_memory_map ( );
void report_memory_map ( );
void find_acpi_rsdp ( );
void exit_boot_services ( );

void report_some_flag ( uintmax_t, uintmax_t, char );

/**
 * The function that the firmware calls to start loading Garbage OS. This
 * function has two jobs:
 * 1. exit boot services.
 * 2. prepare the parameters needed for kmain.
 *
 * Once we enter this function the clock is ticking: boot services has a
 * watchdog timer and will reset the system if we take too long. However, we
 * only need the memory map to exit boot services. However, I would like to also
 * grab the ACPI configuration table since exiting boot services invalidates
 * the con_in and con_out objects given in the system table (meaning no logging
 * if we exit boot services until we initialize something using ACPI.)
 */
efi_status EFIAPI efi_main ( efi_handle        image_handle,
                             efi_system_table *system_table )
{
    efi_status return_value;
    //  setup things...
    remember_variables ( image_handle, system_table );
    greet_console ( &return_value );
    get_memory_map ( );
    report_memory_map ( );
    find_acpi_rsdp ( );

    exit_boot_services ( );

    magic.loader = EFI;
    magic.rsdt   = ( acpi_rsdt   *) ( uintmax_t ) acpi_rsdp.rev1->rdst_address;
    magic.xsdt   = 0;
    if ( acpi_rsdp.table_revision > 1 )
    {
        magic.xsdt = ( acpi_xsdt * ) acpi_rsdp.rev2->xsdt_address;
    }

    kmain ( 0, &magic );

    return -1;
}

void efi_compat_putc ( char character )
{
    efi_char16 temp [ 2 ] = { character, 0 };
    stdout->output_string ( stdout, temp );
}

void remember_variables ( efi_handle bd, efi_system_table *st )
{
    system_table_pointer = st;
    boot_disk            = bd;
    boot_services        = st->boot_services;
    runtime_services     = st->runtime_services;
    stdout               = st->con_out;
}

void greet_console ( void *first_variable )
{
    stdout->reset ( stdout, 0 );
    puts ( efi_compat_putc, "GOS loading from EFI firmware.\r\n" );
    puts ( efi_compat_putc,
           "For debugging purposes, stack pointer is around 0x" );
    putx ( efi_compat_putc, ( uintmax_t ) first_variable, 'A' );
    puts ( efi_compat_putc, "\r\n" );
}

void get_memory_map ( )
{
    efi_uintn  memory_map_size = 0;
    uintmax_t  memory_map_buffer_size;
    uintmax_t  memory_map_buffer_bytes;
    efi_status retval;
    memory_map         = 0;
    memory_map_key     = 0;
    descriptor_size    = 0;
    descriptor_version = 0;

    retval = boot_services->get_memory_map ( &memory_map_size,
                                             memory_map,
                                             &memory_map_key,
                                             &descriptor_size,
                                             &descriptor_version );

    memory_map_buffer_size  = pages_from_bytes ( memory_map_size );
    memory_map_buffer_bytes = 4096 * memory_map_buffer_size;

    retval = boot_services->allocate_pages (
            EFI_ALLOCATE_ANY_PAGES,
            EFI_LOADER_DATA,
            memory_map_buffer_size,
            ( efi_physical_address * ) &memory_map );
    if ( retval )
    {
        puts ( efi_compat_putc,
               "Error: could not allocate buffer for memory map.\r\n" );
        puts ( efi_compat_putc, "Error code: 0x" );
        putx ( efi_compat_putc, retval, 'A' );
        puts ( efi_compat_putc, "\r\n" );
    }

    retval = boot_services->get_memory_map ( &memory_map_buffer_bytes,
                                             memory_map,
                                             &memory_map_key,
                                             &descriptor_size,
                                             &descriptor_version );
    if ( retval != 0 )
    {
        puts ( efi_compat_putc,
               "Error: Could not get access to memory map. Encountered error "
               "code " );
        putx ( efi_compat_putc, retval, 'A' );
        puts ( efi_compat_putc, ". Cannot continue.\r\n" );
        while ( 1 ) { }
    }

    descriptor_count = memory_map_buffer_bytes / descriptor_size;
}

void report_memory_map ( )
{
    puts ( efi_compat_putc, "Memory map has " );
    putu ( efi_compat_putc, descriptor_count );
    puts ( efi_compat_putc, " entries.\r\n" );

    uintmax_t flags [] = {
            EFI_MEMORY_UC,
            EFI_MEMORY_WC,
            EFI_MEMORY_WT,
            EFI_MEMORY_WB,
            EFI_MEMORY_UCE,
            EFI_MEMORY_WP,
            EFI_MEMORY_RP,
            EFI_MEMORY_SP,
            EFI_MEMORY_XP,
            EFI_MEMORY_NV,
            EFI_MEMORY_MORE_RELIABLE,
            EFI_MEMORY_RO,
            EFI_MEMORY_RUNTIME,
    };

    char flag_chars [] = {
            'U',
            'C',
            'T',
            'B',
            'E',
            'w',
            'R',
            'S',
            'X',
            'N',
            'M',
            'r',
            't',
    };

    uintmax_t   flag_count = sizeof ( flags ) / sizeof ( uintmax_t );
    uintmax_t   a;
    uintmax_t   i;
    uintmax_t   j;
    uintmax_t   entry_end;
    char const *type_string = "";

    efi_memory_descriptor *descriptor = memory_map;

    for ( i = 0; i < 129;
          i++, descriptor = get_next ( descriptor, descriptor_size ) )
    {
        puts ( efi_compat_putc, "Entry " );
        putd ( efi_compat_putc, i );
        puts ( efi_compat_putc, ":\r\n\tType: " );

        switch ( ( efi_memory_type ) descriptor->type )
        {
            case EFI_RESERVED_MEMORY_TYPE: type_string = "Reserved"; break;
            case EFI_LOADER_CODE: type_string = "Loader Code (GOS Code)"; break;
            case EFI_LOADER_DATA: type_string = "Loader Data (GOS RAM)"; break;
            case EFI_BOOT_SERVICES_CODE:
                type_string = "Boot Services Code";
                break;
            case EFI_BOOT_SERVICES_DATA:
                type_string = "Boot Services Data";
                break;
            case EFI_RUNTIME_SERVICES_CODE:
                type_string = "Runtime Services Code";
                break;
            case EFI_RUNTIME_SERVICES_DATA:
                type_string = "Runtime Services Data";
                break;
            case EFI_CONVENTIONAL_MEMORY:
                type_string = "Conventional Memory";
                break;
            case EFI_UNUSABLE_MEMORY: type_string = "Unusable Memory"; break;
            case EFI_ACPI_RECLAIM_MEMORY:
                type_string = "ACPI Reclaim(able?) Memory";
                break;
            case EFI_ACPI_MEMORY_NVS:
                type_string = "ACPI Persistent Memory";
                break;
            case EFI_MEMORY_MAPPED_IO: type_string = "MMIO"; break;
            case EFI_MEMMORY_MAPPED_IO_PORT_SPACE:
                type_string = "MMIO Port Space";
                break;
            case EFI_PAL_CODE: type_string = "\"Pal Code\""; break;
            case EFI_PERSISTENT_MEMORY:
                type_string = "Persistent Memory";
                break;
            case EFI_UNACCEPTED_MEMORY_TYPE:
                type_string = "Unaccepted Memory";
                break;
            default: type_string = "Unknown, GOS will ignore."; break;
        }

        puts ( efi_compat_putc, type_string );
        puts ( efi_compat_putc, " \r\n\tAttributes: " );

        a = descriptor->attribute;

        for ( j = 0; j < flag_count; j++ )
        {
            report_some_flag ( a, flags [ j ], flag_chars [ j ] );
        }

        puts ( efi_compat_putc, " \r\n\tStart Address: " );
        putx ( efi_compat_putc, descriptor->physical_start, 'A' );
        puts ( efi_compat_putc, " \r\n\tEnd Address: " );
        entry_end =
                4096 * descriptor->number_of_pages + descriptor->physical_start;
        putx ( efi_compat_putc, entry_end, 'A' );
        puts ( efi_compat_putc, " \r\n" );
    }
}

void report_some_flag ( uintmax_t thing, uintmax_t bit_set, char flag )
{
    if ( thing & bit_set )
    {
        efi_compat_putc ( flag );
    } else {
        efi_compat_putc ( '-' );
    }
}

void find_acpi_rsdp ( )
{
    uintmax_t entry_count = system_table_pointer->number_of_table_entries;
    efi_configuration_table *tables = system_table_pointer->configuration_table;
    efi_guid                 acpi_guid [ 2 ] = { ACPI_OLD_GUID, ACPI_NEW_GUID };

    int       found_old          = 0;
    int       found_new          = 0;
    uintmax_t old_table_location = 0;
    uintmax_t new_table_location = 0;
    uintmax_t i;

    for ( i = 0; i < entry_count; i++ )
    {
        if ( efi_guid_eq_check ( &tables [ i ].vendor_guid, &acpi_guid [ 0 ] ) )
        {
            found_old          = 1;
            old_table_location = i;
        }
        if ( efi_guid_eq_check ( &tables [ i ].vendor_guid, &acpi_guid [ 1 ] ) )
        {
            found_new          = 1;
            new_table_location = i;
        }
    }

    if ( found_new )
    {
        puts ( efi_compat_putc,
               "Found revision 2 ACPI configuration table.\r\n" );
        acpi_rsdp.table_revision = 2;
        acpi_rsdp.rev2           = tables [ new_table_location ].vendor_table;
    } else if ( found_old ) {
        puts ( efi_compat_putc,
               "Found revision 1 ACPI configuration table.\r\n" );
        acpi_rsdp.table_revision = 1;
        acpi_rsdp.rev1           = tables [ old_table_location ].vendor_table;
    } else {
        puts ( efi_compat_putc,
               "Garbage Operating System was unable to find the Advanced "
               "Configuration and Power Interface Root System Descriptino "
               "Pointer in the Unified Extensible Firmware Interface "
               "configuration table. Garbage Operating System will assume that "
               "your computer uses a Color Graphics Array system.\r\n" );
        puts ( efi_compat_putc,
               "In English, GOS cannot find the list of devices on your system "
               "and will assume the bare minimum.\r\n" );
    }
}

void exit_boot_services ( )
{
    //  get the memory map to ensure that we have up-to-date information
    //  to give the key to the call for exit boot services.
    uintmax_t  mmap_buffer_bytes = descriptor_size * descriptor_count;
    efi_status return_value;
    boot_services->get_memory_map ( &mmap_buffer_bytes,
                                    memory_map,
                                    &memory_map_key,
                                    &descriptor_size,
                                    &descriptor_version );
    return_value =
            boot_services->exit_boot_services ( boot_disk, memory_map_key );

    if ( return_value )
    {
        /*failed to exit boot services*/
    } else {
        /*successfully exited boot services*/
    }
}