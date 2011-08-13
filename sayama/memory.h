#ifndef __SY_MEMORY_H__
#define __SY_MEMORY_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

extern volatile void *sy_memset(volatile void *buf, int ch, size_t n);
extern volatile void *sy_memcpy(volatile void *dest,
    const volatile void *src, size_t n);
extern volatile void *sy_memmove(volatile void *dest,
    const volatile void *src, size_t n);

static inline volatile void *
sy_memzero(volatile void *buf, size_t n)
{
  return sy_memset(buf, 0, n);
}

#ifdef __cplusplus
}
#endif

#endif /* __SY_MEMORY_H__ */

