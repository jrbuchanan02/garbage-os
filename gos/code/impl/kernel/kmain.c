/**
 * @file kmain.c
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com) (github: @natara1939344)
 * @brief entry point for the operating system.
 * @version 0.1
 * @date 2021-04-10
 * 
 * @copyright Copyright (C) 2021 under the LGPL version 2.1
 * 
 */
#include "../../intf/kernel/kmain.h"



/**
 * @brief bootboot requires entry point to be called start.
 * On the other hand, I want my entry point to be called kmain. Which is sad.
 * @author Joshua Buchannan (joshuarobertbuchanan@gmail.com) (github: @natara1939344)
 * @date 2021-04-10
 * @version 0.1
 */
void _start(void) {
    /*** NOTE: this code runs on all cores in parallel ***/
    int x, y, s=bootboot.fb_scanline, w=bootboot.fb_width, h=bootboot.fb_height;

    if(s) {
        // cross-hair to see screen dimension detected correctly
        for(y=0;y<h;y++) { *((uint32_t*)(&fb + s*y + (w*2)))=0x00FFFFFF; }
        for(x=0;x<w;x++) { *((uint32_t*)(&fb + s*(h/2)+x*4))=0x00FFFFFF; }

        // red, green, blue boxes in order
        for(y=0;y<20;y++) { for(x=0;x<20;x++) { *((uint32_t*)(&fb + s*(y+20) + (x+20)*4))=0x00FF0000; } }
        for(y=0;y<20;y++) { for(x=0;x<20;x++) { *((uint32_t*)(&fb + s*(y+20) + (x+50)*4))=0x0000FF00; } }
        for(y=0;y<20;y++) { for(x=0;x<20;x++) { *((uint32_t*)(&fb + s*(y+20) + (x+80)*4))=0x000000FF; } }

        // say hello
        //put_string("Hello from a simple BOOTBOOT kernel");
    }
    // fill screen with color changing square
    rectangle_t rectangle = {0x00, 0x00, 0x00, 0x00, 0LLU, 0LLU, bootboot.fb_width, bootboot.fb_height};
    while (1) {
        if (rectangle.fill_b == 0xff) {
            if (rectangle.fill_g == 0xff) {
                rectangle.fill_r++;
            }
            rectangle.fill_g++;
        }
        rectangle.fill_b++;

        draw(&rectangle);
    }
}

void draw(rectangle_t* rectangle) {
    uint64_t cursor_y;
    uint64_t cursor_x;

    uint64_t y_offset = bootboot.fb_height - 1 - rectangle->y_position;
    uint64_t x_offset = bootboot.fb_scanline + 4;

    for (cursor_y = 0; cursor_y < rectangle->height; cursor_y++) {
        for (cursor_x = 0; cursor_x < rectangle->width; cursor_x++) {
            uint64_t offset = (y_offset - cursor_y) * (x_offset * rectangle->y_position * cursor_x);
            (*(uint32_t*)(fb + offset)) = *(uint32_t*)(rectangle); // probably puppy killing hack, fix later.
        }
    }
}