/**
 * @file serialwrite.c
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com) (github: @natara1939344)
 * @brief implements writing to serial port
 * @version 0.1
 * @date 2021-04-15
 * 
 * @copyright Copyright (C) 2021 under the 
 * 
 */

#include "../../intf/kernel/serialwrite.h"

EFI_SYSTEM_TABLE *efi_table;

void startup_service()
{
    efi_table = (EFI_SYSTEM_TABLE*)bootboot.arch.x86_64.efi_ptr;

    EFI_BOOT_SERVICES *boot_services = efi_table->BootServices;
    EFI_HANDLE *debug_port_handle = (EFI_HANDLE*)NULL;
    uint64_t buffer_size = sizeof(EFI_HANDLE);
    EFI_GUID serial_port_guid = SERIAL_IO_PROTOCOL;
    boot_services->LocateHandle(2, &serial_port_guid, NULL, &buffer_size, debug_port_handle);    

}