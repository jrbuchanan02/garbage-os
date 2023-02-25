/**
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Defines things with ACPI tables and such
 * @version 1
 * @date 2023-02-21
 * @copyright Copyright (C) 2023. Intellectual property of the author(s) listed
 * above.
 */
#ifndef SOURCE_COMMON_HAL_ACPI_H
#define SOURCE_COMMON_HAL_ACPI_H

#include <stdint.h>

struct _acpi_old_rsdp;
struct _acpi_new_rsdp;
struct _acpi_meta_rsdp;

struct _acpi_description_header;

struct _acpi_rsdt;
struct _acpi_xsdt;

struct _acpi_dsdt;
struct _acpi_ssdt;

typedef struct _acpi_old_rsdp  acpi_old_rsdp;
typedef struct _acpi_new_rsdp  acpi_new_rsdp;
typedef struct _acpi_meta_rsdp acpi_meta_rsdp;

typedef struct _acpi_description_header acpi_description_header;

typedef struct _acpi_rsdt acpi_rsdt;
typedef struct _acpi_xsdt acpi_xsdt;
typedef struct _acpi_dsdt acpi_dsdt;
typedef struct _acpi_ssdt acpi_ssdt;

enum _acpi_rsdp_table_validity
{
    ACPIRSDP_INVALID,    //  invalid table
    ACPIRSDP_VALID_1,    //  valid version 1.0 table, also specifically not a
                         //  valid rev2+ table
    ACPIRSDP_VALID_2,    //  valid version 2.0+ table, also means its a valid
                         //  rev1 table
};
typedef enum _acpi_rsdp_table_validity acpi_rsdp_table_validity;

#define APIC_TABLE_SIGNATURE "APIC"
//  more should follow
#define RSDT_TABLE_SIGNATURE "RSDT"
//  more shouild follow
#define XSDT_TABLE_SIGNATURE "XSDT"

/**
 * @struct _acpi_old_rdsp
 * @brief the ACPI Root System Description Pointer for ACPI 1.0
 */
struct _acpi_old_rsdp
{
    char     signature [ 8 ];
    uint8_t  checksum;
    char     oem_id [ 6 ];
    uint8_t  revision;
    uint32_t rdst_address;
} __attribute__ ( ( packed ) );

/**
 * @struct _acpi_new_rdsp
 * @brief the ACPI Root System Description Pointer for ACPI 2.0+
 */
struct _acpi_new_rsdp
{
    acpi_old_rsdp old_part;
    uint32_t      length;
    uint64_t      xsdt_address;
    uint8_t       extended_checksum;
    uint8_t       reserved_fields [ 3 ];
} __attribute__ ( ( packed ) );

struct _acpi_meta_rsdp
{
    uintmax_t table_revision;
    union
    {
        acpi_old_rsdp *rev1;
        acpi_new_rsdp *rev2;
    };
} __attribute__ ( ( packed ) );

struct _acpi_description_header
{
    char     signature [ 4 ];
    uint32_t length;
    uint8_t  revision;
    uint8_t  checksum;
    char     oem_id [ 6 ];
    char     oem_table_id [ 8 ];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
} __attribute__ ( ( packed ) );

struct _acpi_ssdt
{
    acpi_description_header header;
    uint8_t aml_code[];
} __attribute__ ( ( packed ) );

struct _acpi_dsdt
{
    acpi_description_header header;
    uint8_t aml_code[];
} __attribute__ ( ( packed ) );

struct _acpi_rsdt
{
    acpi_description_header header;
    uint32_t                entries [];
} __attribute__ ( ( packed ) );

struct _acpi_xsdt
{
    acpi_description_header header;
    uint64_t                entries [];
} __attribute__ ( ( packed ) );

acpi_rsdp_table_validity is_valid_rsdp_table ( void * );

#endif    //  ifndef SOURCE_COMMON_HAL_ACPI_H