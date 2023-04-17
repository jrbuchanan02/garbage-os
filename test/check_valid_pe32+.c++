/**
 * @file check_valid_pe32+.c++
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Checks for a valid EFI image (these are PE32+ formatted images)
 * @version 1
 * @date 2023-04-16
 *
 * @copyright Copyright (C) 2023. Intellectual property of the author(s) listed above.
 *
 */

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

bool check_file( std::ifstream & );

int main( int const argc, char const *const *const argv ) {
    bool processed_any_files = false;
    bool all_passed          = true;
    for ( int i = 1; i < argc; i++ ) {
        std::ifstream fstream { argv [ i ] };
        if ( fstream.good( ) ) {
            std::cout << "Checking " << argv [ i ] << "\n";
            processed_any_files  = true;
            all_passed          &= check_file( fstream );
        } else {
            std::cout << "File does not exist: " << argv [ i ] << ". So it cannot be a valid PE32+\n";
            return -1;
        }
    }

    if ( !processed_any_files ) {
        std::cout << "No files procesed, so no files passed.\n";
        return -1;
    } else if ( !all_passed ) {
        std::cout << "Not all files passed. See above for details.\n";
        return -1;
    } else {
        std::cout << "All files passed. Check above for any notes.\n";
        return 0;
    }
}

struct ms_dos_2_stub {
    std::uint8_t literally_m;
    std::uint8_t literally_z;
    std::uint8_t offset_of_signature;
};

struct pe_signature {
    std::uint8_t  pe_string [ 4 ];
    std::uint16_t machine;
    std::uint16_t section_count;
    std::uint32_t timestamp;
    std::uint32_t symbol_offset_and_count [ 2 ];
    std::uint16_t optional_header_size;
    std::uint16_t characteristics;
};

struct machine_type_data {
    std::string   name;
    std::uint16_t field_value;
    bool          expected_in_efi;
};

static machine_type_data machine_types [] = {
        {                                         "unknown", 0x0000, false},
        {                                             "x64", 0x8664,  true},
        {                                           "IA-32", 0x014C,  true},
        {                                           "IA-64", 0x0200,  true},
        {"ARM / Thumb (EFI reads as ARM, Windows as Thumb)", 0x01C2,  true},
        {                                         "aarch64", 0xAA64,  true},
        {                                        "riscv-32", 0x5032,  true},
        {                                        "riscv-64", 0x5064,  true},
        {                                       "riscv-128", 0x5128,  true},
        {                                    "LoongArch-32", 0x6232,  true},
        {                                    "LoongArch-64", 0x6264,  true},
};

class not_valid_pep {
    std::string reason;

public:
    not_valid_pep( std::string const &reason ) : reason { reason } {
    }

    std::string why( ) const noexcept {
        return reason;
    }
};

std::uint8_t read_byte( std::vector<std::uint8_t>::const_iterator const &from, std::size_t offset = 0 ) {
    return *( from + offset );
}

void read_generic( std::vector<std::uint8_t>::const_iterator const &from,
                   std::unsigned_integral auto                     &into,
                   std::size_t                                      offset        = 0,
                   bool                                             little_endian = true ) {
    std::size_t highest_byte_shift = 8 * ( sizeof( into ) - 1 );
    into                           = 0;
    for ( std::size_t i = 0; i < sizeof( into ); i++ ) {
        if ( little_endian ) {
            into >>= 8;
        } else {
            into <<= 8;
        }
        into |= read_byte( from, offset + i ) << ( little_endian ? highest_byte_shift : 0 );
    }
}

template <std::unsigned_integral integer>
void read_into( std::vector<std::uint8_t>::const_iterator const &from,
                std::output_iterator<integer> auto const        &start,
                std::output_iterator<integer> auto const        &end,
                std::size_t                                      offset        = 0,
                bool                                             little_endian = true ) {
    for ( auto i = start; i < end; i++ ) { read_generic( from, *i, offset + ( i - start ), little_endian ); }
}

ms_dos_2_stub extract_ms_dos_2( std::vector<std::uint8_t>::const_iterator const &from, std::size_t offset = 0 ) {
    ms_dos_2_stub found;
    found.literally_m         = read_byte( from + offset, 0 );
    found.literally_z         = read_byte( from + offset, 1 );
    found.offset_of_signature = read_byte( from + offset, 0x3C );

    if ( found.literally_m != 'M' ) {
        std::stringstream message;
        message << "First byte is not 'M' but " << found.literally_m;
        message << " (" << std::hex << (unsigned) found.literally_m << ")";
        throw not_valid_pep( message.str( ) );
    }

    if ( found.literally_z != 'Z' ) {
        std::stringstream message;
        message << "Second byte is not 'Z' but " << found.literally_z;
        message << " (" << std::hex << (unsigned) found.literally_z << ")";
        throw not_valid_pep( message.str( ) );
    }

    if ( found.offset_of_signature < 0x3D ) {
        std::stringstream message;
        message << "PE offset byte (byte at offset 0x3C) indicates that the PE";
        message << " parts of the file start BEFORE the end of the MS DOS 2.0 ";
        message << "compatibility stub, at offset " << (unsigned) found.offset_of_signature;
        throw not_valid_pep( message.str( ) );
    }

    return found;
}

pe_signature extract_signature( std::vector<std::uint8_t>::const_iterator const &from, std::size_t offset = 0 ) {
    pe_signature signature;

    read_into<std::uint8_t>( from,
                             std::begin( signature.pe_string ),
                             std::end( signature.pe_string ),
                             offset + offsetof( pe_signature, pe_string ) );
    read_generic( from, signature.machine, offset + offsetof( pe_signature, machine ) );
    read_generic( from, signature.section_count, offset + offsetof( pe_signature, section_count ) );
    read_generic( from, signature.timestamp, offset + offsetof( pe_signature, timestamp ) );
    read_into<std::uint32_t>( from,
                              std::begin( signature.symbol_offset_and_count ),
                              std::end( signature.symbol_offset_and_count ),
                              offset + offsetof( pe_signature, symbol_offset_and_count ) );
    read_generic( from, signature.optional_header_size, offset + offsetof( pe_signature, optional_header_size ) );
    read_generic( from, signature.characteristics, offset + offsetof( pe_signature, characteristics ) );

    bool type_was_seen = false;
    for ( auto type : machine_types ) {
        if ( type.field_value == signature.machine ) {
            std::cout << "\tFile identifies itself as a(n) " << type.name << ", which is ";
            if ( !type.expected_in_efi ) { std::cout << "NOT "; }
            std::cout << "expected in an EFI loader. Check that this matches what the name of the file indicates.\n";
            type_was_seen = true;
            break;
        }
    }

    if ( !type_was_seen ) {
        std::stringstream message;
        message << "Machine type field is 0x" << std::hex << signature.machine << ". Which is not known to Garbage OS";
        message << " and likely not to any firmware either.";
        throw not_valid_pep( message.str( ) );
    }

    if ( signature.section_count < 1 ) {
        std::stringstream message;
        message << "Section count field is 0, which implies that this file contains nothing."
                << " That does not make logical sense.";
        throw not_valid_pep( message.str( ) );
    }

    std::cout << "\tFile says it was compiled " << std::dec << signature.timestamp
              << " seconds after the last time that the posix time rolled over.\n";
    auto now = std::chrono::steady_clock::now( ).time_since_epoch( );
    std::cout << "\tFor context, " << now.count( ) / 10000. << " seconds have elapsed since that time.\n";

    return signature;
}

bool check_file( std::ifstream &file ) {
    std::vector<std::uint8_t> bytes;
    while ( !file.eof( ) ) {
        std::uint8_t temp;
        temp = (std::uint8_t) file.get( );
        bytes.push_back( temp );
    }

    auto start_of_bytes = bytes.cbegin( );
    try {
        ms_dos_2_stub stub              = extract_ms_dos_2( start_of_bytes, 0 );
        std::size_t   signature_offset  = stub.offset_of_signature;
        start_of_bytes                 += signature_offset;
        pe_signature signature          = extract_signature( start_of_bytes, 0 );
        start_of_bytes                 += sizeof( signature );

    } catch ( not_valid_pep why_its_invalid ) {
        std::cout << "\tFile was found to be invalid because: " << why_its_invalid.why( ) << "\n";
        return false;
    }

    std::cout << "\tFile may be accepted, nothing said it's invalid but notes may have been emitted.\n";
    return true;
}