/**
 * @file metaarch.h
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Metaprogramming and metadata about our current architectures.
 * @version 1
 * @date 2022-03-24
 * 
 * @copyright Copyright (C) 2022. Intellectual property of the author(s) listed above.
 * 
 */
#ifndef SOURCE_METAARCH_H
#define SOURCE_METAARCH_H

// Same order as in UEFI standard:
// IA32
// IA64
// x86-64
// aarch32 (ARM)
// aarch64
// riscv (we have three subcategories of this one)
// We also add some architectures that I (currently) think would be cool to 
// flex knowledge on and to maybe support oneday:
// m68000 (the 16-bit iterations found in the original 68000 and the 68010)
// m680x0 (the 32-bit iterations)
// Power32 (any POWER ISA set in 32-bits)
// Power64 (any POWER ISA set in 64-bits)
// 65c816 (hopefully, this is the 16-bit version of the equally famous 6502)
// i286 (16-bit x86 with its own protected mode)
// It's possible that we never get to run on those architectures due to the 
// lack of (in-production, readily accessible) hardware for some and limitations
// for others (m68000, 65c816, and i286 lack paging; the first two lack memory
// protections). But we can dream!

// The macros we currently populate per architecture are as follows:
// 1. ARCH_BITS: the amount of bits that should be UINTN; not necessarily equal
// to size_t.
// 2. ADDR_BITS: the amount of bits necessary to span the address space. This is
// a theoretical minimum to support all processors on the architecture and may
// be larger than the machine's current arch.
// 3. SIZE_BITS: bits that should be in size_t
// 5. FLAT: true for a flat memory model, that is, when we specify a pointer, is
// it enough to access any given memory address.
// 6. EVIL: true if the machine ISA is notorious for being difficult to read 
// and debug. EVIL architectures might be the only place where the compiler could
// theoretically, possibly, maybe, have a chance of being the source of a bug.
// GOS also shouldn't show too many disassemblies of EVIL assembly instructions.
// 7. ENDIAN: 1 for little endian (most architectures are), 0 for BIG endian. if
// an architecture is bi-endian, GOS will note LITTLE endian to keep the binaries
// in GOS consistent.

#if defined(__IA32__)

#define ARCH_BITS 32
#define ADDR_BITS 36 // PAE adds 4 bits to the address space
#define SIZE_BITS 32
#define FLAT 1
#define EVIL 0
#define ENDIAN 1

#elif defined(__IA64__)

#define ARCH_BITS 64
#define ADDR_BITS 64
#define SIZE_BITS 64
#define FLAT 1 // (unverified)
#define EVIL 1 // see the readme.md
#define ENDIAN 1

#elif defined(__X86_64__)

#define ARCH_BITS 64
#define ADDR_BITS 57 // most systems use 48 or something around there
#define SIZE_BITS 64
#define FLAT 1
#define EVIL 0
#define ENDIAN 1

#elif defined(__ARM__)

#define ARCH_BITS 32
#define ADDR_BITS 32 // (unverified) Quite possibly too low.
#define SIZE_BITS 32
#define FLAT 1
#define EVIL 0
#define ENDIAN 1

#elif defined(__AARCH64__)

#define ARCH_BITS 64
#define ADDR_BITS 64 // (unverified) Quite possibly too high.
#define SIZE_BITS 64
#define FLAT 1
#define EVIL 0
#define ENDIAN 1

#elif defined(__RISCV_X32__)

#define ARCH_BITS 32 
#define ADDR_BITS 32 // (unverified)
#define SIZE_BITS 32
#define FLAT 1 // (unverified)
#define EVIL 0
#define ENDIAN 1 // (unverified)

#elif defined(__RISCV_X64__)

#define ARCH_BITS 64
#define ADDR_BITS 64 // (unverified)
#define SIZE_BITS 64
#define FLAT 1 // (unverified)
#define EVIL 0
#define ENDIAN 1 // (unverified)

#elif defined(__RISCV_128__)

#define ARCH_BITS 128
#define ADDR_BITS 128 // (unverified)
#define SIZE_BITS 128
#define FLAT 1 // (unverified)
#define EVIL 0
#define ENDIAN 1 // (unverified)

#elif defined(__M68000__)

#define ARCH_BITS 16 // note: 32-bit registers.
#define ADDR_BITS 24 // note: 32-bit registers.
#define SIZE_BITS 32 // note: 32-bit registers.
#define FLAT 1
#define EVIL 0
#define ENDIAN 1

#elif defined(__M680X0__)

#define ARCH_BITS 32
#define ADDR_BITS 32
#define SIZE_BITS 32
#define FLAT 1
#define EVIL 0
#define ENDIAN 1

#elif defined(__POWER32__)

#define ARCH_BITS 32
#define ADDR_BITS 32 // (unverified)
#define SIZE_BITS 32
#define FLAT 1
#define EVIL 0
#define ENDIAN 0 // big endian; not sure about a little endian mode.

#elif defined(__POWER64__)

#define ARCH_BITS 64
#define ADDR_BITS 64 // (unverified)
#define SIZE_BITS 64
#define FLAT 1
#define EVIL 0
#define ENDIAN 0 // big endian; not sure about a little endian mode.

#elif defined(__65C816__)

#define ARCH_BITS 16
#define ADDR_BITS 24
#define SIZE_BITS 16
// if you have the instructions:
// lda #0xAABBCC
// ldx #0xDDEE
// ldy #0xFF00
// then x and y are loaded from:
// 0xAADDEE and 0xAAFF00
// meaning that there is a hidden section of the memory address; qualifying
// the memory model as *segmented* by GOS's standards.
#define FLAT 0
#define EVIL 1
#define ENDIAN 1

#elif defined(__I286__)

#define ARCH_BITS 16
#define ADDR_BITS 20 // see schpiel on this arch in the readme.
#define SIZE_BITS 32 // definition of size_t states that it can span entire address space.
#define FLAT 0
#define EVIL 1
#define ENDIAN 1

#else // default values to let computers help with the code inspection.

#define ARCH_BITS 64
#define ADDR_BITS 64
#define SIZE_BITS 64
#define FLAT 1
#define EVIL 0
#define ENDIAN 1


#endif // if defined(__IA32__)

#endif // ifndef SOURCE_METAARCH_H