/**
 * @file main.h
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Contains enough information for the loaders to call kmain.
 * @version 1
 * @date 2022-03-23
 * 
 * @copyright Copyright (C) 2022. Intellectual property of the author(s) listed above.
 * 
 */
#ifndef KERNEL_ENTRY_MAIN_H
#define KERNEL_ENTRY_MAIN_H

void kmain(void *const mmap, void *const magic);

#endif // ifndef KERNEL_ENTRY_MAIN_H