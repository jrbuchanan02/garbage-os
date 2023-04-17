/**
 * @file 1394.h++
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Definitions from the 1394 specification referenced in the UEFI standard's description for the
 * device path protocol
 * @version 1
 * @date 2023-04-13
 * 
 * It should be noted that this implementation of 1394's does not give any indiaction
 * of what the standard actually does. EFI mentions the standard in as a type of 
 * "Messaging Device Path" -- in the same group of standads like IP, USB, UART,
 * SPI, I2O, but also the VT-100 terminal.
 * 
 * @copyright This file is part of Garbage OS. Garbage OS is Copyright (C) 2023
 * Joshua Buchanan and published under the LGPL license version 2.1. You are 
 * free to use this source code in your project even if your code is not open 
 * source.
 */
#ifndef EXTERNAL_STANDARDS_SPEC1394_1394_HPP
#define EXTERNAL_STANDARDS_SPEC1394_1394_HPP

#include <cstdint>
#include <concepts>

namespace spec1394 {
    using guid = std::uint8_t[8];
}

#endif // ifndef EXTERNAL_STANDARDS_SPEC1394_1394_HPP