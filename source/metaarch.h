/**
 * @file metaarch.h
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Metaprogramming and metadata about our current architectures.
 * @version 1
 * @date 2022-03-24
 *
 * @copyright Copyright (C) 2022. Intellectual property of the author(s) listed
 * above.
 *
 */
#ifndef METAARCH_H
#define METAARCH_H

//  Same order as in UEFI standard:
//  IA32
//  IA64
//  x86-64
//  aarch32 (ARM)
//  aarch64
//  riscv (32, 64, then 128-bits)

//  The "dream" systems, which would imply running GOS on just about everything
//  that can run an operating system using preemptive multitasking
//  m68000 (the 16-bit iterations found in the original 68000 and the 68010)
//  m680x0 (the 32-bit iterations)
//  Power32 (any POWER ISA set in 32-bits)
//  Power64 (any POWER ISA set in 64-bits)
//  65c816 (hopefully, this is the 16-bit version of the equally famous 6502)
//  i286 (16-bit x86 with its own protected mode)

//  The preprocessor defines control the following macros:
//  1. ARCH_BITS: the width of our registers / our ALU
//  2. ADDR_BITS: smallest power of 2 to span the address space
//  3. SIZE_BITS: size to use for size_t
//  4. FLAT: 1 if the processor uses a flat memory model
//  5. EVIL: 1 if the architecture is Itanium or traditionally has not had C
//           language support. Itanium is EVIL because of the complexity of the
//           architecture making its ASM practically unreadable by humans.
//  6. ENDIAN: 1 for little endian systems or bi-endian systems that can switch
//             to little endian without a performance loss.

#if defined( __IA32__ )

  #define ARCH_BITS 32
  #define ADDR_BITS 36
  #define SIZE_BITS 32
  #define FLAT      1
  #define EVIL      0
  #define ENDIAN    1

#elif defined( __IA64__ )

  #define ARCH_BITS 64
  #define ADDR_BITS 64
  #define SIZE_BITS 64
  #define FLAT      1
  #define EVIL      1
  #define ENDIAN    1

#elif defined( __X86_64__ )

  #define ARCH_BITS 64
  #define ADDR_BITS 57
  #define SIZE_BITS 64
  #define FLAT      1
  #define EVIL      0
  #define ENDIAN    1

#elif defined( __ARM__ )

  #define ARCH_BITS 32
  #define ADDR_BITS 32
  #define SIZE_BITS 32
  #define FLAT      1
  #define EVIL      0
  #define ENDIAN    1

#elif defined( __AARCH64__ )

  #define ARCH_BITS 64
  #define ADDR_BITS 64
  #define SIZE_BITS 64
  #define FLAT      1
  #define EVIL      0
  #define ENDIAN    1

#elif defined( __RISCV_X32__ )

  #define ARCH_BITS 32
  #define ADDR_BITS 32
  #define SIZE_BITS 32
  #define FLAT      1
  #define EVIL      0
  #define ENDIAN    1

#elif defined( __RISCV_X64__ )

  #define ARCH_BITS 64
  #define ADDR_BITS 64
  #define SIZE_BITS 64
  #define FLAT      1
  #define EVIL      0
  #define ENDIAN    1

#elif defined( __RISCV_128__ )

  #define ARCH_BITS 128
  #define ADDR_BITS 128
  #define SIZE_BITS 128
  #define FLAT      1
  #define EVIL      1
  #define ENDIAN    1

#elif defined( __M68000__ )

  #define ARCH_BITS 16    //  note: 32-bit registers.
  #define ADDR_BITS 24    //  note: 32-bit registers.
  #define SIZE_BITS 32    //  note: 32-bit registers.
  #define FLAT      1
  #define EVIL      0
  #define ENDIAN    1

#elif defined( __M680X0__ )

  #define ARCH_BITS 32
  #define ADDR_BITS 32
  #define SIZE_BITS 32
  #define FLAT      1
  #define EVIL      0
  #define ENDIAN    1

#elif defined( __POWER32__ )

  #define ARCH_BITS 32
  #define ADDR_BITS 32
  #define SIZE_BITS 32
  #define FLAT      1
  #define EVIL      0
  #define ENDIAN    0

#elif defined( __POWER64__ )

  #define ARCH_BITS 64
  #define ADDR_BITS 64
  #define SIZE_BITS 64
  #define FLAT      1
  #define EVIL      0
  #define ENDIAN    0

#elif defined( __65C816__ )

  #define ARCH_BITS 16
  #define ADDR_BITS 24
  #define SIZE_BITS 16
  #define FLAT      0
  #define EVIL      1
  #define ENDIAN    1

#elif defined( __I286__ )

  #define ARCH_BITS 16
  #define ADDR_BITS 20
  #define SIZE_BITS 32
  #define FLAT      0
  #define EVIL      1
  #define ENDIAN    1

#else    //  default values to let computers help with the code inspection.

  #define ARCH_BITS 64
  #define ADDR_BITS 64
  #define SIZE_BITS 64
  #define FLAT      1
  #define EVIL      0
  #define ENDIAN    1

#endif    //  if defined(__IA32__)

// clang-format off
#define EFI_FILE_PROLOGUE                                                            \
  _Pragma ( "GCC diagnostic push" )                                                  \
  _Pragma ( "message \"Pushing diagnostic: \'-Wpedantic\' to include this file.\"" ) \
  _Pragma ( "GCC diagnostic ignored \"-Wpedantic\"" )                                \
  
#define EFI_FILE_EPILOGUE _Pragma ( "GCC diagnostic pop" )
// clang-format on
#endif    //  ifndef METAARCH_H