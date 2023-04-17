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
    using boolean = bool;
    using intn    = std::intmax_t;
    using uintn   = std::uintmax_t;
    using int8    = std::int8_t;
    using uint8   = std::uint8_t;
    using int16   = std::int16_t;
    using uint16  = std::uint16_t;
    using int32   = std::int32_t;
    using uint32  = std::uint32_t;
#if defined( _32_BITS )
    using int64   = composite_uint64;
    using uint64  = composite_uint64;
    // TODO: make version of composite_uint128 that invovles two composite_uint64s
    using int128  = composite_uint64 [ 2 ];
    using uint128 = composite_uint64 [ 2 ];
#elif defined( _64_BITS )
    using int64   = std::int64_t;
    using uint64  = std::uint64_t;
    using int128  = composite_uint128;
    using uint128 = composite_uint128;
#else
    static_assert( sizeof( void * 0 ) >= 16,
                   "Memory addresses are the wrong size. Did you mean to define _32_BITS or _64_BITS?" );
    using int64   = std::int64_t;
    using uint64  = std::uint64_t;
    using int128  = __int128_t;
    using uint128 = __uint128_t;
#endif    // if defined(_32_BITS
    using char8                  = unsigned char;
    using char16                 = char16_t;
    using status                 = uintn;
    using handle                 = void *;
    using event                  = void *;
    using logical_block_address  = uint64;
    using relative_block_address = uint64;
    using task_priority_level    = uintn;
    using physical_address       = uint64;
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
    using nvdimm_uuid = guid;

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
     * @brief Something related to drivers and adding them to the system. Loaders
     * don't install drivers.
     *
     */
    using install_interface             = callback<status, handle *, guid *, interface_type, void *>;
    /**
     * @brief Something related to drivers and removing them from the system. Loaders
     * shouldn't need to remove drivers.
     *
     */
    using uninstall_interface           = callback<status, handle, guid *, void *>;
    /**
     * @brief Something related to reinstalling drivers in the system. Loaders don't
     * need to reinstall drivers.
     *
     */
    using reinstall_interface           = callback<status, handle, guid *, void *, void *>;
    using register_notify               = callback<status, guid *, event, void **>;
    using locate_handle                 = callback<status, locate_search_type, guid *, void *, uintn *, handle *>;
    using handle_protocol               = callback<status, handle, guid *, void **>;
    using locate_device_path            = callback<status, guid *, device_path **, handle *>;
    using open_protocol                 = callback<status, handle, guid *, void **, handle, handle, uint32>;
    using close_protocol                = callback<status, handle, guid *, handle, handle>;
    using open_protocol_information     = callback<status, handle, guid *, open_protocol_information_entry **, uintn *>;
    using connect_controller            = callback<status, handle, handle *, device_path *, bool>;
    using disconnect_controller         = callback<status, handle, handle, handle>;
    using protocols_per_handle          = callback<status, handle, guid ***, uintn *>;
    using locate_handle_buffer          = callback<status, locate_search_type, guid *, void *, uintn *, handle **>;
    using locate_protocol               = callback<status, guid *, void *, void **>;
    using install_multiple_interfaces   = callback<status, handle *, std::va_list>;
    using uninstall_multiple_interfaces = callback<status, handle, std::va_list>;
    using load_image                    = callback<status, bool, handle, device_path *, void *, uintn, handle *>;
    using start_image                   = callback<status, handle, uintn *, char16 **>;
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
     * @brief Attributes involving open protocols.
     *
     */
    namespace open_protocol_attributes {
        constexpr uint32 by_handle_protocol  = 1 << 0;
        constexpr uint32 get_protocol        = 1 << 1;
        constexpr uint32 test_protocol       = 1 << 2;
        constexpr uint32 by_child_controlelr = 1 << 3;
        constexpr uint32 by_driver           = 1 << 4;
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
         * @brief latest revision as of 2023
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
        efi::raise_tpl                   raise_tpl;
        /**
         * @brief Restores the TPL to what it was before calling raise_tpl.
         * @param tpl the old tpl that we want to return to.
         * @warning EFI explicitly says that strange things can happen if we don't
         * return to the correct TPL.
         */
        efi::restore_tpl                 restore_tpl;
        efi::allocate_pages              page_malloc;
        efi::free_pages                  page_free;
        efi::get_memory_map              get_memory_map;
        efi::allocate_pool               pool_malloc;
        efi::free_pool                   pool_free;
        efi::create_event                create_event;
        efi::set_timer                   set_timer;
        efi::wait_for_event              wait_for_event_in_list;
        efi::signal_event                signal_event;
        efi::close_event                 close_event;
        efi::check_event                 check_event;
        efi::install_interface           install_interface;
        efi::reinstall_interface         reinstall_interface;
        efi::uninstall_interface         uninstall_interface;
        efi::handle_protocol             handle_protocol;
        void                            *reserved_function;
        efi::register_notify             register_protocol_to_event;
        efi::locate_handle               locate_handle;
        efi::locate_device_path          locate_device_path;
        efi::install_configuration_table install_configuration_table;
        efi::load_image                  load_image;
        efi::start_image                 start_image;
        efi::exit                        exit;
        efi::unload_image                unload_image;
        efi::exit_boot_services          exit_boot_services;
        efi::get_next_monotonic_count    get_monotonic_count;
        efi::stall                       stall;
        efi::set_watchdog_timer          set_watchdog_timer;

        // above functions are valid in all versions of the standard. If the
        // boot services are valid, then these functions are reasonably safe
        // to call no matter the revision number of the boot services, except
        // for the reserved field.

        efi::connect_controller            connect_controller;
        efi::disconnect_controller         disconnect_controller;
        efi::open_protocol                 open_a_protocol;
        efi::close_protocol                close_a_protocol;
        efi::open_protocol_information     get_info_on_open_protocol;
        efi::protocols_per_handle          protocols_that_work_with_handle;
        efi::locate_handle_buffer          locate_handle_buffer;
        efi::locate_protocol               find_protocol;
        efi::install_multiple_interfaces   install_interfaces;
        efi::uninstall_multiple_interfaces uninstall_interfaces;
        efi::calculate_crc32               calculate_crc32;
        efi::copy_memory                   memcpy;
        efi::set_memory                    memset;

        // above functions are valid in all versions of the boot services from
        // revision 1.1 and onwards. If the boot services is valid and the
        // (inherited) revision number is at leeast revisions::version_1_1, then
        // all of the above functions are reasonably safe to call even if the
        // revision is any amount greater than version_1_1.

        efi::create_event_ex advanced_create_event;

        // advanced_create_event was added in revision 2.0 and is valid when the
        // revision field is at least revisions::version_2_0. If the revision
        // field is at least that revision then advanced_create_event is safe to
        // call.
    };

    struct EFI_CLASS runtime_services : public table_header {
        efi::get_time                get_time;
        efi::set_time                set_time;
        efi::get_wakeup_time         get_wakeup_time;
        efi::set_wakeup_time         set_wakeup_time;
        efi::set_virtual_address_map set_virtual_address_map;
        efi::convert_pointer         convert_pointer;
        // NOTE: this structure is incomplete and defines more functionality.
    };

    struct EFI_CLASS guid {
        uint32 data1;
        uint16 data2;
        uint16 data3;
        uint8  data4 [ 8 ];
    };

    struct EFI_CLASS ipv4_address {
        uint8 bytes [ 4 ];
    };

    struct EFI_CLASS ipv6_address {
        // placeholder until it becomes more clean
        uint8 bytes [ 16 ];
    };

    union __attribute__( ( aligned( 4 ) ) ) ip_address {
        ipv4_address ipv4;
        ipv6_address ipv6;
    };

    struct EFI_CLASS mac_address {
        uint8_t bytes [ 32 ];
    };

    namespace event_groups {
        constexpr guid exit_boot_services = {
                0x27AB'F055,
                0xB1B8,
                0x4C26,
                {0x80, 0x48, 0x74, 0x8F, 0x37, 0xBA, 0xA2, 0xDF}
        };
        constexpr guid before_exit_boot_services = {
                0x8BE8'E274,
                0x3970,
                0x4B44,
                {0x80, 0xC5, 0x1A, 0xB9, 0x50, 0x2F, 0x3B, 0xFC}
        };

        constexpr guid virtual_address_change = {
                0x13FA'7698,
                0xC831,
                0x49C7,
                {0x87, 0xEA, 0x8F, 0x43, 0xFC, 0xC2, 0x51, 0x96}
        };

        constexpr guid ready_to_boot = {
                0x7CE8'8FB3,
                0x4BD7,
                0x4679,
                {0x87, 0xA8, 0xA8, 0xD8, 0xDE, 0xE5, 0x0D, 0x2B}
        };

        constexpr guid after_ready_to_boot = {
                0x3A2A'00AD,
                0x98B9,
                0x4CDF,
                {0xA4, 0x78, 0x70, 0x27, 0x77, 0xF1, 0xC1, 0x0B}
        };

        constexpr guid reset_system = {
                0x62DA'6A56,
                0x13FB,
                0x485A,
                {0xA8, 0xDA, 0xA3, 0xDD, 0x79, 0x12, 0xCB, 0x6B}
        };
    }    // namespace event_groups

    namespace protocol_guids {
        constexpr guid device_path = {
                0x095'76E9,
                0x6D3F,
                0x11D2,
                {0x8E, 0x39, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B}
        };
    }

    struct EFI_CLASS device_path {
        device_path_type    type;
        device_path_subtype subtype;
        uint16              length;
    };

    struct EFI_CLASS hardware_device_path : public device_path { };

    struct EFI_CLASS acpi_device_path : public device_path { };

    struct EFI_CLASS messaging_device_path : public device_path { };

    struct EFI_CLASS media_device_path : public device_path { };

    struct EFI_CLASS bios_boot_specification_device_path : public device_path {
        uint16 device_type;
        uint16 status_flag;
        FLEXIBLE_ARRAY( char8, desccription )
    };

    struct EFI_CLASS card_device_path : public hardware_device_path {
        uint8 function;
    };

    struct EFI_CLASS pci_device_path : public card_device_path {
        uint8 device;
    };

    struct EFI_CLASS pccard_device_path : public card_device_path { };

    struct EFI_CLASS memory_mapped_device_path : public hardware_device_path {
        efi::memory_type memory_type;
        efi::uint64      start_address;
        efi::uint64      end_address;
    };

    struct EFI_CLASS vendor_device_path : public hardware_device_path {
        guid vendor_guid;
        FLEXIBLE_ARRAY( uint8, vendor_data )
    };

    struct EFI_CLASS controller_device_path : public hardware_device_path {
        uint32 controller_number;
    };

    struct EFI_CLASS baseboard_management_controller_device_path : public hardware_device_path {
        uint8  bmc_interface;
        uint64 base_address;
    };

    struct EFI_CLASS generic_acpi_device_path : public acpi_device_path {
        uint32 hardware_id;
        uint32 unique_id;
    };

    struct EFI_CLASS acpi_short_device_path : public generic_acpi_device_path { };

    struct EFI_CLASS apci_extended_device_path : public generic_acpi_device_path {
        uint32 compatible_id;
        // note: these are three null-terminated strings. However, since they
        // ALL have variable langth, we cannot store them as separate variables.
        FLEXIBLE_ARRAY( char, id_strings )
    };

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