#ifndef __TWIST_H__
#define __TWIST_H__

#include <sys/types.h>

/*
  https://gcc.gnu.org/onlinedocs/gcc/Vector-Extensions.html
  https://clang.llvm.org/docs/LanguageExtensions.html#vector-literals
*/
typedef float float4 __attribute__((ext_vector_type(4)));

/*
typedef float float2 __attribute__((ext_vector_type(2)));

float4 foo(float2 a, float2 b) {
  float4 c;
  c.xz = a;
  c.yw = b;
  return c;
}

typedef unsigned char v4uc __attribute__ ((ext_vector_type (4)));
*/

void update_twist_color(uint32_t *a, uint32_t *b);

void Twister_init(void);  // init once
void Twister_draw(void);  // called each frame

#endif // __TWIST_H__
