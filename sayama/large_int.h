#ifndef __SY_LARGE_INT_H__
#define __SY_LARGE_INT_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct sy_large_int_header  sy_large_int_header;

struct sy_large_int_header {
  bool sign;
  unsigned int len;
};

static inline void sy_init_large_int(sy_large_int_header *hdr, uint32_t *d,
    unsigned int len);
static inline void sy_copy_large_int(sy_large_int_header *dest_hdr,
    uint32_t *dest_d, const sy_large_int_header *src_hdr,
    const uint32_t *src_d);
extern bool sy_large_int_set_str(sy_large_int_header *hdr, uint32_t *d,
    const char *s, unsigned int base);
extern void sy_large_int_set_long(sy_large_int_header *hdr, uint32_t *d,
    long l);
extern void sy_large_int_set_ulong(sy_large_int_header *hdr, uint32_t *d,
    unsigned long ul);

extern int sy_compare_large_ints(const sy_large_int_header *left_hdr,
    const uint32_t *left_d, const sy_large_int_header *right_hdr,
    const uint32_t *right_d);

extern bool sy_add_large_ints(sy_large_int_header *res_hdr,
    uint32_t *res_d, const sy_large_int_header *left_hdr,
    const uint32_t *left_d, const sy_large_int_header *right_hdr,
    const uint32_t *right_d);
extern bool sy_sub_large_ints(sy_large_int_header *res_hdr,
    uint32_t *res_d, const sy_large_int_header *left_hdr,
    const uint32_t *left_d, const sy_large_int_header *right_hdr,
    const uint32_t *right_d);

static inline void
sy_init_large_int(sy_large_int_header *hdr, uint32_t *d, unsigned int len)
{
  hdr->sign = true;
  hdr->len = len;
  memset(d, 0, sizeof(uint32_t) * len);
}

static inline void
sy_copy_large_int(sy_large_int_header *dest_hdr, uint32_t *dest_d,
   const sy_large_int_header *src_hdr, const uint32_t *src_d)
{
  dest_hdr->sign = src_hdr->sign;
  memcpy(dest_d, src_d, sizeof(uint32_t) * dest_hdr->len);
}

#define SY_DEF_LARGE_INT_FUNCS(bits) \
  _SY_DEF_LARGE_INT_FUNCS(bits, (bits/32))
#define _SY_DEF_LARGE_INT_FUNCS(bits, dlen) \
  typedef struct sy_large_int_##bits { \
    sy_large_int_header hdr; \
    uint32_t d[(dlen)]; \
  } sy_large_int_##bits; \
\
  static inline void \
  sy_init_large_int_##bits(sy_large_int_##bits *v) \
  { \
    sy_init_large_int(&v->hdr, v->d, (dlen)); \
  } \
\
  static inline void \
  sy_copy_large_int_##bits(sy_large_int_##bits *dest, \
    const sy_large_int_##bits *src) { \
    sy_copy_large_int(&dest->hdr, dest->d, &src->hdr, src->d); \
  } \
\
  static inline bool \
  sy_large_int_##bits##_set_str(sy_large_int_##bits *v, \
    const char *s, unsigned int base) \
  { \
    return sy_large_int_set_str(&v->hdr, v->d, s, base); \
  } \
\
  static inline void \
  sy_large_int_##bits##_set_long(sy_large_int_##bits *v, \
    long l) \
  { \
    sy_large_int_set_long(&v->hdr, v->d, l); \
  } \
\
  static inline void \
  sy_large_int_##bits##_set_ulong(sy_large_int_##bits *v, \
    unsigned long ul) \
  { \
    sy_large_int_set_ulong(&v->hdr, v->d, ul); \
  } \
\
  static inline int \
  sy_compare_large_ints_##bits(const sy_large_int_##bits *left, \
    const sy_large_int_##bits *right) \
  { \
    return sy_compare_large_ints(&left->hdr, left->d, \
        &right->hdr, right->d); \
  } \
\
  static inline bool \
  sy_add_large_ints_##bits(sy_large_int_##bits *res, \
    const sy_large_int_##bits *left, const sy_large_int_##bits *right) \
  { \
    return sy_add_large_ints(&res->hdr, res->d, &left->hdr, left->d, \
      &right->hdr, right->d); \
  } \
\
  static inline bool \
  sy_sub_large_ints_##bits(sy_large_int_##bits *res, \
    const sy_large_int_##bits *left, const sy_large_int_##bits *right) \
  { \
    return sy_sub_large_ints(&res->hdr, res->d, &left->hdr, left->d, \
      &right->hdr, right->d); \
  }

SY_DEF_LARGE_INT_FUNCS(128)

#ifdef __cplusplus
}
#endif

#endif /* __SY_LARGE_INT_H__ */

