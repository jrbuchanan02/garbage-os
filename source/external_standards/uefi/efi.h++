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
 * version 2.1.You are free to use this source code in your project even if
 * your code is not open source.
 * @todo Add the error codes to this file.
 */

#ifndef EXTERNAL_STANDARDS_UEFI_EFI_HPP
#define EXTERNAL_STANDARDS_UEFI_EFI_HPP

#include <external_standards/spec1394/1394.h++>
#include <utility_functions/flexible_array.h++>
#include <utility_functions/multi_part_integer.h++>

#include <concepts>
#include <cstdarg>
#include <cstdint>

#if defined( ANY_86_TARGET ) && defined( _64_BITS )
// in x64, EFI uses the ms_abi, so when targeting x86 and in 64-bits
// define EFI_API to use the ms_abi.
#  define EFI_API __attribute__( ( ms_abi ) )
#elif defined( ARM_THUMB_TARGET )
// when targeting something that can both run ARM and Thumb code, make EFI_API
// an indication to explicitly use ARM code, since that's the mode that EFI
// standard requires the CPU to be in.
#  define EFI_API __attribute__( ( target( "arm" ) ) )
#else
// if the EFI standard does not require any additional attributes for correctly
// calling its functions, we still need to define EFI_API so the compiler
// knows it has no content in it.
#  define EFI_API
#endif    //  if defined(ANY_86_TARGET) && defined(_64_BITS)
// All EFI structures are packed, meaning that members are one after the other
// as opposed to sitting on the next naturally aligned boundary. Interestingly
// enough, however, this attribute only seems to affect a small number of
// EFI structures, with most having all members naturally aligned anyhow
// or explicitly having dummy members for alignment.
#define EFI_CLASS __attribute__( ( packed ) )

template <typename type>
// concept that explicitly prevents a type from being void when used in a
// template.
concept not_void = !std::same_as<type, void>;

namespace efi {
    /**
     * @brief EFI defines its own boolean type. This type behaves identically to
     * the native C++ type.
     *
     * @note Perhaps it exists in the standard because the source examples are
     * given in C-style pseudo-code?
     */
    using boolean = bool;
    /**
     * @brief Integer of native size. This was taken to mean std::intmax_t, the
     * largest integer that the machine can natively use.
     *
     */
    using intn    = std::intmax_t;
    /**
     * @brief Unsigned integer of native size. This was taken to mean std::uinmax_t,
     * the largest integer that the machine can natively use.
     *
     */
    using uintn   = std::uintmax_t;
    /**
     * @brief signed 8-bit integer.
     *
     */
    using int8    = std::int8_t;
    /**
     * @brief unsigned 8-bit integer.
     *
     */
    using uint8   = std::uint8_t;
    /**
     * @brief signed 16-bit integer.
     *
     */
    using int16   = std::int16_t;
    /**
     * @brief unsigned 8-bit integer.
     *
     */
    using uint16  = std::uint16_t;
    /**
     * @brief signed 32-bit integer.
     *
     */
    using int32   = std::int32_t;
    /**
     * @brief unsigned 32-bit integer.
     *
     */
    using uint32  = std::uint32_t;
#if defined( _32_BITS )
    /**
     * @brief EFI defines some things as 64-bit numbers regardless of the machine's
     * native size. I'll leave it as an exercise to the reader as to why we can't
     * natively use 64-bit numbers on a machine that only supports up to a 32-bit
     * number.
     */
    using int64   = composite_uint64;
    /**
     * @brief EFI defines some things as 64-bit numbers regardless of the machine's
     * native size. I'll leave it as an exercise to the reader as to why we can't
     * natively use 64-bit numbers on a machine that only supports up to a 32-bit
     * number.
     */
    using uint64  = composite_uint64;
    // TODO: make version of composite_uint128 that invovles two composite_uint64s
    using int128  = composite_uint64 [ 2 ];
    using uint128 = composite_uint64 [ 2 ];
#elif defined( _64_BITS )
    /**
     * @brief signed 64-bit integer.
     */
    using int64   = std::int64_t;
    /**
     * @brief unsigned 64-bit integer.
     *
     */
    using uint64  = std::uint64_t;
    /**
     * @brief Anticipate the requirements to use a 128-bit integer on 64-bit
     * systems. It may already be in the standard as EFI defines a 128-bit
     * integer.
     *
     */
    using int128  = composite_uint128;
    /**
     * @brief Anticipate the requirements to use a 128-bit integer on 64-bit
     * systems. It may already be in the standard as EFI defines a 128-bit
     * integer.
     *
     */
    using uint128 = composite_uint128;
#else
    // ensure that 128-bit numbers exist before assuming a build for a 128-bit
    // system.
    static_assert( sizeof( void * ) >= 16, "Memory addresses are the wrong size. Did you mean to define _32_BITS or _64_BITS?" );
    /**
     * @brief signed 64-bit integer
     *
     */
    using int64   = std::int64_t;
    /**
     * @brief unsigned 64-bit integer.
     *
     */
    using uint64  = std::uint64_t;
    /**
     * @brief signed 128-bit integer, uses the GCC builtin type.
     *
     */
    using int128  = __int128_t;
    /**
     * @brief unsigned 128-bit integer, uses the GCC builtin type.
     *
     */
    using uint128 = __uint128_t;
#endif    // if defined(_32_BITS
    /**
     * @brief UTF-8 character.
     *
     * @note not char8_t since C++20 cannot easily mix char8_t and char for
     * whatever reason. If Garbage OS ever buils for C++ 23 or later, then this
     * field should change to char8_t.
     */
    using char8                  = unsigned char;
    /**
     * @brief UTF-16 character.
     *
     */
    using char16                 = char16_t;
    /**
     * @brief What happened during the function? Did it succeed? Did it issue
     * a warning? Did it fail? If it failed, why might it have failed?
     *
     */
    using status                 = uintn;
    /**
     * @brief Handle = the everything is a file thingy.
     *
     */
    using handle                 = void *;
    /**
     * @brief An event. Probably represents some class somewhere in the firmware
     * vendor's top-secret firmware repository. We at least know it probably
     * points to something.
     *
     */
    using event                  = void *;
    /**
     * @brief A memory address, but it's for a disk and not RAM.
     *
     */
    using logical_block_address  = uint64;
    /**
     * @brief A logical block address, but Sony decided that CDs wanted to call
     * it something else.
     *
     */
    using relative_block_address = uint64;
    /**
     * @brief How important things are.
     *
     */
    using task_priority_level    = uintn;
    /**
     * @brief Memory address that corresponds to the actual number the CPU sends
     * out to the motherboard to get from DRAM, MMIO, etc.
     *
     */
    using physical_address       = uint64;
    /**
     * @brief Memory address that corresponds to what the software thinks the
     * CPU sends out on the bus. Whenever you write a non-DOS application, you're
     * using these.
     *
     */
    using virtual_address        = uint64;

    struct guid;
    struct mac_address;
    struct ipv4_address;
    struct ipv6_address;

    struct system_table;
    struct table_header;
    struct runtime_services;
    struct boot_services;
    struct configuration_table;
    struct memory_descriptor;
    struct open_protocol_information_entry;

    struct device_path;
    struct hardware_device_path;
    struct acpi_device_path;
    struct messaging_device_path;
    struct media_device_path;
    struct bios_boot_specification_device_path;

    struct card_device_path;
    struct pci_device_path;
    struct pccard_device_path;

    struct memory_mapped_device_path;
    struct vendor_specific_device_path;
    struct controller_device_path;
    struct baseboard_management_controller_device_path;

    struct generic_acpi_device_path;
    struct acpi_short_device_path;
    struct acpi_extended_device_path;

    struct acpi_address_device_path;
    struct nvdimm_device_path;

    struct atapi_device_path;
    struct scsi_device_path;
    struct fibre_channel_device_path;
    struct fibre_channel_ex_device_path;
    struct spec1394_device_path;
    struct sata_device_path;
    struct usb_device_paths;
    struct usb_wwid_device_path;
    struct device_logical_unit;
    struct usb_class_device_path;
    struct i2o_device_path;

    // struct aliases

    /**
     * @brief For some reason, it's the same format as an EFI GUID.
     *
     * @note coincidentally, Windows GUIDs for their Component Object Model part
     * of the windows API also use GUIDs of the same format.
     *
     */
    using nvdimm_uuid = guid;

    /**
     * @brief Templated version of the runtime-like union for the ip_address.
     * @note Exists because we want smaller IP addresses to sometimes actually
     * be smaller.
     *
     * @tparam type an ip address. I check the implementation. if you understand
     * the || operator then you can figure it out.
     */
    template <typename type>
    concept specific_ip_address = std::same_as<type, ipv4_address> || std::same_as<type, ipv6_address>;

    template <specific_ip_address address_type> struct generic_ip_device_path;

    struct ipv4_device_path;
    struct ipv6_device_path;

    struct ufs_device_path;
    struct mac_address_device_path;
    struct vlan_device_path;
    struct infiniband_device_path;
    struct uart_device_path;
    struct vendor_defined_messaging_device_path;
    struct sas_extended_device_path;
    struct iscsi_device_path;
    struct uri_device_path;
    struct nvme_namespace_device_path;
    struct wifi_device_path;

    struct generic_bluetooth_device_path;
    struct bluetooth_device_path;
    struct bluetooth_le_device_path;

    struct storage_card_device_path;
    struct sd_device_path;
    struct emmc_device_path;

    struct nvdimm_namespace_path;
    struct nvmeof_namespace_device_path;

    struct generic_rest_service_device_path;
    struct rest_service_device_path;
    struct vendor_specific_rest_device_path;

    struct hard_drive_media_device_path;
    struct vendor_defined_media_device_path;
    struct el_torito_device_path;
    struct file_path_media;
    struct media_protocol_device_path;
    struct piwg_firmware_file;
    struct piwg_firmware_volume;
    struct relative_offset_range_device_path;
    struct ram_disk_device_path;

    struct time;
    struct time_capabilities;

    struct simple_text_output_mode;
    struct input_key;

    class simple_text_io;
    class simple_text_input;
    class simple_text_output;
    class arbitrary_load_file;
    class load_boot_file;
    class load_non_boot_file;

    /**
     * @brief What you can do with setting / cancelling timers.
     *
     */
    enum struct timer_delay {
        /**
         * @brief Cancels the timer
         *
         */
        cancel_timer,
        /**
         * @brief Timer that immediately starts again after expiring.
         *
         */
        periodic_timer,
        /**
         * @brief Timer that does not reset after expiring.
         *
         */
        relative_timer,
    };

    /**
     * @brief Where to allocate memory and how to interpret the memory address
     * passed to boot_services.allocate_pages.
     */
    enum struct allocate_type {
        /**
         * @brief Allocates at some available address. the buffer passed to
         * allocate_pages is treated strictly as an out-parameter.
         */
        any_pages,
        /**
         * @brief Allocate the highest address available. the buffer passed to
         * allocate_pages is treated strictly as an out-parameter
         *
         */
        max_address,
        /**
         * @brief Allocate pages starting at the buffer passed to allocate_pages.
         * This buffer is treated asd both an in and out parameter.
         */
        specific_address,
        /**
         * @brief Memory type that doesn't really exist but is listed in the
         * standard. Will cause boot_services.allocate_pages to return
         * invalid_parameter.
         */
        maximum_allocate_type,
    };

    /**
     * @brief The various types of memory that are defined in the EFI standard.
     *
     */
    enum struct memory_type {
        /**
         * @brief Reserved for system use. Do not map. Do not write. Do not
         * EVER execute.
         */
        reserved_memory,
        /**
         * @brief Loader Code. When mapping, ensure that the current page is
         * identity mapped. Do map. Do not write. You really only should be
         * executing this until transferring control to the kernel.
         */
        loader_code,
        /**
         * @brief Loader Data. When mapping, translate all pointers to this
         * location and in this location to virtual locations. Do map. Do write.
         * Do not execute.
         *
         */
        loader_data,
        /**
         * @brief Boot Services Code. When Mapping, this is free memory since
         * we can only map memory after calling boot_services.exit_boot_services
         * so when we control the memory map, the boot services code is not safe
         * to call. Do map (as free ram). Do not write. Execute through calls
         * to the boot_services and its drivers.
         */
        boot_services_code,
        /**
         * @brief Boot Services Data. When mapping, this is free memory since
         * we can only map memory after calling boot_services.exit_boot_services
         * so when we control the memory map, the boot services data has no code
         * that can make sense of it. Do map (as free ram). Do not write. Do not
         * execute.
         */
        boot_services_data,
        /**
         * @brief Runtime Services Code. When Mapping, this remains Runtime
         * Services code and should be relocatable in the virtual address space,
         * but the loader should prefer an identity mapping if an
         * identity mapping would make sense. Only execute through runtime
         * services functions. Do not read. Do not write.
         */
        runtime_services_code,
        /**
         * @brief Runtime Services Data. When Mapping, this reamains Runtime
         * Services data and should be relocatable in the virtual address space,
         * but the loader should prefer an identity mapping if an
         * identity mapping would make sense. Do not read. Do not write. Do not
         * execute.
         */
        runtime_services_data,
        /**
         * @brief Conventional RAM. When reported by the memory map, we do not
         * own it. Before allocating, this ram should not be touched. This memory
         * can be moved literally anywhere in the virtual address space.
         *
         */
        conventional,
        /**
         * @brief Memory that would be conventional, but the firmware has detected
         * errors on it. This memory can be mapped anywhere, but cannot be trusted
         * for any practical use.
         */
        unusable,
        /**
         * @brief Memory that currently holds ACPI tables and can be reclaimed
         * when we are done with them. This memory can be treated as conventional
         * memory that we own.
         *
         */
        acpi_reclaimable,
        /**
         * @brief ACPI variables that (per the standard) need to be stored in
         * nonvolatile mewmory. That doesn't sound right, so I might be reading
         * it wrong.
         */
        acpi_store_in_nonvolatile,
        /**
         * @brief Memory that does not map to storage but to a device connected
         * to the system.
         *
         */
        memory_mapped_input_output,
        /**
         * @brief Special MMIO that corresponds to input / output ports.
         *
         */
        memory_mapped_input_output_port_space,
        /**
         * @brief The standard does not describe what pal code is.
         *
         */
        pal_code,    // no idea what "pal" means. EFI did not explain.
        /**
         * @brief Persistent memory, memory that is nonvolatile.
         *
         */
        persistent,
        /**
         * @brief Memory that the loader needs to "accept" after exiting
         * boot services... and that's all we know about it.
         *
         */
        unaccepted,
        /**
         * @brief Alias for memory_mapped_input_output.
         *
         */
        mmio   = memory_mapped_input_output,
        /**
         * @brief Alias for memory_mapped_input_output_port_space.
         *
         */
        mmiops = memory_mapped_input_output_port_space,
    };

    /**
     * @brief The different types of protocol interface that can be installed.
     *
     */
    enum struct interface_type {
        /**
         * @brief Oops it's all native.
         *
         */
        native,
    };

    /**
     * @brief Differrent ways we can search for the handle to some protocol.
     *
     */
    enum struct locate_search_type {
        /**
         * @brief Look in all handles
         */
        all_handles,
        /**
         * @brief Look by who's registered to be notified with things involving
         * the protocol and the handle
         */
        by_register_notify,
        /**
         * @brief We supply a GUID and we get protocols compatible with that GUID.
         *
         */
        by_protocol,
    };

    /**
     * @brief The types of device path.
     * Device Paths in EFI describe more than just the folders that a file is in
     * but also the physical route that commands must go through to get to the
     * thing that holds the file.
     */
    enum struct device_path_type : uint8 {
        /**
         * @brief Corresponds to a physical piece of hardware we need to talk
         * through to get to the device.
         *
         * Think, "what is the name of the wires
         * that the CPU needs to talk through to get to the disk?" For example,
         * a file on a "disk" in memory would specify that it's between two
         * specific addresses and something on a PCIe bus would need to specify
         * that you need to talk to this PCI device.
         *
         */
        hardware    = 0x01,
        /**
         * @brief What ACPI devices are used to talk to the device.
         *
         * Talking to these devices, or, commmunicating with them without the
         * help of the firmware (which the loader does not do) requires knowledge
         * of how the AML code works and also requires reading the all-important
         * tables of the ACPI standard.
         */
        acpi        = 0x02,
        /**
         * @brief Corresponds to the standard we use to talk to the hardware once
         * we have found the correct pieces of wires.
         *
         * Messaging device paths are quite broad, including everything from
         * Bluetooth to the venerable VT-100 teriminal. If the hardware part of
         * the device path is the location to send messages, the messaging part
         * of the device path is the language to send the messages in.
         */
        messaging   = 0x03,
        /**
         * @brief Corresponds to the type of disk the file is on and the name of
         * the file to actually access. I'm not sure why the file path is
         * considered media, but it's invalid to say that you're trying to access
         * File/{hard drive}/Foo.BIN since the firmware is supposed to reject it.
         * Not that the loader cares if the path makes logical sense.
         *
         * Interestingly, EFI makes no disstinction between any form of storage
         * device that is not RAM or an El Torito boot ROM. That's right, the
         * firmware sees your fancy 128 TiB NVMe RAID 123049 drive the same as it would see
         * an original 0.6 MiB single-sided single-density 8" floppy disk. How
         * aggressively egalitarian.
         */
        media       = 0x04,
        /**
         * @brief This exists on only systems where the EFI firmware can also boot
         * a legacy BIOS device. While it's included in the standard, we don't
         * actually need it or have to care about its existance.
         *
         * Just like BIOS in a few years.
         */
        bios        = 0x05,
        /**
         * @brief End of the device path, allegedly there are multiple types of
         * endings to the device path.
         *
         */
        end_of_path = 0x7F,
    };

    /**
     * @brief Different types of hardware device path.
     *
     */
    enum struct hardware_device_path_subtype : uint8 {
        /**
         * @brief It's connected to the same bus as your imaginary RTX 4090.
         *
         */
        pci        = 0x01,
        /**
         * @brief It's connected to the same bus as your imaginary 3DFX card.
         *
         * PCCARD => forerunner to PCI.
         */
        pccard     = 0x02,
        /**
         * @brief It looks like it's in RAM.
         *
         */
        mmio       = 0x03,
        /**
         * @brief Oh, it's {GUID}? I totally and natively understand 128-bit numbers!
         * (just contains a series of bytes that gain meaning with the GUID, the
         * same GUID is supposed to mean it's compatible)
         */
        vendor     = 0x04,
        /**
         * @brief EFI actually calls this a controller with no additional context
         *
         */
        controller = 0x05,
        /**
         * @brief Basebaord Management Controller, BMC.
         *
         */
        bmc        = 0x06,
    };

    /**
     * @brief Different types of ACPI device path.
     *
     */
    enum struct acpi_device_path_subtype : uint8 {
        /**
         * @brief We use two 32-bit numbers to describe the device, this can be
         * sufficient, but ACPI uses 3 32-bit numbers and 3 strings.
         *
         */
        short_path    = 0x01,
        /**
         * @brief The exact ACPI device that we have to talk to to get the file.
         *
         */
        expanded_path = 0x02,
        /**
         * @brief A list of addresses that the device uses. These are 32-bit
         * addreses. Where did the missing 4 to 32 bits go? Your guess is as
         * good as mine.
         */
        address       = 0x03,
        /**
         * @brief Something involving NVMe, I wasn't really paying attention.
         */
        nvdimm        = 0x04,
    };

    /**
     * @brief Different types of messaging device path.
     *
     */
    enum struct messaging_device_path_subtype : uint8 {
        /**
         * @brief Ancient disk language that isn't even on a serial bus! (scoffs
         * in processor not using serial bus to talk to motherboard).
         */
        atapi               = 0x01,
        /**
         * @brief The one that has all the daisy chains and is used by the original
         * macs.
         */
        scsi                = 0x02,
        /**
         * @brief Never heard of it, but it probably uses a fiber-optic cable.
         *
         */
        fibre_channel       = 0x03,
        /**
         * @brief Actually called 1394 in the EFI standard with the only explanation
         * being that 1394 is some other specification. Never heard of it before.
         */
        spec_1394           = 0x04,
        /**
         * @brief A USB device. Fun fact: while USB has been universal and serial,
         * it has not been both at the same time if you require serial busses to
         * only send one bit at a time (usb3 and later can do multiple channels
         * at once, so ***technically*** no longer serial).
         *
         */
        usb_device          = 0x05,
        /**
         * @brief Probably stands for inter-input output, like how i2c stands for
         * inter-integrated circuit.
         *
         */
        i2o                 = 0x06,
        // RIP the numbers 7 and 8.
        /**
         * @brief InfiniBand is probably some common, previously proprietary standard.
         * But the amount of bits it ends up using to describe who talks to whom
         * might suggest infinite devices to connect.
         *
         */
        infiniband          = 0x09,
        /**
         * @brief The various standards that you can just know. Your friend the
         * VT-100 terminal sits here. The TL;DR on the VT-100 is that it was so
         * popular that modern console applications talk to the console as if
         * the console was a real VT-100... unless you're on Windows, then you
         * have to ask Windows to make it a VT-100.
         *
         */
        vendor              = 0x0A,
        /**
         * @brief Media Access Control address. Technically not a messaging standard
         * on its own, I guess it doesn't really fit anywhere else.
         *
         */
        mac_address         = 0x0B,
        /**
         * @brief The IP address that only allows ~4 billion devices on the internet.
         * The average amount of internet-capable devices per person is at least
         * 2. So there is one IPv4 address per at least 4 devices on the internet.
         * Good Standard!
         *
         */
        ipv4                = 0x0C,
        /**
         * @brief The IP address made by Cisco, who then made their own IPv6
         * address read something like "cisco dog food" while testing. (eating
         * your own dog food is a software-engineering term for using your own
         * product while developing it).
         *
         */
        ipv6                = 0x0D,
        /**
         * @brief UART. The standard older than dirt. It works too well to kill,
         * however. Designed for mainframes to talk to terminals.
         *
         */
        uart                = 0x0E,
        /**
         * @brief What type of USB device we're talking to.
         */
        usb_class           = 0x0F,
        /**
         * @brief Something about USB and (maybe) internet. I don't know why it's
         * a world wide id instead of a globally unique id or a universally unique
         * id. But what about "mostly unique" or "galactically unique"?
         *
         */
        usb_wwid            = 0x10,
        /**
         * @brief Allows us to say "it looks like a disk" when it's not a disk.
         * As for why the paths with the DLU make it either 8-bits, 16-bits, or
         * 64-bits, it's because reasons.
         *
         */
        device_logical_unit = 0x11,
        /**
         * @brief Serial ATA, finally! Some good, modern, standards older than
         * me!
         */
        sata                = 0x12,
        /**
         * @brief Some internet-based SCSI that (Microsoft?) made.
         *
         */
        iscsi               = 0x13,
        /**
         * @brief (Virtual?) Local Area Network
         *
         */
        vlan                = 0x14,
        /**
         * @brief A fibre channel, but one of it's fields is an array of length
         * 8 of bytes instead of an 8-byte long number. I thought about placing
         * the two types in a union because honestly, they're the same size and
         * all the variables are the same size.
         *
         */
        fibre_channel_ex    = 0x15,
        /**
         * @brief No idea where the non-extended SAS is. I guess EFI only allows
         * the extended version.
         */
        sas_extended        = 0x16,
        /**
         * @brief Something about those fancy drives Windows 11 can connect
         * directly to the graphics card.
         *
         */
        nvme_namespace      = 0x17,
        /**
         * @brief It's definitely, 100% not a URL. It's clearly a .. uh .. *checks notes*
         * .. identifier.
         *
         */
        uri                 = 0x18,
        /**
         * @brief EFI doesn't tell us what this one stands for either.
         *
         */
        ufs                 = 0x19,
        /**
         * @brief SD Card. The thing where some devices won't read the card if it's
         * the wrong size! (how user-friendly!)
         *
         */
        sd                  = 0x1A,
        /**
         * @brief The tooth that's blue.
         *
         */
        bluetooth           = 0x1B,
        /**
         * @brief The faster thing that shares frequencies with the bluetooth.
         *
         */
        wifi                = 0x1C,
        /**
         * @brief Presumably a predecessor to the secure digital cards. Perhaps
         * this card is analog? Perhaps it's not secure? Who knows!!
         *
         */
        emmc                = 0x1D,
        /**
         * @brief Low Energy Bluetooth since EFI cares about the difference.
         *
         */
        bluetooth_le        = 0x1E,
        /**
         * @brief Cause of at least 75% of internet errors. Can set up to redirect
         * entire neighborhoods from google.com to some other website if you're
         * fast enough.
         *
         */
        dns                 = 0x1F,
        /**
         * @brief I think it's related to the NVDIMM mentioned in the ACPI path
         * types. Perhaps also related to NVME?
         *
         */
        nvdimm_namespace    = 0x20,
        /**
         * @brief Redfish something something services.
         *
         */
        rest_service        = 0x21,
        /**
         * @brief NVMe Over Fabric, in case you want your disk to connect to
         * your computer through a bed-sheet.
         *
         */
        nvmeof_namespace    = 0x22,

        /**
         * @brief Alias for when it's a normal REST service.
         *
         */
        standard_rest_service = rest_service,
        /**
         * @brief Alias for when there's something special with the REST service.
         *
         */
        vendor_specific_rest  = rest_service,
    };

    /**
     * @brief Different types of media device path.
     *
     */
    enum struct media_device_path_subtype : uint8 {
        /**
         * @brief A hard drive. EFI doesn't care if it's a floppy disk or an
         * NVMe. It's a hard-drive.
         *
         */
        hard_drive            = 0x01,
        /**
         * @brief An El Torito (little bull) boot rom. These are on CDs and can
         * be used to load an OS from.
         *
         */
        el_torito             = 0x02,
        /**
         * @brief A special type of media that we're supposed to know based on a
         * 128 bit number.
         *
         */
        vendor_defined        = 0x03,
        /**
         * @brief the /home/homework.zip in /home/homework.zip.
         *
         */
        file_path             = 0x04,
        /**
         * @brief We have to talk to some driver to get the file. Weren't we
         * already doing that?
         *
         */
        media_protocol        = 0x05,
        /**
         * @brief Firmware file. PIWG stands for something but it shouldn't be
         * relavent to an OS loader.
         *
         */
        piwg_file             = 0x06,
        /**
         * @brief Firmware volume. PIWG stands for something but it shouldn't be
         * relavent to an OS loader.
         *
         */
        piwg_volume           = 0x07,
        /**
         * @brief A beginning and ending location on thing.
         *
         */
        relative_offset_range = 0x08,
        /**
         * @brief It's a "disk" that's really memory we're reading like as if it's
         * a disk.
         *
         */
        ram_disk              = 0x09,
    };

    /**
     * @brief A disk for when the machine also supports BIOS. I don't know if
     * Garbage OS's EFI-specific loader will ever see the device.
     *
     */
    enum struct bios_boot_specification_subtype : uint8 {
        /**
         * @brief OOPS! All BIOS!
         *
         */
        bios = 0x01,
    };

    /**
     * @brief Allow the device paths to simply say they have a subtype so we don't
     * have to explicitly cast to the correct subtype.
     *
     */
    union device_path_subtype {
        /**
         * @brief When we read that it's a hardware device path.
         *
         */
        hardware_device_path_subtype    as_hardware;
        /**
         * @brief When we read that it's a software device path.
         *
         */
        acpi_device_path_subtype        as_acpi;
        /**
         * @brief When we read it's a messaging device path.
         *
         */
        messaging_device_path_subtype   as_messaging;
        /**
         * @brief When we read it's a media device path.
         *
         */
        media_device_path_subtype       as_media;
        bios_boot_specification_subtype as_bios;
    };

    /**
     * @brief Neater than writing something like typedef status EFI_API (*install_multiple_protocol_interfaces)(handle,
     * ...).
     *
     * @tparam returns what the function returns.
     * @tparam params the parameters. void is not allowed here.
     */
    template <typename returns, not_void... params> using callback = returns EFI_API ( * )( params... );

    /**
     * @brief efi_main is one of these.
     *
     */
    using image_entry_point = callback<status, handle, system_table *>;
    /**
     * @brief Function that is called when an event happens.
     *
     */
    using event_notify      = callback<void, event, void *>;
    /**
     * @brief How to make an event.
     *
     */
    using create_event      = callback<status, uint32, task_priority_level, event_notify, void *, event *>;
    /**
     * @brief A more advanced how to create an event.
     *
     */
    using create_event_ex = callback<status, uint32, task_priority_level, event_notify, void const *, guid const *, event *>;
    /**
     * @brief How to say that we no longer need an event.
     *
     */
    using close_event                   = callback<status, event>;
    /**
     * @brief Get an event to run.
     *
     */
    using signal_event                  = callback<status, event>;
    /**
     * @brief Wait for an event to run.
     *
     */
    using wait_for_event                = callback<status, uintn, event *, uintn *>;
    /**
     * @brief Check if an event is ... running? Aren't we running in a single-threaded
     * environment!?
     *
     */
    using check_event                   = callback<status, event>;
    /**
     * @brief Sets (or cancels) a timer that will trigger the specified event
     *
     */
    using set_timer                     = callback<status, event, timer_delay, uint64>;
    /**
     * @brief Increases the task priority level. We're not supposed to keep the
     * TPL higher than application for long periods of time and we should not
     * try to use this function to do anything other than raise the TPL.
     *
     */
    using raise_tpl                     = callback<task_priority_level, task_priority_level>;
    /**
     * @brief Return the TPL to whatever it was before a matching call to raise_tpl.
     * No one knows what happens if we give the wrong TPL to this function.
     *
     */
    using restore_tpl                   = callback<void, task_priority_level>;
    /**
     * @brief Allocate memory in 4096 byte chunks aligned at a 4096 byte address
     * of a certain type and at either the address we want, at least the address
     * we want, the highest address possible, or whatever address.
     *
     */
    using allocate_pages                = callback<status, allocate_type, memory_type, uintn, physical_address *>;
    /**
     * @brief Reverses a call to allocate_pages. We need to remember how many
     * pages we allocated, however.
     *
     */
    using free_pages                    = callback<status, physical_address, uintn>;
    /**
     * @brief Gets the memory map. In a low% EFI boot speedrun, this function
     * call is one of two required calls to exit boot services.
     *
     */
    using get_memory_map                = callback<status, uintn *, memory_descriptor *, uintn *, uintn *, uint32 *>;
    /**
     * @brief Allocates however many bytes of memory starting at whatever address
     * the firmware wants it to be. We can control the memory type, however.
     *
     */
    using allocate_pool                 = callback<status, memory_type, uintn, void **>;
    /**
     * @brief Returns memory allocated with allocate_pool
     *
     */
    using free_pool                     = callback<status, void *>;
    /**
     * @brief Installs a protocol interface on a device handle
     *
     */
    using install_interface             = callback<status, handle *, guid *, interface_type, void *>;
    /**
     * @brief Removes a protocol interface from a device handle
     *
     */
    using uninstall_interface           = callback<status, handle, guid *, void *>;
    /**
     * @brief Reinstalls a protocol interface on a device handle.
     */
    using reinstall_interface           = callback<status, handle, guid *, void *, void *>;
    /**
     * @brief Registers an event to be signal when an interface is installed for a
     * specific protocol
     *
     */
    using register_notify               = callback<status, guid *, event, void **>;
    /**
     * @brief Returns an array of handles for a specified protocol
     *
     */
    using locate_handle                 = callback<status, locate_search_type, guid *, void *, uintn *, handle *>;
    /**
     * @brief Checks if a handle supports a protocol.
     *
     */
    using handle_protocol               = callback<status, handle, guid *, void **>;
    /**
     * @brief Locates all devices on a device path that support a specified protocol and returns the one that is
     * "cloest to the path"
     *
     */
    using locate_handle_on_device_path  = callback<status, guid *, device_path **, handle *>;
    /**
     * @brief Adds elements to the list of things using the protocol interface specified
     *
     */
    using open_protocol                 = callback<status, handle, guid *, void **, handle, handle, uint32>;
    /**
     * @brief Removes elements from the list of things using the protocol interface specified.
     *
     */
    using close_protocol                = callback<status, handle, guid *, handle, handle>;
    /**
     * @brief Gets the list of things that have opened a specified protocol
     *
     */
    using open_protocol_information     = callback<status, handle, guid *, open_protocol_information_entry **, uintn *>;
    /**
     * @brief Connects the best set of drivers to a controller.
     *
     */
    using connect_controller            = callback<status, handle, handle *, device_path *, bool>;
    /**
     * @brief Informs a set of drivers to stop talking to a controller.
     *
     */
    using disconnect_controller         = callback<status, handle, handle, handle>;
    /**
     * @brief Gets a buffer of how mnay protocols are active on a specified handle.
     *
     */
    using protocols_per_handle          = callback<status, handle, guid ***, uintn *>;
    /**
     * @brief Gets a list of handles in a database that meet a specific set of criteria
     *
     */
    using locate_handles_for_protocol   = callback<status, locate_search_type, guid *, void *, uintn *, handle **>;
    /**
     * @brief Finds the first handle in a database that supports the requested protocol
     *
     */
    using locate_protocol               = callback<status, guid *, void *, void **>;
    /**
     * @brief Installs (potentially) more than one interface to a handle
     *
     */
    using install_multiple_interfaces   = callback<status, handle *, std::va_list>;
    /**
     * @brief Uninstalls (poetnatially) more than oen interface to a handle
     *
     */
    using uninstall_multiple_interfaces = callback<status, handle, std::va_list>;
    /**
     * @brief Loads an image from a path. If the loader ever calls this function,
     * it will set the boolean parameter to false.
     */
    using load_image                    = callback<status, bool, handle, device_path *, void *, uintn, handle *>;
    /**
     * @brief Starts an image. Presumably it remains present in RAM once it returns?
     *
     */
    using start_image                   = callback<status, handle, uintn *, char16 **>;
    /**
     * @brief Unloads an image. Gets rid of the thing in memry. It also releases all of its allocatred resources.
     *
     */
    using unload_image                  = callback<status, handle>;
    /**
     * @brief We need to give control back to the firmware.
     *
     */
    using exit                          = callback<status, handle, status, uintn, char16 *>;
    /**
     * @brief We're telling the firmware that WE'RE in control.
     *
     */
    using exit_boot_services            = callback<status, handle, uintn>;
    /**
     * @brief Sets or modifies the watchdog timer. On boot, we have 5 minutes
     * to either change the timer's value or exit boot services -- and we're not
     * touching this function unless we need less time.
     *
     */
    using set_watchdog_timer            = callback<status, uintn, uint64, uintn, char16 *>;
    /**
     * @brief Have the processor constantly check if however many microseconds
     * have passed since we called this function.
     *
     */
    using stall                         = callback<status, uintn>;
    /**
     * @brief Copies memory from one place to another.
     *
     */
    using copy_memory                   = callback<void, void *, void *, uintn>;
    /**
     * @brief Sets memory to be the byte we ask for.
     *
     */
    using set_memory                    = callback<void, void *, uintn, uint8>;
    /**
     * @brief Gets the next counter that's only supposed to reach each number
     * once. In reality, it repeats every 4 billion times we call it between system
     * resets and repeats evry 4 billion resets.
     *
     */
    using get_next_monotonic_count      = callback<status, uint64 *>;
    /**
     * @brief Adds a configuration table. We don't need to do this.
     *
     */
    using install_configuration_table   = callback<status, guid *, void *>;
    /**
     * @brief Calculates the CRC 32. Would be useful if we didn't need to calculate
     * 2 CRC-32s to check if this function is safe.
     *
     */
    using calculate_crc32               = callback<status, void *, uintn, uint32 *>;

    /**
     * @brief What time is it.
     *
     */
    using get_time                = callback<status, time *, time_capabilities *>;
    /**
     * @brief Turn the computer into a time machine.
     *
     */
    using set_time                = callback<status, time *>;
    /**
     * @brief Set an alarm clock.
     *
     */
    using get_wakeup_time         = callback<status, bool *, bool *, time *>;
    /**
     * @brief Anxiously check the alarm clock to realize how little sleep we're
     * getting.
     *
     */
    using set_wakeup_time         = callback<status, bool, time *>;
    /**
     * @brief Move memory around, or at least look like we did it.
     *
     */
    using set_virtual_address_map = callback<status, uintn, uintn, uint32, memory_descriptor *>;
    /**
     * @brief Move this pointer that's at this address we're telling you it's at.
     *
     */
    using convert_pointer         = callback<status, uintn, void **>;

    /**
     * @brief types of events. These are supposed to be bit-flags.
     *
     */
    namespace event_types {
        /**
         * @brief This event corresponds to a timer.
         *
         */
        constexpr uint32 timer                          = 0x8000'0000;
        /**
         * @brief This event has something to do with runtime services.
         *
         */
        constexpr uint32 runtime                        = 0x4000'0000;
        /**
         * @brief This event runs when we wait for it to run... but then why do
         * we wait for it? That's just calling a function but with extra steps!
         *
         */
        constexpr uint32 notify_wait                    = 0x0000'0100;
        /**
         * @brief This event runs when we signal it to run... that's calling
         * a function but with extra steps!
         *
         */
        constexpr uint32 notify_signal                  = 0x0000'0200;
        /**
         * @brief This function is signaled when we exit boot services. Notice
         * that it should also run when we signal it
         *
         */
        constexpr uint32 signal_exit_boot_services      = 0x0000'0201;
        /**
         * @brief This function runs when there is a change in the virtual
         * address map. Would be useful if events were supported once we have
         * control of the virtual address map.
         *
         */
        constexpr uint32 signal_virtual_adddress_change = 0x0000'0202;
    }    // namespace event_types

    /**
     * @brief Different attributes that memory can have.
     *
     */
    namespace memory_attributes {
        /**
         * @brief CPU cannot write down what this value is. We have to get this tool
         * from the hardware store every time we need it.
         *
         */
        constexpr uint64 uncacheable            = uint64( 1 ) << 0;
        /**
         * @brief CPU CAN write down what this value is. Not sure what the combining
         * part is all about.
         *
         */
        constexpr uint64 write_combining        = uint64( 1 ) << 1;
        /**
         * @brief CPU can remember what this value is, but if it changes the value
         * that always has to go through to memory.
         *
         */
        constexpr uint64 write_through          = uint64( 1 ) << 2;
        /**
         * @brief CPU writes this memory to RAM when it's getting rid of the
         * memory. Useful for performance, not useful for when we want to talk
         * to something that looks like memory.
         *
         */
        constexpr uint64 write_back             = uint64( 1 ) << 3;
        /**
         * @brief It's not cacheable, and it's .. uh .. exported. I forgot what
         * that means again.
         *
         */
        constexpr uint64 uncacheable_exported   = uint64( 1 ) << 4;
        /**
         * @brief We can write to it, but when we do so, the changes are immediately
         * propogated to all copies of the memory. This doesn't refer to memory
         * permissions but instead to thread safety.
         *
         */
        constexpr uint64 write_protected        = uint64( 1 ) << 12;
        /**
         * @brief "See no Evil"
         *
         */
        constexpr uint64 read_protected         = uint64( 1 ) << 13;
        /**
         * @brief "Do no Evil"
         *
         */
        constexpr uint64 execute_protected      = uint64( 1 ) << 14;
        /**
         * @brief It remembers its value even when we turn it off!
         *
         */
        constexpr uint64 nonvolatile            = uint64( 1 ) << 15;
        /**
         * @brief Unlike that "less reliable" memory, this memory should... that's
         * a scary implication that the RAM could be unreliable.
         *
         */
        constexpr uint64 more_reliable          = uint64( 1 ) << 16;
        /**
         * @brief This is the actual "write protected" bit. Meaning that we can
         * tell the processor that applications can't write here.
         *
         */
        constexpr uint64 read_only              = uint64( 1 ) << 17;
        /**
         * @brief This is indeed memory, but it's a special memory that's slower
         * if you use it wrong.
         *
         */
        constexpr uint64 specific_purpose       = uint64( 1 ) << 18;
        /**
         * @brief The CPU can turn this into gibberish and back.
         *
         */
        constexpr uint64 cpu_encrypt_support    = uint64( 1 ) << 19;
        /**
         * @brief This memory must be mentioned in the virtual address map, but
         * what memory isn't mentioned in the virtual address map?
         *
         */
        constexpr uint64 runtime                = uint64( 1 ) << 63;
        /**
         * @brief Whether certain special bits between 62 and like 30-something
         * actually mean anything.
         *
         */
        constexpr uint64 has_arch_specific_info = uint64( 1 ) << 62;

        /**
         * @brief The special bits that might not exist and their meanings, as
         * determined by the processor the loader is running on right now.
         *
         * No, we don't have to do a runtime check for what these flags mean if
         * set.
         *
         */
        constexpr uint64 arch_specific_flags_bitmask = ( uint64( 0x0FFF'F000 ) << 32 ) + 0x0000'0000;

        // TODO insert the arch specific flags bits here.
    }    // namespace memory_attributes

    /**
     * @brief Attributes involving open protocols. These are the attributes in
     * the open_protocol function.
     *
     */
    namespace open_protocol_attributes {
        /**
         * @brief If this bit is set, then calling OpenProtocol is the same as calling HandleProtocol
         *
         */
        constexpr uint32 by_handle_protocol  = 1 << 0;
        /**
         * @brief If this bit is set, then we are assumed to be a driver looking for the protocol interface to a
         * specific handle.
         *
         */
        constexpr uint32 get_protocol        = 1 << 1;
        /**
         * @brief If this bit is set, then we are assumed to be looking if a protocol interface exists on a handle.
         *
         */
        constexpr uint32 test_protocol       = 1 << 2;
        /**
         * @brief If this bit is set, we are assumed to be a bus driver (no, wires not the four-wheeled vehicle) and we
         * are showing the firmware that we are using the handle. I'm not sure if showign is actually the correct word
         * here, but it's the one that the EFI standard uses.
         *
         */
        constexpr uint32 by_child_controller = 1 << 3;
        /**
         * @brief If this bit is set, we are a driver looking to gain access to a protocol interface. If this is set
         * along iwth the exclusive bit, then we want exclusive access and the firmware calls disconnect_controller on
         * anyone using the thing.
         *
         */
        constexpr uint32 by_driver           = 1 << 4;
        /**
         * @brief If the exclusive bit is set, then we will try to kick out everyone else using the protocol interface.
         *
         */
        constexpr uint32 exclusive           = 1 << 5;
    }    // namespace open_protocol_attributes

    /**
     * @brief Convenient grouping of all the known revisions.
     *
     */
    namespace revisions {
        /**
         * @brief Gets the revision number from some major and minor revision
         *
         *
         * @param major the number before the first decimal point
         * @param minor all the other numbers
         * @return the revision number.
         */
        consteval uint32 version_number( uint16 major, uint16 minor ) {
            return ( uint32( major ) << 16 ) | minor;
        }

        /**
         * @brief latest revision as of April 2023
         *
         */
        constexpr uint32 version_2_10 = version_number( 2, 100 );
        /**
         * @brief Latest revision as of 2021-2022
         *
         */
        constexpr uint32 version_2_9  = version_number( 2, 90 );
        /**
         * @brief Latest revision as of 2021
         *
         */
        constexpr uint32 version_2_8  = version_number( 2, 80 );
        /**
         * @brief Revision that was replaced before development of Garbage OS
         * began in any way shape or form.
         *
         */
        constexpr uint32 version_2_7  = version_number( 2, 70 );
        /**
         * @brief Version 2.6, coincidentally the last one I made the documentation
         * comment for, unless if any version after 2.11 is also in this namespace.
         *
         */
        constexpr uint32 version_2_6  = version_number( 2, 60 );
        /**
         * @brief Your computer is likely newer than this.
         *
         */
        constexpr uint32 version_2_5  = version_number( 2, 50 );
        /**
         * @brief Adds features that were not present in 2.3
         *
         */
        constexpr uint32 version_2_4  = version_number( 2, 40 );
        /**
         * @brief Creates a streak of four updates without a major revision.
         *
         */
        constexpr uint32 version_2_3  = version_number( 2, 30 );
        /**
         * @brief Adds features that were not present in 2.1
         *
         */
        constexpr uint32 version_2_2  = version_number( 2, 20 );
        /**
         * @brief Adds features that were not present in 2.0
         *
         */
        constexpr uint32 version_2_1  = version_number( 2, 10 );
        /**
         * @brief It's now "Unified"
         *
         */
        constexpr uint32 version_2_0  = version_number( 2, 0 );
        /**
         * @brief version 1.1 because they immediately remembered some things
         * they forgot or something.
         *
         */
        constexpr uint32 version_1_1  = version_number( 1, 10 );
        /**
         * @brief Everything from ROM-hacks to commercial firmware becomes fully
         * functional only in version 1.02.
         *
         */
        constexpr uint32 version_1_02 = version_number( 1, 2 );
    }    // namespace revisions

    /**
     * @brief The various signatures of the three most important tables in the
     * firmware.
     *
     */
    namespace signatures {
        /**
         * @brief The system table. It's a problem if the signature field is
         * not this number. I have no idea where the signature field came from.
         *
         */
        constexpr uint64 system_table  = ( uint64( 0x5453'5953 ) << 32 ) + 0x2049'4249;
        /**
         * @brief the boot services. It's a problem if the signature field is not
         * this number and the table is where Boot Services should be. I have
         * no idea where EFI got this particular number.
         *
         */
        constexpr uint64 boot_services = ( uint64( 0x5652'4553 ) << 32 ) + 0x544f'4f42;

    }    // namespace signatures

    /**
     * @brief The header common to the System Table, Boot Services, and
     * Runtime Services. Due to oddities of how C++ handles structures, we tell
     * the compiler that those structures are types of table header. The wonders
     * of object orientated programming.
     *
     */
    struct EFI_CLASS table_header {
        /**
         * @brief The table signature. It may be a slight issue if it's not something
         * we recognize.
         *
         */
        uint64 signature;
        /**
         * @brief The table revision. This is actually always equal to the firmware
         * revision, but only indicates a different table size for boot_serviecs.
         *
         */
        uint32 revision;
        /**
         * @brief How many bytes long the table is. Thankfully for those who make
         * header files, we're nowhere close to the half-billion functions needed
         * to exceed 4 GiB.
         *
         */
        uint32 size;
        /**
         * @brief The 32-bit CRC. EFI explicitly uses the same CRC parameters as
         * the IEEE standard that commonly defines the CRC. If we set this value
         * to zero and calculate the CRC from the beginning of the header to the
         * beginning plus size then we should get a CRC that matches this one.
         *
         * In English, the table must add up.
         *
         */
        uint32 crc;
        /**
         * @brief Reserved, probably for alignment reasons, probably to fit in
         * another thing. Who knows!
         *
         */
        uint32 reserved_header_field;
    };

    /**
     * @brief The system table. All the inforamtion we need should be here.
     *
     * @note I know it says it's a table header, that just means we can look at it
     * as if it is a table header.
     *
     */
    struct EFI_CLASS system_table : public table_header {
        /**
         * @brief UTF-16 string that says who made the firmware. We're expecting
         * values like "Dell" or "Definitely not Chinese Spyware" (just kidding
         * about that second one). But the firmware vendor should not affect the
         * performance unless common EFI implementations are broken in some way.
         *
         * @note A broken implementation would be something like how certain
         * Award BIOS revisions have a BIOS function that "forgets" to pop a
         * value from the stack, causing the processor to load the wrong flags
         * and return to THE WRONG PLACE.
         *
         */
        efi::char16              *firmware_vendor;
        /**
         * @brief 32-bit number that identifies the firmware revision. While
         * the vendor can really put whatever value they want here, we're supposed
         * to consider higher values as more recent revisions. Like with the
         * firmware vendor field, the revision should not affect the performance
         * and we actually don't look at this field when determining the EFI
         * version (we instead look at the table revisions). This field would be
         * important if, for example, "FooBar Co."'s revision 1234 firmware has
         * a serious defect and certian functions just crash.
         *
         */
        efi::uint32               firmware_revision;
        /**
         * @brief The everything-is-a-file-thing that corresponds to how we get
         * console input.
         *
         * @note This is required to be here, but not required to do anything.
         *
         */
        efi::handle               console_in_handle;
        /**
         * @brief The simple_text_input device that corresponds to the console
         * input. i.e., std::cin or /dev/stdin but in the EFI firmware.
         *
         * @note This is required to be here and have functions safe to call when
         * in boot services. It is not required to do anything.
         */
        efi::simple_text_input   *console_in;
        /**
         * @brief The everything-is-a-file-thing that corresponds to how we get
         * console output.
         *
         * @note This is required to be here, but not required to do anything.
         *
         */
        efi::handle               console_out_handle;
        /**
         * @brief The simple_text_input device that corresponds to the console
         * output. i.e., std::cout or /dev/stdout but in the EFI firmware.
         *
         * @note This is required to be here and have functions safe to call when
         * in boot services. It is not required to do anything.
         */
        efi::simple_text_output  *console_out;
        /**
         * @brief Probably the console output handle, but we can never be too
         * sure.
         *
         */
        efi::handle               standard_error_handle;
        /**
         * @brief Probably the console output device, but we can never be too sure.
         *
         */
        efi::simple_text_output  *standard_error;
        /**
         * @brief The runtime services. These functions tell us if they work
         * after exiting boot services. However, it's possible that none of them
         * work. Regardless, unless we delete the structures in runtime services,
         * these functions are guaranteed safe to call at all times if the table
         * is valid.
         *
         */
        efi::runtime_services    *runtime_services;
        /**
         * @brief The boot services. Functions that are actually quite helpful
         * and make EFI's boot environment almost like an operating system itself.
         *
         * @note It's not uncommon amongst hobbyist OS developement to remain in
         * Boot Services indefinitely! However, we don't want that since the system
         * is not designed to remain in boot services for long periods of time.
         *
         */
        efi::boot_services       *boot_services;
        /**
         * @brief How many entries .. er .. configuration tables there are in the
         * configuration table. Damn, Software Engineers are good at consistent
         * naming.
         *
         */
        efi::uintn                configuration_entry_count;
        /**
         * @brief The "array" of configuration tables. In the most confusing way
         * possible, EFI guarantees that we can at least access the first table
         * even if there are no entries.
         *
         * @note EFI says "it is guaranteed that at least the first table is
         * addressible".
         *
         * @note being able to access the first table is important on x86-based
         * systems since the modes that EFI loads from require memory protections...
         * and both the firmware and the loader can violate these memory protections.
         *
         */
        efi::configuration_table *configuration;
    };

    /**
     * @brief A big table of functions that are active until something calls
     * exit_boot_services.
     *
     * @note In a mildly-ironic way, exit_boot_serviecs is one of the functions
     * that is only active during boot_services.
     *
     */
    struct EFI_CLASS boot_services : public table_header {
        /**
         * @brief Raises the current task priority level to one we specify
         * @param tpl the new task priority level
         * @return our previous task priority level
         * @warning Strange things will happen if we don't return to the old one
         * @warning EFI explicitly says the firmware can do whatever it wants if
         * we try to lower the tpl using this function.
         *
         */
        efi::raise_tpl                    raise_tpl;
        /**
         * @brief Restores the TPL to what it was before calling raise_tpl.
         * @param tpl the old tpl that we want to return to.
         * @warning EFI explicitly says that strange things can happen if we don't
         * return to the correct TPL.
         */
        efi::restore_tpl                  restore_tpl;
        /**
         * @brief Allocates pages for use by whoever calls the function.
         * @param allocate_type the type of allocation to perform, that is, what does the
         * firmware make of the address passed in.
         * @param memory_type what type of memory to allocate
         * @param uintn the amount of pages to allocate.
         * @param memory a pointer to a physical address that the location of the
         * allocated buffer (if the function succeeds). Depending on the allocate_type,
         * the firmware may or may not put a different location here than where
         * we ask for the allocation.
         * @return success - allocation happens successfully
         * @return out_of_resources - there is not enough free, contiguous RAM
         * to do the allocation (really bad if we ask for any pages)
         * @return invalid_parameter - type is out of bounds, memory_type is
         * reserved for non-OS use, memory_type is peristent or unaccepted, the
         * pointer to the memory location is zero,
         * @return not_found if "the requested pages could not be found".
         * @note EFI defines a page to be 4096 bytes. So, this function can only
         * allocate chunks of 4096 bytes and these must be aligned on a multiple
         * of 4096 bytes.
         */
        efi::allocate_pages               page_malloc;
        /**
         * @brief frees pages allocated with allocate_pages.
         * @param memory the start of the page buffer
         * @param pages how many contiguous pages to free
         * @return success if the pages were freed
         * @return not_found if the pages were not allocated with a call to
         * allocate_pages
         * @return invalid_parameter if memory is not a multiple of the page size
         * or if "pages is invalid".
         */
        efi::free_pages                   page_free;
        /**
         * @brief Get ahold of the memory map.
         * @param memory_map_size the size of the buffer we give to the firmware
         * to put the memory map into. If the buffer is too small, it gets set
         * to the minimum size required.
         * @param memory_map the start of the memory map buffer.
         * @param mak_key a special magic number that changes on each allocation.
         * We need the most recent one to exit boot services.
         * @param descriptor_size the size of each memory descriptor. The firmware
         * tells us this size, not the other way around.
         * @param descriptor_version the vrsion of the version number associated
         * with the memory descriptor. Currently 1 but future versions are implied
         * backward-compatible.
         * @return success - we were given the memory map
         * @return buffer_too_small - the buffer was not the correct size to fit
         * the memory map but we were told how large it needs to be
         * @return invalid_parameter if the pointer to the size is null or if the
         * buffer is supposed to be the right size but we tell the firmware to
         * put the map at address 0 (i.e., nullptr).
         */
        efi::get_memory_map               get_memory_map;
        /**
         * @brief Allocates a memory pool.
         * @param memory_type the type of memory to allocate
         * @param uintn the amount of bytes to allocate
         * @param void **the memory address of the start of the buffer we want
         * to allocate.
         * @return success - all bytes were allocated
         * @return out_of_resources - could not allocate the memmory
         * @return invalid_parameter: memory_type was out of range, buffer is
         * nullptr, or we told the firmware to allocate persistent_memory.
         */
        efi::allocate_pool                pool_malloc;
        /**
         * @brief Undoes a call to allocate pool
         * @param void * the start of the buffer that was allocated.
         * @return success - I'll let you guess what it means.
         * @return invalid_paramter - We can't return the buffer because it either
         * isn't allowed to be returned, or was not taken using pool_malloc.
         */
        efi::free_pool                    pool_free;
        /**
         * @brief Creates an event.
         * @param type the type of event.
         * @param notify_tpl how important the notifications are, higher is better
         * but we cannot use all the values.
         * @param notify_function the function that the event calls, optional
         * @param notify_context the context that the notify function uses.
         * @param event the resulting event.
         */
        efi::create_event                 create_event;
        /**
         * @brief Sets a timer that will signal an event.
         * @param event the event to signal when the timer expires.
         * @param type how to interpret the timer delay
         * @param trigger_time a duration in 100 ns units. So, a value of 10 means
         * one microsecond.
         * @return success the event will signal at the requested time, or, if
         * we requested the timer to be canceled, the timer has been canceled.
         * @return invalid_parameter event or type are invalid.
         */
        efi::set_timer                    set_timer;
        /**
         * @brief Waits for any of the events in a provided list to be signaled
         * and indicates which event in the list was signaled.
         * @param number_of_events how many events are in the provided list
         * @param events the list of events
         * @param index which event allowed us to continue running first.
         * @return success we waited for those events successfully.
         * @return invalid_parameter the number of events was zero or the event
         * that satisfied the condition was an event that needed to be signaled
         * @return unsupported we are not running at the application level.
         */
        efi::wait_for_event               wait_for_event_in_list;
        /**
         * @brief Signals an event.
         * @param event the event to signal.
         * @return success the event was signaled.
         * @warning it actually seems that the standard does not define what
         * happens when the event is invalid. Issue?
         */
        efi::signal_event                 signal_event;
        /**
         * @brief Closes an event.
         * @param event the event to close
         * @return success the event was closed
         * @warning it actually seems that the standard does not define what
         * happens when the event is invalid. Issue?
         */
        efi::close_event                  close_event;
        /**
         * @brief Checks if an event has been signaled. If the function has a
         * notification function, can be signaled, and is not yet signaled, then
         * check_event signals the function.
         * @param event the event to check
         * @return success - the event is signaled.
         * @return not_ready - the event has no notification function and has not
         * been signaled.
         * @return invalid_parameter - the event is of type notify_signal.
         */
        efi::check_event                  check_event;
        /**
         * @brief Installs an interface. If the handle to install the interface
         * on does not exist, it is created.
         * @deprecated use install_protocol_interfaces instead when available.
         * @param handle the handle to install to
         * @param protocol the GUID corresponding to the protocol.
         * @param interface_type the execution environment type of the request.
         * @param interface the actual protocol interface to install.
         * @return success the interface was installed.
         * @return out_of_resources the space for the new handle could not be
         * allocated.
         * @return invalid_parameter handle or protocol are nullptr, or, interface_type
         * is not native, or, protocol has already been installed on the handle.
         */
        efi::install_interface            install_interface;
        /**
         * @brief reinstalls an interface
         * @param handle the handle to reinstall the interface on
         * @param protocol the GUID corresponding to the protocol
         * @param old_interface the interfcae to remove, nullptr if the protocol
         * has no interface associated with it.
         * @param new_interface the interface to install, nullptr if the protocol
         * has no interface associated with it.
         * @return success - the interface was reinstalled
         * @return not_found - old_interface is not attached to handle.
         * @return access_denied - someone else is currently using old_interface
         * @return invalid_parameter - if handle or protocol are nullptr.
         */
        efi::reinstall_interface          reinstall_interface;
        /**
         * @brief uninstalls an interface
         * @param handle the handle t uninstall the interface from
         * @param protocol the GUID corresponding to the protocol
         * @param interface the interface to remove, nullptr if the protocol has
         * no interface associated with it.
         * @return success - the interface was removed.
         * @return not_found - the interface was "not found" (likely this means
         * that the pointer does not refer to an interface, or, the handle is
         * not connected to interface)
         * @return access_denied - someone else is currently using interface
         * @return invalid_parameter - handle or protocol are nullptr
         */
        efi::uninstall_interface          uninstall_interface;
        /**
         * @brief Asks a handle if it supports a protocol.
         * @param handle the handle to ask
         * @param protocol the protocol that we want handle to support
         * @param interface the address of the pointer to the interface. This
         * pointer is set to nullptr if the handle has no interface assocaited with the
         * protocol.
         * @return success - the handle supports the protocol and the interface
         * to use was returned through interface.
         * @return unsupported - the handle does not support the protocol.
         * @return invalid_parameter - any of our arguments are nullptr.
         */
        efi::handle_protocol              handle_protocol;
        /**
         * @brief $5 on this function still being reserved in 10 years.
         * @note I'll see you in 2033.
         */
        void                             *reserved_function;
        /**
         * @brief registers an event to be signaled whenever an interface is
         * installed on a particular protocol.
         * @param protocol the protocol we're interested in
         * @param event how we get notfied.
         * @param registration a special magic number that we need to retrieve the
         * handles that have been added to the interface when the interface is
         * installed.
         * @return success - the event has been registered.
         * @return out_of_resources - the space for the notification event could
         * not be allocated. That is, the firmware could not allocate things to
         * actually respond to the event.
         * @return invalid_parameter - any of our inputs are nullptr.
         */
        efi::register_notify              register_protocol_to_event;
        /**
         * @brief Returns an array of handles that support the protocol we ask
         * about.
         * @param search_type how we want the firmware to find the handles.
         * @param protocol an optional pointer to the GUID that we want to use
         * to search by. Required if searching by protocol.
         * @param search_key an optional parameter to the registration thing we
         * got when we called register_protocol_to_event some time earlier.
         * Required if searching by by_register_notify.
         * @param buffer_size the size of bytes in the buffer, This value gets
         * set to the amount of bytes that were writen (or needed, if the buffer
         * is too small).
         * @param buffer an array of handles that match the criteria.
         * @return success - we got the device handles we were looking for
         * @return not_found - literally nothing matches the criteria we specified
         * @return buffer_too_small - not enough space in the buffer we provided.
         * @return invalid_parameter - search_type is out of bounds, search_key is nullptr
         * for the by_register_notify mode, protocol is nullptr for the by_protocol mode,
         * buffer_size is nullptr and any amount of handles exist, or the buffer is
         * allegedly the correct size and the buffer is nullptr.
         */
        efi::locate_handle                locate_handle;
        /**
         * @brief Locates a handle on the device path that supports the protocol
         * @param protocol the protocol the handle must support.
         * @param device_path the device path, get's modified to refer to where
         * the handle is.
         * @param handle the device.
         * @return success - the firmware found the handle and gave it to us.
         * @return not_found - there is no handle matching these criteria
         * @return invalid_parameter - protocol or device_path are nullptr or
         * device was null when a match exists.
         */
        efi::locate_handle_on_device_path locate_handle_on_device_path;
        /**
         * @brief Adds, updates, or removes the configuration table associated
         * with the specified GUID.
         * @param guid the guid of the table we wish to update.
         * @param table the new value, the decicsion to add, update, or remove
         * is made based on whether the entry is in the table and whether this
         * value is nullptr or not.
         * @note If there is no entry associated with guid, and table is not nullptr,
         * then the table is added.
         * @note if there is an entry associated with guid, and table is not nullptr,
         * then the table is updated.
         * @note if there is no entry associated with guid, and table is nullptr,
         * that's an error.
         * @note if there is an entry associated with guid, and table is nullptr,
         * then the table is removed.
         * @return success - the table was added, updated, or removed (see the notes)
         * @return invalid_parameter - guid is nullptr
         * @return not_found - table was nullptr and there is no entry associated
         * with guid.
         * @return out_of_resources - the firmware could not add new entries to
         * the table.
         */
        efi::install_configuration_table  install_configuration_table;
        /**
         * @brief Loads an image into memory from a device path.
         * @param boot_policy whether this function call comes from the boot manager
         * trying to load a boot option. WE ALWAYS SET THIS TO FALSE.
         * @param handle the image of the thing trying to load the image. Needed
         * by an internally used loaded_iamge_protocol.
         * @param device_path the file path to load the image from.
         * @param source_buffer where to put the image.
         * @param source_size the size of the source buffer in bytes
         * @param image_handle the handle to the loaded image.
         * @note this function operates in one of two distinct ways:
         * @note 1. if source buffer is specified, then this function is considered to be
         * an operation to copy memory from one address to another. Additionally, when
         * source buffer is specified, the device path is optional but strongly
         * recommended since the firmware is allowed to use this path to make
         * certain security decisions.
         * @note 2. if source bufer is not specified, then this function is considered
         * to be an operation that loads the image from a file and into memory. If
         * there is a simple_file_system associated with the path, then the firmware
         * will attempt to use that, otherwise, it will attempt to use either a
         * load_file protocol (if boot_policy is true) or a load_file2 and then a
         * load_file (if boot_policy is false).
         * @return success - the image was loaded into memory
         * @return not_found we didn't specify where to find the image (i.e., both
         * source_buffer and device_path are nullptr)
         * @return invalid_parameter - image_handle or parent_image_handle is nullptr,
         * or one of the other parameters is otherwise invalid.
         * @return unsupported - we were told that the firmware does not support
         * the image type
         * @return out_of_resources - the firmware ran out of resources when
         * loading the file.
         * @return load_error - something was corrupt or otherwise went wrong in
         * the loading process.
         * @return device_error - the device we loaded from gave a read error.
         * @return access_denied - the firmware does not allow the image to be
         * loaded.
         * @return security_violation - the image was loaded, but the firmware
         * says we are not allowed to run the image.
         */
        efi::load_image                   load_image;
        /**
         * @brief Starts an image.
         * @param image_handle the image to start
         * @param exit_data_size how much information the image gave once it
         * returned control back to us.
         * @param exit_data an optional buffer to allow the image to give us
         * exit data.
         * @return invalid_parameter - the image handle has either already run once
         * before or the handle is invalid.
         * @return security_violation - the firmware said that the image is not allowed
         * to be started.
         * @return anything else - whatever the image returns. What if it happens
         * to return security violation or invalid parameter?
         */
        efi::start_image                  start_image;
        /**
         * @brief Exits the current program.
         * @param image_handle the handle to our image.
         * @param status our status code
         * @param exit_data_size how much exit data we're giving
         * @param exit_data an optional string that may be followed up by additional
         * binary data after the null terminator. This data is only valid if we
         * allcoate it with a call to allocate_pool and status != success.
         * @return success - if the image was someone else, this works like
         * unload_image.
         * @return invalid_parameter the image_handle is invalid
         * @return *nothing* - we successfully exited.
         * @note Yes. this function can error out, in theory. but if we specify
         * only our own handle, then it's REALLY bad if we fail to exit properly.
         */
        efi::exit                         exit;
        /**
         * @brief unloads an image from RAM
         *
         */
        efi::unload_image                 unload_image;
        /**
         * @brief Exits boot services. This is the only way to assume control of
         * the system.
         * @param imgae_handle the handle to the image taking control of the
         * system. That is, us.
         * @param map_key the magic number that identifies the most recent memory
         * map.
         * @return success - boot services have been teriminated
         * @return invalid_parameter map_key is wrong.
         *
         * Definitely don't do this.
         * @code {.c++}
         * boot_services->set_watchdog_timer(0, 0x1'0000, 0, nullptr);
         * for(efi::uintn key = 0; boot_services->exit_boot_services(image_handle, map_key) != 0; key++) { }
         * @endcode
         *
         */
        efi::exit_boot_services           exit_boot_services;
        /**
         * @brief Gets the next monotonic count from the firmware.
         * @param count pointer to the place to put the monotonic count.
         * @return success - monotonic count was given.
         * @return device_error - "the device is not functioning properly", well
         * yeah, we understood that part when you said "device error"
         * @return invalid_parameter - count is nullptr
         */
        efi::get_next_monotonic_count     get_monotonic_count;
        /**
         * @brief Puts the processor in a spin loop until the specified amount
         * of microseconds passes.
         * @param microseconds how long to wait.
         * @return success - this function is apparently required to work.
         */
        efi::stall                        stall;
        /**
         * @brief Sets, modifies, or cancels the watchdog timer.
         * @param timeout how many seconds until the watchdog timer expires. A
         * value of zero cancels the timer.
         * @param watchdog_code the code to log when the timer expires. The
         * firmware reserves `0x0000` through `0xFFFF`
         * @param data_size the size of watchdog_data in bytes
         * @param watchdog_data the data to log if the timer expires.
         * @return success - the timeout has been set / the timer has been
         * canceled, depending on the value of timeout.
         * @return invalid_parameter - we gave an invalid watchdog_code.
         * @return unsupported - the system lacks a watchdog timer.
         * @return device_error the watchdog timer could not be programmed due
         * to a hardware error.
         *
         * @details sample code that might attempt to set a 3-minute watchdog timer.
         * @code {.C++}
         * char16 error_message[] = u"This is the error message";
         * status result = boot_service->set_watchdog_timer(180,
         *                                                  0x1'0000,
         *                                                  sizeof(error_message),
         *                                                  error_message);
         * switch (result) {
         *  case statuses::success:
         *      break;
         *  case statuses::invalid_parameter:
         *      write(u"Warning: could not set watchdog timer because the loader-reserved code 0x1'0000 is "
         *            u"invalid. If Garbage OS takes more than 3 minutes from this point to show a logo, "
         *            u"then the system has crashed and you can safely perform a reset.\r\n");
         *      break;
         *  case statuses::unsupported:
         *      write(u"Warning: could not set the watchdog timer because your system says it does not "
         *            u"have one. If Garbage OS takes more than 3 minutes from this point to show a logo, "
         *            u"then the system has crashed and you can safely perform a reset.\r\n");
         *      break;
         *  case statuses::device_error:
         *      write(u"Warning: could not set the watchdog timer because your system reported a hardware error "
         *            u"while setting it. If Garbage OS takes more than 3 minutes from this point to show a logo, "
         *            u"then the system has crashed and you can safely perform a reset.\r\n");
         *      break;
         *  default:
         *      write(u"Warning: your system reported an error that Garbage OS did not expect when asked to "
         *            u"set the watchdog timer. This means Garbage OS does not know what will happen if "
         *            u"the loading process crashes. If Garbage OS takes more than 3 minutes from this point "
         *            u"to show a logo, then the system has likely crashed and you can safely reset it.\r\n");
         *      break;
         * }
         * @endcode
         *
         */
        efi::set_watchdog_timer           set_watchdog_timer;

        // above functions are valid in all versions of the standard. If the
        // boot services are valid, then these functions are reasonably safe
        // to call no matter the revision number of the boot services, except
        // for the reserved field.

        /**
         * @brief Connects one or more drivers to a controller.
         * @param controller_handle the controller to connect to
         * @param driver_image_handle array of handles that support the
         * driver_binding_protocol. Null terminated (recall, handle is an alias
         * for void *)
         * @param remaining_device_path the device path that specified a child
         * of the controller to connect to. If nullptr, then handles of all the
         * controller's children will be created.
         * @param recursive whether or not this function is called recursively
         * until reaching the end of the controller tree.
         * @return success - the drivers were connected, or, the device path is
         * at the very end and no drivers were connected. That is, all possible
         * devices were connected to the controller.
         * @return invalid_parameter - controller_handle is nullptr.
         * @return not_found there are no driver_binding_protocol instances available or
         * no drivers were connected to the controller handle and remaining_device_path
         * was either not specified or not the end of a device path.
         * @return security_violation you do not have permission to connect drivers
         * to this controller or connect drivers on this device path. Whichever
         * the firmware pegs us with.
         */
        efi::connect_controller            connect_controller;
        /**
         * @brief Disconnects one or more drivers from a controller.
         * @param controller_handle the controller to disconnect drivers from.
         * @param driver_image_handle the driver to disconnect, if nullptr, then
         * all drivers are disconnected.
         * @param child_handle the child-handle of the controller to destroy. If nullptr, then all children are
         * destroyed (quite morbid).
         */
        efi::disconnect_controller         disconnect_controller;
        /**
         * @brief Opens a protocol if a handle supports the protocol.
         * @param handle the handle to check support for.
         * @param protocol the protocol to try to connect to the handle
         * @param interface an optional pointer to the interface pointer.
         * @param agent_hadnle the handle of the agent opeating the protocol
         * interface.
         * @param controller_handle if the agent opening the protocol follows the
         * UEFI driver model, then this refers to the controller that requires
         * the preotocol. Otherwise, this parameter is nullptr.
         * @param attributes the mode under which we open the protocol.
         *
         * @return invalid_parameter - protcol is nullptr
         * @return invalid_parameter - interface is nullptr and attributes is not
         * test_protocol.
         * @return invalid_parameter - handle is nullptr
         * @return invalid_parameter - attributes is not a legal value
         * @return invalid_parameter - attributes is by_child_controller, by_driver,
         * exclusive, or by_driver | exclusive and controller_handle is nullptr.
         * @return unsupported - handle does not support protocol
         * @return access_denied - attributes is by_driver, exclusive, or by_driver | exclusive
         * and anything else has the (handle?) open with the exclusive or by_driver | exclusive
         * attribute.
         * @return access_denied - attributes is by_driver or by_driver | exclusive and anything else has already
         * opened the (handle?) with the same attributes and a different agent_handle
         * @return access_denied - attributes is by_driver | exclusive or exclusive and after the boot_services
         * called disconnect_controller there was still something that has the (handle?) open with the by_driver
         * attribute.
         * @return already_started - attributes is by_driver or by_driver | exclusive and someone else has opened
         * a protocol with the same attributes and the same agent_handle.
         */
        efi::open_protocol                 open_a_protocol;
        /**
         * @brief Closes a protocol on a handle that was previously opened with open_protocol.
         * @param handle the handle to close
         * @param protocol the protocol we want to close
         * @param agent_handle the handle of the agent closing the protocol
         * @param controller_handle the controller handle that requires the
         * protocol. If the agent does not follow the UEFI driver model, this
         * parameter is ignored and can be nullptr.
         * @return success - the protocol was closed
         * @return invalid_paramter - handle, agent_handle, or protocol are nullptr. Additionally, if controller_handle
         * is "not nullptr and controller_handle is nullptr".
         * @return not_found - the protocol was either not opened by that agent / controller combination or the handle
         * does not support the protocol.
         */
        efi::close_protocol                close_a_protocol;
        /**
         * @brief Retrieves a list of agents that currently have a protocol interface
         * opened.
         * @param handle the handle that agents have protocols open on.
         * @param protocol the protocol we're interested in
         * @param entry_buffer the pointer to the buffer that
         * the firmware allocated to give us this all important infromation. It's
         * not clear whether this buffer is allocated with allocate_pool or allocate_pages.
         * @param entry_count the number of entries in the buffer.
         * @return success - the information about the entries were given in the
         * buffer and the number of entries given in entry_count.
         * @return not_found - handle does not support protocol
         * @return out_of_resources - the firmware could not allocate the buffer.
         *
         */
        efi::open_protocol_information     get_info_on_open_protocol;
        /**
         * @brief Retrieves the list of prtocols installed on a specific handle.
         * @param handle the handle we're interested in
         * @param protocol_buffer a pointer to a list of guid pointers that refer
         * to the protocols installed on the handle.
         * @param protocol_buffer_count the amount of entries in the buffer.
         */
        efi::protocols_per_handle          protocols_opened_on_handle;
        /**
         * @brief Returns an array of handles that support the specified protocol
         * @param search_type how to look for the handle(s).
         * @param provides the protocol to search by. Only required if searching
         * by_protocol
         * @param search_key the search key
         * @param handle_count how many handles are in the buffer
         * @param handle_buffer the buffer containing all the handles.
         * @return success - the handles were successfully returned in the buffer.
         * @return invalid_parameter - handle_count or handle_buffer are nullptr.
         * @return not_found - no handles match the search.
         * @return out_of_resources - the buffer could not be allocated.
         * @remark the standard specifically says that this buffer is allocated with
         * allocate_pool
         */
        efi::locate_handles_for_protocol   locate_handles_for_protocol;
        /**
         * @brief Finds an interface that matches the specified protocol.
         * @param protocol - the protocol to match
         * @param registration - the registration key returned from a call to
         * register_protocol_notify. Ignored if nullptr.
         * @param interface - the instance of the protocol. That is, some class
         * that supports the protocol.
         * @return success - the first matching protocol instance was returned in
         * the interface.
         * @return invalid_parameter - protocol or interface are nullptr.
         * @return not_found there is no scuh protocol instance matching protocol
         * and registration.
         *
         * @note this function sets interface to point to nullptr if there is no
         * instance matching that protocol.
         */
        efi::locate_protocol               find_protocol;
        /**
         * @brief Installs one or more protocol interfaces into the boot services
         * environment
         * @param handle the pointer to the handle to install the new interfaces
         * on, nullptr if the handle needs to be allocated first.
         * @param [va_args] a list containing pairs of protocols and interfaces. Terminated by a nullptr protocol.
         * @return success - all the protocol interfaces were installed.
         * @return already_started - a device_path_protocol was passed in that already exists in the handle database.
         * @return invalid_parameter - a protocol is already iinstalled on handle or the pointer to the handle is
         * nullptr.
         */
        efi::install_multiple_interfaces   install_interfaces;
        /**
         * @brief Removes one or more protocol interfaces from the boot services environment.
         * @param handle the handle to remove the protocols from.
         * @param [va_args] a variable length list containing pairs of protocol guids and
         * interface instances.
         * @return success - all protocol interfaces were removed
         * @return invalid_parameter - one of the interfaces was not previously
         * installed on the handle.
         */
        efi::uninstall_multiple_interfaces uninstall_interfaces;
        /**
         * @brief Calculates the CRC-32 of specified data.
         * @param data the start of the data to calculate the CRC-32 on.
         * @param data_size the amount of bytes in the data.
         * @param crc32 - the CRC-32
         * @return success - the 32-bit CRC was calculated and placed in *crc32
         * @return invalid_parameter - any of these arguements are 0 / nullptr.
         * @remark unfortunately, we have tl calculate 2 CRC-32's before we can
         * be confident that this function is safe *facepalm*.
         */
        efi::calculate_crc32               calculate_crc32;
        /**
         * @brief Copies memory from one location to another.
         * @param destination the start of where to put the memory
         * @param source the start of the memory to copy.
         * @param length the number of bytes to copy.
         * @note this function properly handles when source and destination overlap.
         */
        efi::copy_memory                   memcpy;
        /**
         * @brief Sets a buffer to a specified value.
         * @param buffer the start of memory to set
         * @param size how many bytes to set in buffer
         * @param value the value of buffer.
         */
        efi::set_memory                    memset;

        // above functions are valid in all versions of the boot services from
        // revision 1.1 and onwards. If the boot services is valid and the
        // (inherited) revision number is at leeast revisions::version_1_1, then
        // all of the above functions are reasonably safe to call even if the
        // revision is any amount greater than version_1_1.

        /**
         * @brief Creates an event in a group
         * @param type the type of the event
         * @param notify_tpl the task priority level when running the notify
         * function.
         * @param notify_function an optional callback to call when the event
         * occurs.
         * @param notify_context the context for the function.
         * @param event_group the gorup that the event is in.
         * @param event the newly-created event
         * @return success - the event was created
         * @return invalid_parameter event is null, type is an unsupported value, notify is null when the event can be
         * signaled or notified, notify_tpl is unsupported.
         * @return out_of_resources - the firmware could not allocate the event.
         * @see efi::create_event
         */
        efi::create_event_ex advanced_create_event;

        // advanced_create_event was added in revision 2.0 and is valid when the
        // revision field is at least revisions::version_2_0. If the revision
        // field is at least that revision then advanced_create_event is safe to
        // call.
    };

    /**
     * @brief The runtime services table.
     * @note currently, this structure is incomplete. As for why, I direct you
     * to the current line number.
     */
    struct EFI_CLASS runtime_services : public table_header {
        /**
         * @brief Gets the current date / time along with the time-keeping abilities
         * of the system's real time clock
         * @param time - a pointer to an instance of efi::time
         * @param capabilities - optional pointer to the time capabilities.
         * @return success - the firmware gave us the time.
         * @return invalid_parameter - time is nullptr
         * @return device_error - the clock died.
         * @return unsupported - this function no longer works after calling
         * exit_boot services. The firmware told us that this function does
         * not work.
         */
        efi::get_time                get_time;
        /**
         * @brief Sets the current local time and date information.
         * @param time the time.
         * @note this function is why systems with dual boot between windows and
         * ubuntu get their times messed up. Ubuntu changes the timezone to be in
         * UTC, then Windows reads it as "we must be in UTC+0", meaning that someone
         * like me, who lives in the central time zone, gets a windows that thinks
         * it's six hours ahead.
         * @return success - successfully set the time
         * @return invalid_parameter - the time does not make sense (eg. February 31st)
         * @return device_error - the clock died.
         * @return unsupported - this function on longer works after calling
         * exit_boot_services. The firmware told us that this function does not
         * work.
         */
        efi::set_time                set_time;
        /**
         * @brief Gets the current wakeup clock setting.
         * @param enabled - whether the system wakeup clock is currently enabled
         * @param pending - whether the alarm must be acknowledged, analogus to an
         * alarm clock going off
         * @param time the time that the alarm is set to go off at.
         * @note time may be rounded to the nearest second. EFI defines the resolution
         * of this clock to be one second.
         * @return success - successfully got the alarm settings
         * @return invalid_parameter - any of the arguments are nullptr.
         * @return device_error - the clock died.
         * @return unsupported - this function no longer works after calling exit_boot_services.
         * The firmware told us that this function does not work.
         */
        efi::get_wakeup_time         get_wakeup_time;
        /**
         * @brief Sets the system wakeup time.
         * @note if this alarm goes off, the system wakes as soon as possible. if
         * it is about to turn off or go to sleep, it immediately wakes up. if the
         * system lacks the power to turn back on, the system turns on as soon as
         * it gets the power to do so.
         * @param enable whether to enable or disable the alarm.
         * @param time the time to set the alarm. If enable is false, then this
         * value is optional.
         * @return success - whether the alarm was enabled / disabled.
         * @return invalid_parameter - the time does not make sense (eg. 13 PM)
         * @return device_error - the clock died.
         * @return unsupported - this function no longer workds after calling
         * exit_boot_services. THe firmware told us that this function does not
         * work.
         */
        efi::set_wakeup_time         set_wakeup_time;
        /**
         * @brief Applies our virtual memory map that we worked really hard on.
         * @param memory_map_size the size of the memory map (bytes)
         * @param descriptor_size the size of each entry in the map (bytes)
         * @param descriptor_version the version of the descritpors. Currently
         * only version 1 exists.
         * @param virtual_map the map we worked really hard on.
         * @return success - the map was applied
         * @return unsupported - we haven't called exit_boot_services, the firmware
         * has already done its virtual mapping, or the runtime services is telling
         * us that it's kinda useless and really only works with variables and the
         * clock.
         * @return no_mapping we didn't accept all of the memory
         * @return not_found we specified a virtual address for a physical address
         * which does not exist.
         */
        efi::set_virtual_address_map set_virtual_address_map;
        /**
         * @brief Concerts a pointer to its virtual address version.
         * @param debug_disposition some debugging information that is currently
         * defined to be `0x0000'0001`.
         * @param address the memory address that needs to be fixed to its 
         * virtual version.
         * @return success - the pointer was successfully converted
         * @return not_found the pointer that was to be converted is not in the
         * virtual address map. EFI says that if we see this, then catastrophic
         * program failure is immanent (much more mildly, however, as "this is 
         * normally fatal").
         * @return invalid_parameter address is nullptr or *address is nullptr and
         * the debug_disposition is not optional_pointer.
         * @return unsupported the firmware is kinda telling us that it doesn't
         * do the whole runtime services thing. Like it gave the table, but the
         * functions don't do anything.
         */
        efi::convert_pointer         convert_pointer;
        // NOTE: this structure is incomplete and defines more functionality.
    };

    /**
     * @brief A globally unique identifier.
     * 
     */
    struct EFI_CLASS guid {
        /**
         * @brief The first four bytes.
         * 
         */
        uint32 data1;
        /**
         * @brief The fifth and sixth bytes.
         * 
         */
        uint16 data2;
        /**
         * @brief The seventh and eighth bytes.
         * 
         */
        uint16 data3;
        /**
         * @brief The remaining half of the 16-byte structure.
         * 
         */
        uint8  data4 [ 8 ];
    };

    /**
     * @brief The IP address that's still used today even though it actually does
     * not have enough values for all devices which can access the internet. 
     * Garbage OS will likely prefer using ipv6 addresses.
     * 
     */
    struct EFI_CLASS ipv4_address {
        uint8 bytes [ 4 ];
    };

    /**
     * @brief The CISCO-brand IP address that can access as many internet-capable
     * devices as EFI can access files. In other words, if we run out of IPv6
     * addresses, the firmware has bigger issues at hand.
     * 
     */
    struct EFI_CLASS ipv6_address {
        // placeholder until it becomes more clean
        uint8 bytes [ 16 ];
    };

    /**
     * @brief Magical union that's either an ipv4 or ipv6 address.
     * @note not always used when we just "need an ip address" since this results
     * in padding placed between ipv4 addresses.
     */
    union __attribute__( ( aligned( 4 ) ) ) ip_address {
        ipv4_address ipv4;
        ipv6_address ipv6;
    };

    /**
     * @brief A media access control address. A MAC uniquely identifies the 
     * hardware the device uses to connect to the internet. For example, your 
     * phone has two MACs: one that it uses for the cellular network's antenna
     * and one for the Wi-Fi antenna. 
     * @remark Fun fact: the fifth generation of Pokémon games utilize the MAC
     * address in their RNG function and as a result each individual DS / DSi / 
     * 3DS has a unique RNG manipulation for each game!
     */
    struct EFI_CLASS mac_address {
        /**
         * @brief EFI does not specify the format of a MAC address.
         * 
         */
        uint8_t bytes [ 32 ];
    };

    /**
     * @brief The event groups as defined by EFI.
     * @see create_event_ex
     */
    namespace event_groups {
        /**
         * @brief Tells EFI to call this event's notification function when the
         * call to exit_boot_services is *happening*
         * 
         */
        constexpr guid exit_boot_services = {
                0x27AB'F055,
                0xB1B8,
                0x4C26,
                {0x80, 0x48, 0x74, 0x8F, 0x37, 0xBA, 0xA2, 0xDF}
        };
        /**
         * @brief Tells EFI to call this event's notification function when the
         * exiting of boot_services is about to happen.
         * 
         */
        constexpr guid before_exit_boot_services = {
                0x8BE8'E274,
                0x3970,
                0x4B44,
                {0x80, 0xC5, 0x1A, 0xB9, 0x50, 0x2F, 0x3B, 0xFC}
        };

        /**
         * @brief Tells EFI to call this event's notification function when a 
         * virtual address changes.
         * 
         */
        constexpr guid virtual_address_change = {
                0x13FA'7698,
                0xC831,
                0x49C7,
                {0x87, 0xEA, 0x8F, 0x43, 0xFC, 0xC2, 0x51, 0x96}
        };

        /**
         * @brief Tells EFI to call this event's notification function when the 
         * system is ready to boot, presumably after POSTing.
         * 
         */
        constexpr guid ready_to_boot = {
                0x7CE8'8FB3,
                0x4BD7,
                0x4679,
                {0x87, 0xA8, 0xA8, 0xD8, 0xDE, 0xE5, 0x0D, 0x2B}
        };

        /**
         * @brief Tells EFI to call this event's notification function after the
         * system is ready to boot. 
         * 
         */
        constexpr guid after_ready_to_boot = {
                0x3A2A'00AD,
                0x98B9,
                0x4CDF,
                {0xA4, 0x78, 0x70, 0x27, 0x77, 0xF1, 0xC1, 0x0B}
        };

        /**
         * @brief Tells EFI to call this event's notification function when the
         * system is reset.
         * 
         */
        constexpr guid reset_system = {
                0x62DA'6A56,
                0x13FB,
                0x485A,
                {0xA8, 0xDA, 0xA3, 0xDD, 0x79, 0x12, 0xCB, 0x6B}
        };
    }    // namespace event_groups

    /**
     * @brief GUIDs that define a specific protocol. Compatibility with a protocol
     * is defined by the protocol having the correct GUID. That is, having the
     * protocol we expect means having the features we expect.
     * 
     */
    namespace protocol_guids {
        /**
         * @brief Identifies a device path protocol.
         * 
         */
        constexpr guid device_path = {
                0x095'76E9,
                0x6D3F,
                0x11D2,
                {0x8E, 0x39, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B}
        };

        /**
         * @brief Identifies an HII package list protocol.
         * 
         */
        constexpr guid hii_package_list = {
                0x6A1E'E763,
                0xD46A,
                0x43B4,
                {0xAA, 0xBE, 0xEF, 0x1D, 0xE2, 0xAB, 0x56, 0xFC},
        };
    }    // namespace protocol_guids

    /**
     * @brief Fields common to all device paths.
     * 
     */
    struct EFI_CLASS device_path {
        /**
         * @brief Which part of the device path is this?
         * 
         */
        device_path_type    type;
        /**
         * @brief What type of this part of the device path is this?
         * 
         */
        device_path_subtype subtype;
        /**
         * @brief How many bytes long is this node in the path?
         * 
         */
        uint16              length;
    };

    /**
     * @brief Dummy structure to enlist the compiler in making sure that we don't
     * stuff something that isn't a hardware_device_path where we can only put a
     * hardware_device_path.
     * 
     */
    struct EFI_CLASS hardware_device_path : public device_path { };

    /**
     * @brief Dummy structure to enlist the compiler in making sure that we don't
     * stuff something that isn't an acpi_device_path where we can only put an
     * acpi_device_path
     * 
     */
    struct EFI_CLASS acpi_device_path : public device_path { };

    /**
     * @brief Dummy structure to enlist the compiler in making sure that we don't
     * stuff something that isn't a messaging_device_path where we can only put
     * a messaging_device_path.
     * 
     */
    struct EFI_CLASS messaging_device_path : public device_path { };

    /**
     * @brief Dummy structure to enlist the compiler in making sure that we don't
     * stuff something that isn't a media_device_path where we can only put a 
     * media_device_path
     * 
     */
    struct EFI_CLASS media_device_path : public device_path { };

    /**
     * @brief A BIOS Boot Specification device path. There is only one type of
     * device path with this subtype. Like BIOS, it's quickly becoming gone.
     * 
     */
    struct EFI_CLASS bios_boot_specification_device_path : public device_path {
        /**
         * @brief What BIOS calls this boot device.
         * 
         */
        uint16 device_type;
        /**
         * @brief Whatever BIOS says is going on with this device.
         * 
         */
        uint16 status_flag;
        /**
         * @brief The description of the device.
         * 
         */
        FLEXIBLE_ARRAY( char8, desccription )
    };

    /**
     * @brief Fields common to PCI device paths and PCCARD device paths.
     * 
     */
    struct EFI_CLASS card_device_path : public hardware_device_path {
        uint8 function;
    };

    /**
     * @brief A device path that refers to a specific function and device 
     * combination on the PCI bus. Take for example, the Graphics card or the
     * M.2 drive, or the network card.
     * 
     */
    struct EFI_CLASS pci_device_path : public card_device_path {
        uint8 device;
    };

    /**
     * @brief A specific PCCARD which is connected to the device.
     * 
     */
    struct EFI_CLASS pccard_device_path : public card_device_path { };

    /**
     * @brief An MMIO device. 
     * 
     */
    struct EFI_CLASS memory_mapped_device_path : public hardware_device_path {
        /**
         * @brief Not sure what will happen if we see a type that we can't write
         * to...
         * 
         */
        efi::memory_type memory_type;
        efi::uint64      start_address;
        efi::uint64      end_address;
    };

    /**
     * @brief Vendor-defined hardware device path.
     * @note we're just supposed to look at the GUID and immediatly know what the
     * data means.
     */
    struct EFI_CLASS vendor_defined_hardware_device_path : public hardware_device_path {
        guid vendor_guid;
        FLEXIBLE_ARRAY( uint8, vendor_data )
    };

    /**
     * @brief A controller device path with no additional context as to what the
     * controller is, but it has a number.
     * 
     */
    struct EFI_CLASS controller_device_path : public hardware_device_path {
        uint32 controller_number;
    };

    /**
     * @brief A hardware device path indicating that we need to go through the
     * baseboard management controller.
     * 
     */
    struct EFI_CLASS baseboard_management_controller_device_path : public hardware_device_path {
        uint8  bmc_interface;
        uint64 base_address;
    };

    /**
     * @brief Fields common to the ACPI short device path and the ACPI extended
     * device path.
     * 
     */
    struct EFI_CLASS generic_acpi_device_path : public acpi_device_path {
        /**
         * @brief The _HID field
         * 
         */
        uint32 hardware_id;
        /**
         * @brief The _UID field
         * 
         */
        uint32 unique_id;
    };

    /**
     * @brief An ACPI device, here the system specifies a bit less information than
     * the acpi_extended_device_path.
     * 
     */
    struct EFI_CLASS acpi_short_device_path : public generic_acpi_device_path { };

    /**
     * @brief An ACPI device path with every possible bell and whistle.
     * 
     */
    struct EFI_CLASS apci_extended_device_path : public generic_acpi_device_path {
        /**
         * @brief The _CID field.
         * 
         */
        uint32 compatible_id;
        /**
         * @brief The Hardware ID, Unique ID, and PNP Compatible ID which are all
         * null-terminated strings and embedded in this structure. Since there is
         * no way to tell the compiler where the unique ID and compatible ID are, 
         * just trust that they're delimited by nul.
         * 
         */
        FLEXIBLE_ARRAY( char, id_strings )
    };

    /**
     * @brief A list of addresses that mean something in the context of the ACPI
     * standard.
     * 
     */
    struct EFI_CLASS acpi_address_device_path : public acpi_device_path {
        FLEXIBLE_ARRAY( uint32, addresses )
    };

    struct EFI_CLASS nvdimm_device_path : public acpi_device_path {
        uint32 nfit_device_handle;
    };

    struct EFI_CLASS atapi_device_path : public messaging_device_path {
        bool   is_secondary;
        bool   is_driven_device;
        uint16 logical_unit_number;
    };

    struct EFI_CLASS scsi_device_path : public messaging_device_path {
        uint16 target_id;
        uint16 logical_unit_number;
    };

    struct EFI_CLASS fibre_channel_device_path : public messaging_device_path {
        uint32 reserved;
        uint64 world_wide_name;
        uint64 logical_unit_number;
    };

    struct EFI_CLASS fibre_channel_ex_device_path : public messaging_device_path {
        uint32 reserved;
        uint64 world_wide_name;
        uint8  logical_unit_number [ 8 ];
    };

    struct EFI_CLASS spec_1394_device_path : public messaging_device_path {
        uint32         reserved;
        spec1394::guid guid;
    };

    struct EFI_CLASS sata_device_path : public messaging_device_path {
        uint16 hba_port_number;
        uint16 port_multiplier;
        uint16 logical_unit_number;
    };

    struct EFI_CLASS usb_device_paths : public messaging_device_path {
        uint8 parent_port;
        uint8 usb_interface;
    };

    struct EFI_CLASS usb_wwid_device_path : public messaging_device_path {
        uint16 usb_interface;
        uint16 vendor_id;
        uint16 product_id;
        // note: this string is at most 64 characters long.
        FLEXIBLE_ARRAY( char16, serial_number )
    };

    struct EFI_CLASS device_logical_unit : public messaging_device_path {
        uint16 logical_unit_number;
    };

    struct EFI_CLASS usb_class_device_path : public messaging_device_path {
        uint16 vendor_id;
        uint16 product_id;
        uint8  device_class;
        uint8  device_subclass;
        uint8  device_protocol;
    };

    struct EFI_CLASS i2o_device_path : public messaging_device_path {
        uint32 target_id;
    };

    template <specific_ip_address address_type> struct EFI_CLASS generic_ip_device_path : public messaging_device_path {
        address_type local_ip;
        address_type remote_ip;
        uint16       local_port;
        uint16       remote_port;
        uint16       protocol;
    };

    struct EFI_CLASS ipv4_device_path : public generic_ip_device_path<ipv4_address> {
        bool         is_static;
        ipv4_address gateway;
        uint32       subnet_mask;
    };

    struct EFI_CLASS ipv6_device_path : public generic_ip_device_path<ipv6_address> {
        uint8        ip_address_origin;
        uint8        prefix_length;
        ipv6_address gateway;
    };

    struct EFI_CLASS ufs_device_path : public messaging_device_path {
        uint8 target_id;
        uint8 logical_unit_number;
    };

    struct EFI_CLASS mac_address_device_path : public messaging_device_path {
        efi::mac_address mac_address;
        efi::uint8       network_interface;
    };

    struct EFI_CLASS vlan_device_path : public messaging_device_path {
        uint16 vlan_id : 13;
    };

    struct EFI_CLASS infiniband_device_path : public messaging_device_path {
        uint32  resource_flags;
        uint128 port_gid;
        uint64  ioc_or_service_id;
        uint64  target_port;
        uint64  remote_device_id;
    };

    struct EFI_CLASS uart_device_path : public messaging_device_path {
        uint32 reserved;
        uint64 baud_rate;
        uint8  data_bits;
        uint8  parity;
        uint8  stop_bits;
    };

    struct EFI_CLASS vendor_defined_mesaging_device_path : public messaging_device_path {
        guid vendor_guid;
        FLEXIBLE_ARRAY( uint8, vendor_data )
    };

    struct EFI_CLASS sas_extended_device_path : public messaging_device_path {
        uint8  sas_address [ 8 ];
        uint8  logical_unit_number [ 8 ];
        uint16 sas_sata_device_and_topology;
        uint16 relative_target_port;
    };

    struct EFI_CLASS iscsi_device_path : public messaging_device_path {
        uint16 protocol;
        uint16 login_options;
        uint8  logical_unit_number [ 8 ];
        uint16 target_portal_group;
        FLEXIBLE_ARRAY( uint8, target_name )
    };

    struct EFI_CLASS nvme_namespace_device_path : public messaging_device_path {
        uint32 namespace_identifier;
        uint64 ieee_eui;
    };

    struct EFI_CLASS wifi_device_path : public messaging_device_path {
        uint8 ssid [ 32 ];
    };

    struct EFI_CLASS generic_bluetooth_device_path : public messaging_device_path {
        uint8 bluetooth_address [ 6 ];
    };

    struct EFI_CLASS bluetooth_device_path : public generic_bluetooth_device_path { };

    struct EFI_CLASS efi_bluetooth_le_device_path : public generic_bluetooth_device_path {
        uint8 address_type;
    };

    struct EFI_CLASS storage_card_device_path : public messaging_device_path {
        uint8 slot_number;
    };

    struct EFI_CLASS sd_device_path : public storage_card_device_path { };

    struct EFI_CLASS emmc_device_path : public storage_card_device_path { };

    struct EFI_CLASS dns_service_path {
        bool is_ipv6;
        FLEXIBLE_ARRAY( ip_address, addresses )
    };

    struct EFI_CLASS nvdimm_namespace_device_path : public messaging_device_path {
        nvdimm_uuid uuid;
    };

    struct EFI_CLASS generic_rest_service_device_path : public messaging_device_path {
        uint8 service;
        uint8 access_mode;
    };

    struct EFI_CLASS rest_service_device_path : public generic_rest_service_device_path { };

    struct EFI_CLASS vendor_specific_rest_service_device_path : public generic_rest_service_device_path {
        guid vendor_guid;
        FLEXIBLE_ARRAY( uint8, vendor_data )
    };

    struct EFI_CLASS nvmeof_namespace_device_path : public messaging_device_path {
        uint8 namespace_id_type;
        uint8 namespace_id [ 16 ];
        // length <= 224.
        FLEXIBLE_ARRAY( uint8, subsystem_nqn )
    };

    struct EFI_CLASS hard_drive_media_device_path : public media_device_path {
        uint32                partition_number;
        logical_block_address partition_start;
        logical_block_address partition_length;
        uint8                 partition_signature [ 16 ];
        uint8                 partition_format;
        uint8                 signature_type;
    };

    struct EFI_CLASS vendor_defined_media_device_path : public media_device_path {
        guid vendor_guid;
        FLEXIBLE_ARRAY( uint8, vendor_data )
    };

    struct EFI_CLASS el_torito_device_path : public media_device_path {
        uint32                 boot_entry;
        relative_block_address partition_start;
        relative_block_address partition_length;
    };

    struct EFI_CLASS file_path_media_device_path : public media_device_path {
        // char type unclear in UEFI.
        FLEXIBLE_ARRAY( char, name )
    };

    struct EFI_CLASS media_protocol_device_path : public media_device_path {
        guid protocol;
    };

    struct EFI_CLASS piwg_firmware_file : public media_device_path {
        // a piwg file. PIWG is a standard realted to UEFI
        FLEXIBLE_ARRAY( uint8, contents )
    };

    struct EFI_CLASS piwg_firmware_volume : public media_device_path {
        // a piwg voluem. PIWG is a standard related to UEFI.
        FLEXIBLE_ARRAY( uint8, contents )
    };

    struct EFI_CLASS relative_offset_range_device_path : public media_device_path {
        uint32 reserved;
        uint64 starting_offset;
        uint64 ending_offset;
    };

    struct EFI_CLASS ram_disk_device_path : public media_device_path {
        physical_address starting_address;
        physical_address ending_address;
        guid             disk_type;
        uint16           disk_instance_number;
    };

    struct EFI_CLASS memory_descriptor {
        uint32           type;
        physical_address physical_start;
        virtual_address  virtual_start;
        uint64           page_count;
        uint64           attributes;
    };

    struct EFI_CLASS open_protocol_information_entry {
        handle agent;
        handle controller;
        uint32 attributes;
        uint32 use_count;
    };

    struct EFI_CLASS time {
        uint16 year;
        uint8  month;
        uint8  day;
        uint8  hour;
        uint8  second;
        uint8  padding_for_alignment;
        uint32 nanoseconds;
        uint16 time_zone;
        uint8  daylight_savings_state;
        uint8  more_padding_for_alignment;
    };

    struct EFI_CLASS time_capabilities {
        // how many clock updates per second
        uint32 resolution;
        // accuracy of real time clock in pico-ticks. An accuracy of 50us with
        // a resolution of 1 would be 50,000,000 accuracy.
        uint32 accuracy;
        // whether setting the clock clears the resolution below the level
        // reported in runtime_services.get_time.
        bool   sets_to_zero;
    };

    struct EFI_CLASS simple_text_output_mode {
        uint32 max_mode;
        uint32 mode;
        uint32 attribute;
        uint32 cursor_column;
        uint32 cursor_row;
    };

    struct EFI_CLASS input_key {
        uint16 scan_code;
        char16 unicode_char;
    };

    namespace daylight_adjust_modes {
        constexpr uint8 adjust_daylight  = 1;
        constexpr uint8 time_in_daylight = 2;
    }    // namespace daylight_adjust_modes

    namespace timezones {
        // time zones go here
        constexpr uint16 central_daylight_time = 300;
        constexpr uint16 unspecified_timezone  = 0x7FF;
    }    // namespace timezones

    class EFI_CLASS simple_text_io {
    public:
        virtual status reset( bool ) noexcept = 0;
    };

    class EFI_CLASS simple_text_input : public simple_text_io {
    public:
        virtual status read_key_stroke( input_key * ) noexcept = 0;
        event          wait_for_key;
    };

    class EFI_CLASS simple_text_output : public simple_text_io {
    public:
        virtual status           output_string( char16 *string ) noexcept                         = 0;
        virtual status           test_string( char16 *string ) noexcept                           = 0;
        virtual status           query_mode( uintn number, uintn *columns, uintn *rows ) noexcept = 0;
        virtual status           set_mode( uintn number ) noexcept                                = 0;
        virtual status           set_attribute( uintn attribute ) noexcept                        = 0;
        virtual status           clear_mode( ) noexcept                                           = 0;
        virtual status           set_cursor_position( uintn column, uintn row ) noexcept          = 0;
        virtual status           enable_cursor( bool ) noexcept                                   = 0;
        simple_text_output_mode *mode;
    };

    class EFI_CLASS arbitrary_load_file {
    public:
        virtual status load( device_path *, bool, uintn *, void * ) noexcept = 0;
    };

    class EFI_CLASS load_boot_file : public arbitrary_load_file { };

    class EFI_CLASS load_non_boot_file : public arbitrary_load_file { };

    namespace protocols {
        constexpr guid simple_text_input = {
                0x3874'77C1,
                0x69C7,
                0x11D2,
                {0x8E, 0x39, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B}
        };

        constexpr guid simple_text_output = {
                0x3874'77C2,
                0x69C7,
                0x11D2,
                {0x8E, 0x39, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B}
        };

        constexpr guid load_boot_file = {
                0x56EC'3091,
                0x954C,
                0x11D2,
                {0x8E, 0x3F, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B}
        };

        constexpr guid load_non_boot_file = {
                0x4006'C0C1,
                0xFCB3,
                0x403E,
                {0x99, 0x6D, 0x4A, 0x6C, 0x87, 0x24, 0xE0, 0x6D}
        };
    }    // namespace protocols

    namespace statuses {
        constexpr status success = 0;
    }
}    //  namespace efi

#endif