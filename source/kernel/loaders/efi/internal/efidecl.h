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
#ifndef KERNEL_LOADERS_EFI_INTERNAL_EFIDECL_H
#define KERNEL_LOADERS_EFI_INTERNAL_EFIDECL_H

#define IN
#define OUT
#define CONST const
#define OPTIONAL

#if defined( __X64__ ) || defined( __X32__ )
  #define EFIAPI __attribute__ ( ( ms_abi ) )
#elif defined( __A32__ )
  #define EFIAPI __attribute__ ( ( target ( "arm" ) ) )
#else
  #define EFIAPI

#endif    //  defined(__X64__) || defined(__X32__)

#endif    //  ifndef KERNEL_LOADERS_EFI_INTERNAL_EFIDECL_H