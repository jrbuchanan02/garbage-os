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
 * @brief kernel main function
 * FIXME #25 - this issue seems to be quite perplexing.
 * @date 2021-04-10
 */
void kmain(void)
{
    uint32_t red, green, blue;
    switch (bootboot.fb_type) {
        default:
        case FB_ABGR:
            red =   0x000000FF;
            green = 0x0000FF00;
            blue =  0x00FF0000;
            break;
        case FB_ARGB:
            red =   0x00FF0000;
            green = 0x0000FF00;
            blue =  0x000000FF;
            break;
        case FB_BGRA:
            red =   0x0000FF00;
            green = 0x00FF0000;
            blue =  0xFF000000;
            break;
        case FB_RGBA:
            red =   0xFF000000;
            green = 0x00FF0000;
            blue =  0x0000FF00;
            break;
    }

    volatile uint32_t* framebuffer = (uint32_t*)bootboot.fb_ptr;   
    uint32_t x;
    uint32_t y;
    volatile uint32_t w = bootboot.fb_width;
    volatile uint32_t h = bootboot.fb_height;
    volatile uint32_t s = bootboot.fb_scanline;
    // spin until s
    while (!s)
    {
        s = bootboot.fb_scanline;
    }

    if(s) {
        // cross-hair to see screen dimension detected correctly
        for(y=0;y<h;y++) { framebuffer[s * y + (w * 2)] = red | green | blue; }
        for(x=0;x<w;x++) { framebuffer[s* (h / 2)+ x * 4] = red | green | blue; }

        // red, green, blue boxes in order
        for(y=0;y<20;y++) { for(x=0;x<20;x++) { framebuffer[s * (y + 20) + (x + 20) * 4] = red; } }
        for(y=0;y<20;y++) { for(x=0;x<20;x++) { framebuffer[s * (y + 20) + (x + 50) * 4] = green; } }
        for(y=0;y<20;y++) { for(x=0;x<20;x++) { framebuffer[s * (y + 20) + (x + 80) * 4] = blue; } }
        // say hello
        //puts("Hello from a simple BOOTBOOT kernel");
    }
    //unsigned int no_s_counter = 0;
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


