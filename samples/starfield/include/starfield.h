#ifndef __STARFLD_H__
#define __STARFLD_H__

#include <sys/types.h>

typedef struct _STAR{
    float xpos, ypos;
    short zpos, speed;
    uint8_t color;
} STAR;
//__attribute__((aligned(16)));

#define NUMBER_OF_STARS 256*4     // max 2^16 for uint16_t


void init_star(STAR *star, const uint16_t i);
void init_starfield(void);
void draw_starfield(void);

#endif // __STARFLD_H__
