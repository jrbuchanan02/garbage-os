/**
 * @file efi.h++
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief A C++ version of the UEFI standard.
 * @note This file is intentionally incomplete. You are free to contribute if
 * you know the details of how C++ code links to binary.
 * @version 1
 * @date 2023-04-05
 *
 * @copyright EFI and UEFI are copyright of the UEFI group. Garbage OS is
 * Copyright (C) 2023 Joshua Buchanan and distributed freely under the LGPL
 * version 2.1.
 */

#ifndef EXTERNAL_STANDARDS_UEFI_EFI_HPP
#define EXTERNAL_STANDARDS_UEFI_EFI_HPP

#include <utility_functions/multi_part_integer.h++>

#include <concepts>
#include <cstdint>

#if defined( ANY_86_TARGET )
#  define EFI_API __attribute__( ( ms_abi ) )
#elif defined( ARM_THUMB_TARGET )
#  define EFI_API __attribute__( ( target( "arm" ) ) )
#else
#  define EFI_API
#endif    //  if defined(ANY_86_TARGET)

#define EFI_CLASS __attribute__( ( packed ) )

template <typename type>
concept not_void = !std::same_as<type, void>;

namespace efi {
    using boolean = bool;
    using intn    = std::ptrdiff_t;
    using uintn   = std::size_t;
    using int8    = std::int8_t;
    using uint8   = std::uint8_t;
    using int16   = std::int16_t;
    using uint16  = std::uint16_t;
    using int32   = std::int32_t;
    using uint32  = std::uint32_t;
    using int64   = std::uint64_t;
    using uint64  = std::uint64_t;
    using int128  = composite_uint128;
    using uint128 = composite_uint128;

    using char8   = char;
    using char16  = std::uint16_t;

    struct guid;
    struct mac_address;
    struct ipv4_address;
    struct ipv6_address;
    union ip_address;

    using status                = uintn;
    using handle                = void *;
    using event                 = void *;
    using logical_block_address = uint64;
    using physical_address      = uint64;
    using virtual_address       = uint64;
    using task_priority_level   = uintn;

    constexpr uint32 revision( uint16 major, uint16 minor ) {
        return ( major << 16 ) | minor;
    }

    namespace revisions {
        // revisions go here.
    }

    namespace signatures {
        constexpr uint64 system_table  = 0x5453'5953'2049'4249;
        constexpr uint64 boot_services = 0x5652'4553'544F'4F42;
    }    //  namespace signatures

    template <typename protocol> struct EFI_CLASS protocol_handle_pair {
        handle    file_like_part;
        protocol *protocol_part;
    };

    struct table_header;
    struct system_table;
    struct boot_services;
    struct runtime_services;
    struct configuration_table;
    struct runtime_properties_table;

    class simple_text_input;
    class simple_text_output;

    // function callback definitions here
    template <typename returns, not_void... args> using efi_function = returns( EFI_API * )( args... );

    struct EFI_CLASS table_header {
        uint64 signature;
        uint32 revision;
        uint32 header_size;
        uint32 crc32;
        uint32 reserved_header_value;
    };

    struct EFI_CLASS system_table : public table_header {
        char16                                  *firmware_vendor;
        uint32                                   firmware_revision;
        protocol_handle_pair<simple_text_input>  console_in;
        protocol_handle_pair<simple_text_output> console_out;
        protocol_handle_pair<simple_text_output> standard_error;
        runtime_services                        *runtime;
        boot_services                           *boot;
        uintn                                    table_entries;
        configuration_table                     *configuration;
    };

    struct EFI_CLASS boot_services : public table_header { };

    struct EFI_CLASS runtime_services : public table_header { };

    /**
     * @brief One of the configuration tables that may or may not exist in the
     * list of configuration entries in system_table.
     * @note if this table explicitly defines NO supported services, then the
     * loader can reclaim runtime data and code. This table explicitly defines
     * one bit in its bitmask for each function that runtime services provides.
     * @note if a service is not supported, it is still safe to call after
     * exiting boot services.
     * @note when exiting boot services and applying a new virtual memory map,
     * the address of this table within its configuration entry is not fixed.
     */
    struct EFI_CLASS runtime_properties_table {
        uint16 version;
        uint16 length;
        uint32 supported_services;
    };

    struct EFI_CLASS guid {
        uint32 data1;
        uint16 data2, data3;
        uint8  data4 [ 8 ];
    };

    struct EFI_CLASS mac_address {
        uint8_t bytes [ 32 ];
    };

    struct EFI_CLASS ipv4_address {
        uint8 bytes [ 4 ];
    };

    struct EFI_CLASS ipv6_address {
        uint8 bytes [ 16 ];
    };

    union EFI_CLASS __attribute__( ( aligned( 4 ) ) ) ip_address {
        ipv4_address as_v4;
        ipv6_address as_v6;
    };

    static_assert( alignof( ip_address ) == 4 );

    //  industry standard guids
    constexpr static guid acpi_rsdp_revision2_guid = {
            0x8868'E871,
            0xE4F1,
            0x11D3,
            {0xBC, 0x22, 0x00, 0x80, 0xC7, 0x3C, 0x88, 0x81}
    };

    constexpr static guid acpi_rsdp_revision0_guid = {
            0xEB9'D230,
            0x2D88,
            0x11D3,
            {0x9A, 0x16, 0x00, 0x90, 0x27, 0x3F, 0xC1, 0x4D}
    };

    //  UEFI then describes a guid relating to a standard named SAL. I have
    //  never heard of this standard and cannot find information relating to it
    //  on the internet. If I DID include the SAL guid, it would go here.

    constexpr static guid smbios_table_guid = {
            0xEB9D'2D31,
            0x2D88,
            0x11D3,
            {0x9A, 0x16, 0x00, 0x90, 0x27, 0x3F, 0xC1, 0x4D}
    };

    constexpr static guid smbios3_table_guid {
            0xF2FD'1544,
            0x9794,
            0x4A2C,
            {0x99, 0x23, 0xE5, 0xBB, 0xCf, 0x20, 0xE3, 0x94}
    };

    constexpr static guid devicetree_blob_table_guid {
            0xB1B6'21D5,
            0xF19C,
            0x41A5,
            {0x83, 0x0B, 0xD9, 0x15, 0x2C, 0x69, 0xAA, 0xE0}
    };

    constexpr static guid runtime_properties_table_guid {
            0xEB66'918A,
            0x7EEF,
            0x402A,
            {0x84, 0x2E, 0x93, 0x1D, 0x21, 0xC3, 0x8A, 0xE9}
    };

    constexpr static uint32 runtime_supports_get_time                 = 0x0001;
    constexpr static uint32 runtime_supports_set_time                 = 0x0002;
    constexpr static uint32 runtime_supports_get_wake_time            = 0x0004;
    constexpr static uint32 runtime_supports_set_wake_time            = 0x0008;
    constexpr static uint32 runtime_supports_get_variable             = 0x0010;
    constexpr static uint32 runtime_supports_get_next_variable_name   = 0x0020;
    constexpr static uint32 runtime_supports_set_variable             = 0x0040;
    constexpr static uint32 runtime_supports_set_virtual_address_map  = 0x0080;
    constexpr static uint32 runtime_supports_convert_pointer          = 0x0100;
    constexpr static uint32 runtime_supports_get_next_high_mono_count = 0x0200;
    constexpr static uint32 runtime_supports_reset_system             = 0x0400;
    constexpr static uint32 runtime_supports_update_capsule           = 0x0800;
    constexpr static uint32 runtime_supports_query_capsule_abilities  = 0x1000;
    constexpr static uint32 runtime_supports_query_variable_info      = 0x2000;

    struct EFI_CLASS configuration_table {
        guid  vendor_guid;
        void *vendor_table;
    };
}    //  namespace efi

inline bool operator== ( efi::guid const &lhs, efi::guid const &rhs ) {
    if ( lhs.data1 != rhs.data1 ) { return false; }
    if ( lhs.data2 != rhs.data2 ) { return false; }
    if ( lhs.data3 != rhs.data3 ) { return false; }
    constexpr static std::integral auto data4_length = sizeof( efi::guid::data4 ) / sizeof( efi::uint8 );
    for ( std::unsigned_integral auto i = 0U; i < data4_length; i++ ) {
        if ( lhs.data4 [ i ] != rhs.data4 [ i ] ) { return false; }
    }
    return true;
}

#endif