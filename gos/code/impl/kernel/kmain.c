/**
 * @file kmain.c
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com) (github: @natara1939344)
 * @brief 
 * @version 0.1
 * @date 2021-04-10
 * 
 * @copyright Copyright (C) 2021 under the LGPL version 2.1
 * 
 */
#include "../../intf/kernel/kmain.h"


/**
 * @brief kernel main function
 * @date 2021-04-10
 */
void kmain(void)
{
    int x, y, s=bootboot.fb_scanline, w=bootboot.fb_width, h=bootboot.fb_height;

    if(s) {
        // cross-hair to see screen dimension detected correctly
        for(y=0;y<h;y++) { *((uint32_t*)(&framebuffer + s*y + (w*2)))=0x00FFFFFF; }
        for(x=0;x<w;x++) { *((uint32_t*)(&framebuffer + s*(h/2)+x*4))=0x00FFFFFF; }

        // red, green, blue boxes in order
        for(y=0;y<20;y++) { for(x=0;x<20;x++) { *((uint32_t*)(&framebuffer + s*(y+20) + (x+20)*4))=0x00FF0000; } }
        for(y=0;y<20;y++) { for(x=0;x<20;x++) { *((uint32_t*)(&framebuffer + s*(y+20) + (x+50)*4))=0x0000FF00; } }
        for(y=0;y<20;y++) { for(x=0;x<20;x++) { *((uint32_t*)(&framebuffer + s*(y+20) + (x+80)*4))=0x000000FF; } }

        // say hello
        //puts("Hello from a simple BOOTBOOT kernel");
    }
    // hang for now
    while(1);

}
/**
 * @brief bootboot requires entry point to be called start.
 * On the other hand, I want my entry point to be called kmain.
 * @author Joshua Buchannan (joshuarobertbuchanan@gmail.com) (github: @natara1939344)
 * @date 2021-04-10
 * @version 0.1
 */
void _start(void) {
    kmain();
}
