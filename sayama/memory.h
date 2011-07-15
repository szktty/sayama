#ifndef __SY_MEMORY_H__
#define __SY_MEMORY_H__

#ifdef __cplusplus
extern "C"
{
#endif

extern void *sy_memset(void *buf, int ch, size_t n);
extern void *sy_memmove(void *dest, const void *src, size_t n);

static inline void *
sy_memzero(void *buf, size_t n)
{
  return sy_memset(buf, 0, n);
}

#ifdef __cplusplus
}
#endif

#endif /* __SY_MEMORY_H__ */

