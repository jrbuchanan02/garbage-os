/**
 * @file system.h++
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Describes a class which is a system's state.
 * @version 1
 * @date 2023-04-08
 *
 * @copyright Copyright (C) 2023. Intellectual property of the author(s) listed
 * above.
 *
 */

#ifndef KERNEL_TAKEOVER_SYSTEM_HPP
#define KERNEL_TAKEOVER_SYSTEM_HPP

#include <cstdint>

namespace takeover
{
    /**
     * @brief The types of system memory that the kernel cares about when it
     * sees a system-wide memory map.
     *
     */
    enum class system_memory_type
    {
        /**
         * @brief Memory available for use by the operating system for mostly
         * any purpose. It's possible that this RAM contains the kernel.
         */
        ram,
        /**
         * @brief Memory for use by the Runtime Services provided by the
         * system's EFI implementation. This memory must be preserved by the
         * kernel, but can be moved.
         *
         */
        efi_runtime,
        /**
         * @brief Memory currently used by system driver tables (eg, ACPI
         * tables). This memory can be reclaimed by the kernel when the kernel
         * is done with it.
         *
         */
        device_tables,
        /**
         * @brief Memory designated as an MMIO-style interface to a device. Only
         * the device module is allowed access to these physical addresses.
         *
         */
        device_access,
        /**
         * @brief Memory we cannot use but can move around in the address space.
         *
         */
        unusable,
        /**
         * @brief Memory like unusable memory, but we are not allowed even to 
         * move it around in the address space. Examples include regions marked
         * as EFI Reserved or the BIOS EDBA. This memory must be identity mapped.
         * 
         */
        unmappable,
    };

    struct memory_region
    {
        std::size_t const        start;
        std::size_t const        end;
        system_memory_type const type;
    };

    class system
    {
    public:
        // memory map of the system as given to the kernel by the loader.
        memory_region const *const memory_map;
    };

    struct immediate_state
    {
        bool preemptive_multitasking_ready;
        bool device_tables_compiled;
        bool memory_management_ready;
        
    };
}    //  namespace takeover

#endif    //  ifndef KERNEL_TAKEOVER_SYSTEM_HPP