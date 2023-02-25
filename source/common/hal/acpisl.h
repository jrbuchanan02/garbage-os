/**
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief ASL interpreter for the ACPI standard.
 * @version 1
 * @date 2023-02-23
 * @copyright Copyright (C) 2023. Intellectual property of the author(s) listed
 * above.
 */

#ifndef SOURCE_COMMON_HAL_ACPISL_H
#define SOURCE_COMMON_HAL_ACPISL_H

#include <common/hal/acpi.h>

struct _asl_entry;

//  how AML works:
//  example ASL (excl. header)           | Resulting AML:
//  Scope ( _SB )                        | 0x10 0x15 _SB_[
//  {                                    |
//   Device (PCI0)                       | 0x82 0x0f PCI0 0x08
//   {                                   |
//       Name (_HID, EisaId, "PNP0A03")  | _HID 0x0C 0xD0 0x0A 0x03 0x0A
//   }
//  }



#endif    //  ifndef SOURCE_COMMON_HAL_ACPISL_H