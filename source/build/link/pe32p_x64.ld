/* 
 * File: pe32p_x64.ld
 * Author: Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * Brief: Provides parameters for pe32p.ld to create a linker script to output
 * a valid PE32+ executible for use in an operating system loader on x64-based
 * systems using EFI-compliant firmware.
 * Version: 1
 * Date: 2023-04-16
 * Copyright: This file is part of Garbage OS. Garbage OS is Copyright (C) 2023
 * Joshua Buchanan and published under the LGPL license version 2.1. You are 
 * free to use this source code in your project even if your code is not open 
 * source.
 */
INCLUDE ../source/build/link/pe32p.ld
machine = 0x8664;