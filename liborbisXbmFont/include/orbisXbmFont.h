/*
    Example:
    update_gradient(&c1, &c2);
    sprintf(tmp_ln, "0x%p, %d bytes:", data, data->len);
    x = get_aligned_x(tmp_ln, CENTER);
    print_text(x, y, tmp_ln);
*/

#pragma once

/* generate with genXBMfonts, https://github.com/masterzorag/xbm_tools */
#include "xbm_font.h"

#ifdef __cplusplus
extern "C" {
#endif
	
// for text alignment, sorting matters!
#define LEFT       0  // useless
#define RIGHT      1
#define CENTER     2

#define SHADOW_PX  2
#define SIMPLE_GRADIENT_STEPS    ((uint8_t)(FONT_H /2))  // steps we split delta


/***********************************************************************
* update_gradient
*
* precompute palette to use in print_text() and setup colors range
*
* uint32_t *a     = pointer to start color
* uint32_t *b     = pointer to end color
***********************************************************************/
void update_gradient(const uint32_t *a, const uint32_t *b);


/***********************************************************************
* compute x to align text into canvas
*
* const char *str = referring string
* uint8_t align.  = RIGHT / CENTER
***********************************************************************/
uint16_t get_aligned_x(const char *str, const uint8_t alignment);


/***********************************************************************
* print text, with bitmap data from xbm_font.h
*
* int32_t x       = start x coordinate into canvas
* int32_t y       = start y coordinate into canvas
* const char *str = string to print
***********************************************************************/
int32_t print_text(int32_t x, int32_t y, const char *str);

#ifdef __cplusplus
}
#endif
