#ifndef __SY_WORD_H__
#define __SY_WORD_H__

#include <sayama/memory.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

static inline uint32_t sy_word_value(const uint8_t *bytes);
static inline void sy_set_word_value(uint8_t *dest, uint32_t v);
static inline void sy_land_word(uint8_t *dest,
    const uint8_t *w1, const uint8_t *w2);
static inline void sy_land_words(uint8_t *dest,
    const uint8_t *w1, const uint8_t *w2, size_t len);
static inline void sy_lor_word(uint8_t *dest,
    const uint8_t *w1, const uint8_t *w2);
static inline void sy_lor_words(uint8_t *dest,
    const uint8_t *w1, const uint8_t *w2, size_t len);
static inline void sy_lxor_word(uint8_t *dest,
    const uint8_t *w1, const uint8_t *w2);
static inline void sy_lxor_words(uint8_t *dest,
    const uint8_t *w1, const uint8_t *w2, size_t len);
static inline void sy_invert_word(uint8_t *dest, const uint8_t *w);
static inline void sy_invert_words(uint8_t *dest, const uint8_t *w,
    size_t len);
static inline void sy_rotl_word(uint8_t *dest, const uint8_t *src,
    size_t n);
static inline void sy_add_word(uint8_t *dest,
    const uint8_t *w1, const uint8_t *w2);
static inline void sy_copy_words(uint8_t *dest, const uint8_t *src,
    size_t len);
static inline void sy_copy_word(uint8_t *dest, const uint8_t *src);
static inline void sy_memzero_word(uint8_t *bytes);

#define W(ws,i)     ((ws)+(i)*4)

static inline uint32_t
sy_word_value(const uint8_t *bytes)
{
  return (uint32_t)(bytes[0] << 24 | bytes[1] << 16 |
      bytes[2] << 8 | bytes[3]);
}

static inline void
sy_set_word_value(uint8_t *dest, uint32_t v)
{
  dest[0] = (v >> 24) & 0xff;
  dest[1] = (v >> 16) & 0xff;
  dest[2] = (v >> 8) & 0xff;
  dest[3] = v & 0xff;
}

static inline void
sy_land_word(uint8_t *dest, const uint8_t *w1, const uint8_t *w2)
{
  dest[0] = w1[0] & w2[0];
  dest[1] = w1[1] & w2[1];
  dest[2] = w1[2] & w2[2];
  dest[3] = w1[3] & w2[3];
}

static inline void
sy_land_words(uint8_t *dest, const uint8_t *w1, const uint8_t *w2,
    size_t len)
{
  size_t i, j;

  for (i = 0; i < len; i++) {
    j = i * 4;
    sy_land_word(dest+j, w1+j, w2+j);
  }
}

static inline void
sy_lor_word(uint8_t *dest, const uint8_t *w1, const uint8_t *w2)
{
  dest[0] = w1[0] | w2[0];
  dest[1] = w1[1] | w2[1];
  dest[2] = w1[2] | w2[2];
  dest[3] = w1[3] | w2[3];
}

static inline void
sy_lor_words(uint8_t *dest, const uint8_t *w1, const uint8_t *w2,
    size_t len)
{
  size_t i, j;

  for (i = 0; i < len; i++) {
    j = i * 4;
    sy_lor_word(dest+j, w1+j, w2+j);
  }
}

static inline void
sy_lxor_word(uint8_t *dest, const uint8_t *w1, const uint8_t *w2)
{
  dest[0] = w1[0] ^ w2[0];
  dest[1] = w1[1] ^ w2[1];
  dest[2] = w1[2] ^ w2[2];
  dest[3] = w1[3] ^ w2[3];
}

static inline void
sy_lxor_words(uint8_t *dest, const uint8_t *w1, const uint8_t *w2,
    size_t len)
{
  size_t i, j;

  for (i = 0; i < len; i++) {
    j = i * 4;
    sy_lxor_word(dest+j, w1+j, w2+j);
  }
}

static inline void
sy_invert_word(uint8_t *dest, const uint8_t *w)
{
  dest[0] = ~(w[0]);
  dest[1] = ~(w[1]);
  dest[2] = ~(w[2]);
  dest[3] = ~(w[3]);
}

static inline void
sy_invert_words(uint8_t *dest, const uint8_t *w, size_t len)
{
  size_t i, j;

  for (i = 0; i < len; i++) {
    j = i * 4;
    sy_invert_word(dest+j, w+j);
  }
}

static inline void
sy_rotl_word(uint8_t *dest, const uint8_t *src, size_t n)
{
  uint32_t vsrc;

  vsrc = sy_word_value(src);
  vsrc = vsrc << n | (vsrc & (0xff << (8 - n))) >> (8 - n);
  sy_set_word_value(dest, vsrc);
}

static inline void
sy_add_word(uint8_t *dest, const uint8_t *w1, const uint8_t *w2)
{
  sy_set_word_value(dest, sy_word_value(w1) + sy_word_value(w2));
}

static inline void
sy_copy_words(uint8_t *dest, const uint8_t *src, size_t len)
{
  sy_memmove(dest, src, len*4);
}

/* Copy a word. This function is faster than sy_copy_words(). */
static inline void
sy_copy_word(uint8_t *dest, const uint8_t *src)
{
  if (src < dest && dest < src + 4) {
    dest[3] = src[3];
    dest[2] = src[2];
    dest[1] = src[1];
    dest[0] = src[0];
  } else {
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
    dest[3] = src[3];
  }
}

static inline void
sy_memzero_word(uint8_t *bytes)
{
  volatile uint8_t *vbytes;

  vbytes = (volatile uint8_t *)bytes;
  vbytes[0] = 0;
  vbytes[1] = 0;
  vbytes[2] = 0;
  vbytes[3] = 0;
}

#ifdef __cplusplus
}
#endif

#endif /* __SY_WORD_H__ */

