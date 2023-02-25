
#include <common/hal/framebuffer.h>

uint32_t volatile *framebuffer = 0;

uintmax_t hres = 0, vres = 0;

uint32_t r_map = 0, g_map = 0, b_map = 0;

hal_framebuffer_return_status set_framebuffer ( void *frame_buffer )
{
    if ( framebuffer != 0 ) { return HAL_FRAMEBUFFER_ALREADY; }
    framebuffer = ( uint32_t * ) frame_buffer;
    return HAL_FRAMEBUFFER_SUCCESS;
}

hal_framebuffer_return_status set_horizontal_resolution ( uint32_t h )
{
    if ( hres != 0 ) { return HAL_FRAMEBUFFER_ALREADY; }
    hres = h;
    return HAL_FRAMEBUFFER_SUCCESS;
}

hal_framebuffer_return_status set_vertical_position ( uint32_t v )
{
    if ( vres != 0 ) { return HAL_FRAMEBUFFER_ALREADY; }
    vres = v;
    return HAL_FRAMEBUFFER_SUCCESS;
}

hal_framebuffer_return_status set_pixel_mapping ( uint32_t r,
                                                  uint32_t g,
                                                  uint32_t b )
{ 
    if ( r_map )
    {
        return HAL_FRAMEBUFFER_ALREADY;
    }
    r_map = r;
    g_map = g;
    b_map = b;
    return HAL_FRAMEBUFFER_SUCCESS;
}


hal_framebuffer_return_status plot_pixel(uintmax_t x, uintmax_t y, hal_framebuffer_pixel p)
{
    uint32_t *pixel = &framebuffer[x + y * hres];

    uint8_t first, second, third;
    if ( r_map == 1 )
    {
        first = p.r;
        if ( g_map == 2)
        {
            second = p.g;
            third = p.b;
        } else
        {
            second = p.b;
            third = p.g;
        }
    } else if ( g_map == 1)
    {
        first = p.g;
        if ( r_map == 2 )
        {
            second = p.r;
            third = p.b;
        } else
        {
            second = p.b;
            third = p.r;
        }
    } else if ( b_map == 1)
    {
        first = p.b;
        if ( r_map == 2 )
        {
            second = p.r;
            third = p.g;
        } else
        {
            second = p.g;
            third = p.r;
        }
    }

    *pixel = 0;

    // b32       -->                   b0
    // reserved | first | second | third
    *pixel |= (uint32_t)(first << 16);
    *pixel |= (uint32_t)(second << 8);
    *pixel |= (uint32_t)(third << 0);
    return HAL_FRAMEBUFFER_SUCCESS;
}