/**
 * @file efidecl.h
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Definitions for specifically the EFI calling convention
 * @version 1
 * @date 2022-03-25
 *
 * @copyright Copyright (C) 2022. Intellectual property of the author(s) listed
 * above.
 *
 */
#ifndef SOURCE_KERNEL_LOADERS_EFI_INTERNAL_EFIDECL_H
#define SOURCE_KERNEL_LOADERS_EFI_INTERNAL_EFIDECL_H

#define IN
#define OUT
#define CONST const
#define OPTIONAL
#define EFIAPI __attribute__((ms_abi))

#endif // ifndef SOURCE_KERNEL_LOADERS_EFI_INTERNAL_EFIDECL_H