#ifndef __SY_LARGE_INT_H__
#define __SY_LARGE_INT_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct sy_large_int_header  sy_large_int_header;

struct sy_large_int_header {
  bool sign;
  unsigned int len;
};

#define SY_DEF_LARGE_INT_FUNCS(bits, len) \
  typedef struct sy_large_int_##bits { \
    sy_large_int_header hdr; \
    uint32_t d[len]; \
  }; \
\
  static inline void \
  sy_init_large_int_##bits(sy_large_int_##bits *v) \
  { \
    sy_init_large_int(v->hdr, v->d);
  } \
\
  static inline void \
  sy_compare_large_ints_##bits(const sy_large_int_##bits *left, \
    const sy_large_int_##bits *right) \
  { \
    sy_compare_large_ints(left->hdr, left->d, right->hdr, right->d); \
  } \

static inline void sy_copy_large_int_##bits(sy_large_int_##bits *dest,
    const sy_large_int_##bits *src);

static inline bool sy_large_int_##bits##_set_str(sy_large_int_##bits *v,
    const char *s, unsigned int base);
static inline bool sy_large_int_##bits##_set_int(sy_large_int_##bits *v,
    int32_t v);
static inline bool sy_large_int_##bits##_set_uint(sy_large_int_##bits *v,
    uint32_t v);

static inline bool sy_add_large_ints_##bits(sy_large_int_##bits *dest,
    const sy_large_int_##bits *left, const sy_large_int_##bits *right);
static inline bool sy_sub_large_ints_##bits(sy_large_int_##bits *dest,
    const sy_large_int_##bits *left, const sy_large_int_##bits *right);

#ifdef __cplusplus
}
#endif

#endif /* __SY_LARGE_INT_H__ */

