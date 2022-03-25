/**
 * @file halt.h
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Function to halt the processor.
 * @version 1
 * @date 2022-03-25
 *
 * @copyright Copyright (C) 2022. Intellectual property of the author(s) listed
 * above.
 *
 */
#ifndef SOURCE_KERNEL_MACHINE_HALT_H
#define SOURCE_KERNEL_MACHINE_HALT_H

/**
 * @brief Halts the processor and clears interrupts.
 * @note Typically only accessable through the highest permission level. Ensure
 * that the reset for the permission levels will not end up resetting the system
 * when on EFI and when we are in the boot process.
 * @note This function does not return.
 * @note If the processor supports an NMI, that may cause control flow to leave
 * the halt instruction within this function. That is the point of an NMI so
 * (while hacks around it probably exist) this function does not mess with the
 * interrupt table.
 */
extern void halt();

#endif // ifndef SOURCE_KERNEL_MACHINE_HALT_H
