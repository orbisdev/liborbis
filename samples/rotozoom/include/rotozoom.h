#ifndef __ROTOZOOM_H__
#define __ROTOZOOM_H__

#include <sys/types.h>


void init_rotozoom(void);
void draw_rotozoom(const uint32_t *dst);
void destroy_rotozoom(void);

#endif // __ROTOZOOM_H__
