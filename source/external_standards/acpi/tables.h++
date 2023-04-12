/**
 * @file tables.h++
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Defines the ACPI tables.
 * @version 1
 * @date 2023-04-09
 *
 * @copyright Copyright (C) 2023. Intellectual property of the author(s) listed
 * above.
 *
 */
#ifndef EXTERNAL_STANDARDS_ACPI_HPP
#define EXTERNAL_STANDARDS_ACPI_HPP

#include <concepts>
#include <cstddef>
#include <cstdint>

#define ACPI_TABLE __attribute__( ( packed ) )

namespace acpi {
    constexpr static std::uint8_t known_revisions [ 2 ]         = { 0, 2 };
    constexpr static std::uint8_t known_skipped_revisions [ 1 ] = { 1 };

    template <std::integral auto len = 4> requires( len > 0 )
    using acpi_string = __attribute__( ( nonstring ) ) char [ len ];

    struct ACPI_TABLE root_system_description_pointer_revision0 {
        acpi_string<8> signature;
        std::uint8_t   checksum;
        acpi_string<6> oemid;
        std::uint8_t   revision;
        std::uint32_t  rsdt_address;
    };

    struct ACPI_TABLE root_system_description_pointer_revision2 : public root_system_description_pointer_revision0 {
        std::uint32_t length;
        std::uint64_t xsdt_address;
        std::uint8_t  excended_checksum;
        std::uint8_t  reserved_bytes [ 3 ];
    };

    struct ACPI_TABLE system_description_header {
        acpi_string<>  signature;
        std::uint32_t  length;
        std::uint8_t   revision;
        std::uint8_t   checksum;
        acpi_string<6> oemid;
        std::uint32_t  oem_revision;
        acpi_string<>  creator_id;
        acpi_string<>  creator_revision;
    };

    template <typename type>
    concept acpi_table_with_checksum = requires( type const &t ) {
        requires std::same_as<std::uint8_t, decltype( t.checksum )>;
        requires std::same_as<std::uint8_t, decltype( t.revision )>;
        requires std::same_as<acpi_string<6>, decltype( t.oemid )>;
    };

    static_assert( acpi_table_with_checksum<root_system_description_pointer_revision0> );
    static_assert( acpi_table_with_checksum<system_description_header> );

}    //  namespace acpi

#endif    //  ifndef EXTERNAL_STANDARDS_ACPI_HPP