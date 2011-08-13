#ifndef __SY_WORD_H__
#define __SY_WORD_H__

#include <sayama/memory.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef uint32_t sy_word;

static inline void sy_encode_words(sy_word *words, const uint8_t *bytes,
    size_t len, uint8_t padding);
static inline void sy_decode_words(uint8_t *bytes, const sy_word *words,
    size_t len);
static inline bool sy_equal_words(const sy_word *words1,
    const sy_word *words2, size_t len);
static inline sy_word sy_rotl_word(sy_word word, size_t n);
static inline sy_word sy_rotr_word(sy_word word, size_t n);
static inline void sy_copy_words(sy_word *dest, const sy_word *src,
    size_t len);
static inline sy_word *sy_memset_words(sy_word *words, uint8_t v, size_t len);
static inline sy_word *sy_memzero_words(sy_word *words, size_t len);

static inline void
sy_encode_words(sy_word *words, const uint8_t *bytes,
    size_t len, uint8_t padding)
{
  size_t i;

  i = 0;
  if (len >= 4) {
    for (; i < len - 4; i += 4) {
      words[i/4] = bytes[i] << 24 | bytes[i+1] << 16 |
        bytes[i+2] << 8 | bytes[i+3];
    }
  }

  if (i < len) {
    words[i/4] = bytes[i] << 24 | padding << 16 | padding << 8 | padding;
    if (++i < len) {
      words[i/4] = (words[i/4] & 0xff00ffff) | bytes[i] << 16;
      if (++i < len) {
        words[i/4] = (words[i/4] & 0xffff00ff) | bytes[i] << 8;
        if (++i < len)
          words[i/4] = (words[i/4] & 0xffffff00) | bytes[i];
      }
    }
  }
}

static inline void
sy_decode_words(uint8_t *bytes, const sy_word *words, size_t len)
{
  sy_word w;
  size_t i;

  i = 0;
  if (len >= 4) {
    for (i = 0; i < len - 4; i += 4) {
      w = words[i/4];
      bytes[i] = (w >> 24) & 0xff;
      bytes[i+1] = (w >> 16) & 0xff;
      bytes[i+2] = (w >> 8) & 0xff;
      bytes[i+3] = w & 0xff;
    }
  }

  if (i < len) {
    bytes[i] = (words[i/4] >> 24) & 0xff;
    if (++i < len) {
      bytes[i] = (words[i/4] >> 16) & 0xff;
      if (++i < len) {
        bytes[i] = (words[i/4] >> 8) & 0xff;
        if (++i < len)
          bytes[i] = words[i/4] & 0xff;
      }
    }
  }
}

static inline bool
sy_equal_words(const sy_word *words1, const sy_word *words2, size_t len)
{
  size_t i;
  unsigned int n;

  for (i = 0; i < len; i++) {
    n = (3-i % 4) * 8;
    if (((words1[i/4] >> n) & 0xff) != ((words2[i/4] >> n) & 0xff))
      return false;
  }
  return true;
}

static inline sy_word
sy_rotl_word(sy_word word, size_t n)
{
  return word << n | (word & (0xffffffff << (32 - n))) >> (32 - n);
}

static inline sy_word
sy_rotr_word(sy_word word, size_t n)
{
  return word >> n | (word & (0xffffffff >> (32 - n))) << (32 - n);
}

static inline void
sy_copy_words(sy_word *dest, const sy_word *src, size_t len)
{
  if (len >= 4)
    sy_memmove(dest, src, len / 4 * 4);

  switch (len % 4) {
  case 0:
    break;

  case 1:
    dest[len/4] = (dest[len/4] & 0x00ffffff) | (src[len/4] & 0xff000000);
    break;

  case 2:
    dest[len/4] = (dest[len/4] & 0x0000ffff) | (src[len/4] & 0xffff0000);
    break;

  case 3:
    dest[len/4] = (dest[len/4] & 0x000000ff) | (src[len/4] & 0xffffff00);
    break;
  }
}

static inline sy_word *
sy_memset_words(sy_word *words, uint8_t v, size_t len)
{
  size_t i;
  sy_word vw;

  i = 0;
  if (len >= 4) {
    vw = v << 24 | v << 16 | v << 8 | v;
    for (i = 0; i < len - 4; i += 4)
      words[i/4] = vw;
  }

  if (i < len) {
    words[i/4] = (words[i/4] & 0x00ffffff) | v << 24;
    if (++i < len) {
      words[i/4] = (words[i/4] & 0xff00ffff) | v << 16;
      if (++i < len) {
        words[i/4] = (words[i/4] & 0xffff00ff) | v << 8;
        if (++i < len)
          words[i/4] = (words[i/4] & 0xffffff00) | v << 8;
      }
    }
  }
  return words;
}

static inline sy_word *
sy_memzero_words(sy_word *words, size_t len)
{
  return sy_memset_words(words, len, 0);
}

#ifdef __cplusplus
}
#endif

#endif /* __SY_WORD_H__ */

