#ifndef GOS_LOADER_EFI_BOOT_SERVICES_HPP
#define GOS_LOADER_EFI_BOOT_SERVICES_HPP

#include <loader/efi/device_path_protocol.h++>
#include <loader/efi/event.h++>
#include <loader/efi/guid.h++>
#include <loader/efi/handle.h++>
#include <loader/efi/macros.h++>
#include <loader/efi/status.h++>
#include <loader/efi/table_header.h++>
#include <loader/efi/task_priority_level.h++>

#include <types/pointers.h++>

namespace efi {
    enum struct timer_delay_e {
        cancel,
        periodic,
        relative
    };

    enum struct allocate_type_e {
        any_pages,
        max_address,
        address,
        max_allocate_type
    };

    enum struct memory_type_e : std::uint32_t {
        reserved,
        loader_code,
        loader_data,
        boot_services_code,
        boot_services_data,
        runtime_services_code,
        runtime_services_data,
        conventional,
        unusable,
        acpi_reclaim,
        acpi_nvs,
        memory_mapped_io,
        memory_mapped_io_port_space,
        pal_code,
        persistent,
        unaccepted,
        max_memory_type
    };

    enum struct interface_type_e {
        native
    };

    enum struct locate_search_type_e {
        all_handles,
        by_register_nofiy,
        by_protocol
    };

    using physical_address_y = std::uint64_t;
    using virtual_address_y = std::uint64_t;

    EFI_STRUCT memory_descriptor {
        memory_type_e type;
        physical_address_y physical_start;
        virtual_address_y virtual_start;
        std::uint64_t page_count;
        union {
            EFI_STRUCT {
                std::uint64_t uncacheable : 1;
                std::uint64_t write_combining : 1;
                std::uint64_t write_through : 1;
                std::uint64_t write_back : 1;
                std::uint64_t uncacheable_exported : 1;
                std::uint64_t : 4;
                std::uint64_t write_protected : 1;  // the cacheability attribute (not sure what that is supposed to mean)
                std::uint64_t read_protected : 1;
                std::uint64_t execute_protected : 1;
                std::uint64_t nonvolatile : 1;
                std::uint64_t more_reliable : 1;
                std::uint64_t read_only : 1;
                std::uint64_t specific_purpose : 1;
                std::uint64_t cpu_crypto : 1;
                std::uint64_t : 46;
                std::uint64_t runtime : 1;
            } flags;
            std::uint64_t attribute;
        };
    };

    static_assert(sizeof(memory_descriptor) == 36);

    EFI_STRUCT open_protocol_information_entry {
        handle agent;
        handle controller;
        std::uint32_t attributes;
        std::uint32_t open_count;
    };
    static_assert(sizeof(open_protocol_information_entry) == 2 * sizeof(void *) + 8);

    EFIAPI EFI_STRUCT boot_services : public table_header {
        task_priority_level_e (*raise_tpl)(task_priority_level_e);
        void (*restore_tpl)(task_priority_level_e);

        status_y (*allocate_pages)(allocate_type_e, memory_type_e, std::size_t, physical_address_y *);
        status_y (*free_pages)(physical_address_y, std::size_t);
        status_y (*get_memory_map)(std::size_t *, memory_descriptor *, std::size_t *, std::size_t *, std::uint32_t *);
        status_y (*allocate_pool)(memory_type_e, std::size_t, void **);
        status_y (*free_pool)(void *);

        status_y (*create_event)(event_type_y, task_priority_level_e, event_notify_function, void *, event_y *);
        status_y (*set_timer)(event_y, timer_delay_e, std::uint64_t);
        status_y (*wait_for_event)(std::size_t, event_y *, std::size_t *);
        status_y (*signal_event)(event_y);
        status_y (*close_event)(event_y);
        status_y (*check_event)(event_y);
        
        status_y (*install_protocol_interface)(handle *, guid *, interface_type_e, void *);
        status_y (*reinstall_protocol_interface)(handle, guid *, void *, void *);
        status_y (*uninstall_protocol_interface)(handle, guid *, void *);
        status_y (*handle_protocol)(handle, guid *, void **);
        void *reserved;
        status_y (*register_protocol_notify)(guid *, event_y, void **);
        status_y (*locate_handle)(locate_search_type_e, guid *, void *, std::size_t *, handle *);
        status_y (*locate_device_path)(guid *, device_path_protocol **, handle *);
        status_y (*install_configuration_table)(guid *, void *);

        status_y (*load_image)(bool, handle, device_path_protocol *, void *, std::size_t, handle *);
        status_y (*image_start)(handle, std::size_t *, char16_t **);
        status_y (*exit)(handle, status_y, std::size_t, char16_t *);
        status_y (*unload_image)(handle);
        status_y (*exit_boot_services)(handle, std::size_t);

        status_y (*get_next_monotonic_count)(std::uint64_t *);
        status_y (*stall)(std::size_t);
        status_y (*set_watchdog_timer)(std::size_t, std::uint64_t, std::size_t, char16_t *);

        status_y (*connect_controller)(handle, handle *, device_path_protocol *, bool);
        status_y (*disconnect_controller)(handle, handle, handle);

        status_y (*open_protocol)(handle, guid *, void **, handle, handle, std::uint32_t);
        status_y (*close_protocol)(handle, guid *, handle, handle);
        status_y (*open_protocol_information)(handle, guid *, open_protocol_information_entry **, std::size_t *);
        
        status_y (*protocols_per_handle)(handle, types::ref<types::array<types::viewer<guid>>>, types::ref<std::size_t>);
        status_y (*locate_handle_buffer)(locate_search_type_e, guid *, void *, std::size_t *, handle *);
        status_y (*locate_protocol)(guid *, void *, void **);
        status_y (*install_multiple_protocol_interfaces)(handle *, ...);
        status_y (*uninstall_multiple_protocol_interfaces)(handle, ...);

        status_y (*calculate_crc32)(void *, std::size_t, std::uint32_t *);
        void (*copy_mem)(void *, void *, std::size_t);
        void (*set_mem)(void *, std::size_t, std::uint8_t);
        status_y (*create_event_ex)(event_type_y, task_priority_level_e, event_notify_function, void const *, guid const *, event_y *);
    };
}

#endif // ifndef GOS_LOADER_EFI_BOOT_SERVICES_HPP