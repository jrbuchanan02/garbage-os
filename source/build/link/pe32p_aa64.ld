/*
 * File: pe32p_aa64.ld
 * Author: Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * Brief: Provides parameters for pe32p.ld to create a linker script to output
 * a valid PE32+ executable for use in an operating system loader on 64-bit ARM
 * (aarch64)-based systems using EFI-compliant firmware.
 * Version: 1
 * Date 2023-07-20
 * Copyright: This file is part of Garbage OS. Garbage OS is Copyright (C) 2023
 * Joshua Buchanan and published under the LGPL license version 2.1. You are 
 * free to use this source code in your project even if your code is not open 
 * source.
 */

/* 
 * The include must refer relative to where ld is invoked. ld is invoked not in
 * the repository root but the build directory of the repository root so we have
 * to navigate one level above where we are at to get the repository root and 
 * avoid file-not-found errors.
 */
INCLUDE ../source/build/link/pe32p.ld
/*
 * Currently the only part of the format which changes from architecture to 
 * architecture.
 */
machine = 0xAA64;