/**
 * Simple framebuffer implementation.
 */

#ifndef SOURCE_COMMON_HAL_FRAMEBUFFER_H
#define SOURCE_COMMON_HAL_FRAMEBUFFER_H

#include <stdint.h>

enum _hal_framebuffer_return_status
{
    //  action successfully performed
    HAL_FRAMEBUFFER_SUCCESS,
    //  function was already called and cannot be called again.
    HAL_FRAMEBUFFER_ALREADY,
    //  function fails (eg, cannot access framebuffer in a video-video blt)
    HAL_FRAMEBUFFER_FAILURE,
};

typedef enum _hal_framebuffer_return_status hal_framebuffer_return_status;

struct _hal_framebuffer_pixel;

typedef struct _hal_framebuffer_pixel hal_framebuffer_pixel;

hal_framebuffer_return_status set_framebuffer ( void *framebuffer );
hal_framebuffer_return_status set_horizontal_resolution ( uint32_t hres );
hal_framebuffer_return_status set_vertical_resolution ( uint32_t vres );
// order of the three colors.
hal_framebuffer_return_status set_pixel_mapping ( uint32_t r,
                                                  uint32_t g,
                                                  uint32_t b );
hal_framebuffer_return_status plot_pixel ( uintmax_t             x,
                                           uintmax_t             y,
                                           hal_framebuffer_pixel p );

struct _hal_framebuffer_pixel
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

#endif    //  ifndef SOURCE_COMMON_HAL_FRAMEBUFFER_H