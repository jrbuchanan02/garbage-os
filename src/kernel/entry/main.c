/**
 * @file main.c
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Entry point for the Operating System
 * @version 1
 * @date 2022-03-23
 *
 * @copyright Copyright (C) 2022. Intellectual property of the author(s) listed above.
 *
 */

/**
 * @brief Kernel Entry point. All architectures end up calling this function after
 * their respective loading and setup processes.
 *
 * @param mmap the location of the memory map. It's currently a void pointer because
 * the memory map type is not yet defined
 *
 * @param magic the location of the magic number which tells us what specific
 * environment we are in. It's a void pointer since the magic type is not yet defined.
 */
void kmain(void *const mmap, void *const magic)
{
}