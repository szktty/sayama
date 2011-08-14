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
static inline void sy_clear_words(sy_word *words,
    size_t from, size_t to);

extern void sy_fill_words(sy_word *words, uint8_t v,
    size_t from, size_t to);

static inline void
sy_encode_words(sy_word *words, const uint8_t *bytes,
    size_t len, uint8_t padding)
{
  size_t i;

  if (len >= 4) {
    for (i = 0; i + 3 < len; i += 4) {
      words[i/4] = bytes[i] << 24 | bytes[i+1] << 16 |
        bytes[i+2] << 8 | bytes[i+3];
    }
  }

  switch (len % 4) {
  case 0:
    break;

  case 1:
    words[len/4] = bytes[len/4*4] << 24 | padding << 16 |
      padding << 8 | padding;
    break;

  case 2:
    words[len/4] = bytes[len/4*4] << 24 | bytes[len/4*4+1] << 16 |
      padding << 8 | padding;
    break;

  case 3:
    words[len/4] = bytes[len/4*4] << 24 | bytes[len/4*4+1] << 16 |
      bytes[len/4*4+2] << 8 | padding;
    break;
  }
}

static inline void
sy_decode_words(uint8_t *bytes, const sy_word *words, size_t len)
{
  sy_word w;
  size_t i;

  if (len >= 4) {
    for (i = 0; i + 3 < len; i += 4) {
      w = words[i/4];
      bytes[i] = (w >> 24) & 0xffU;
      bytes[i+1] = (w >> 16) & 0xffU;
      bytes[i+2] = (w >> 8) & 0xffU;
      bytes[i+3] = w & 0xffU;
    }
  }

  switch (len % 4) {
  case 0:
    break;

  case 3:
    bytes[len/4*4+2] = (words[len/4] >> 8) & 0xffU;

  case 2:
    bytes[len/4*4+1] = (words[len/4] >> 16) & 0xffU;

  case 1:
    bytes[len/4*4] = (words[len/4] >> 24) & 0xffU;
    break;
  }
}

static inline bool
sy_equal_words(const sy_word *words1, const sy_word *words2, size_t len)
{
  size_t i;
  unsigned int n;

  for (i = 0; i < len; i++) {
    n = (3-i % 4) * 8;
    if (((words1[i/4] >> n) & 0xffU) != ((words2[i/4] >> n) & 0xffU))
      return false;
  }
  return true;
}

static inline sy_word
sy_rotl_word(sy_word word, size_t n)
{
  return word << n | (word & (0xffffffffU << (32 - n))) >> (32 - n);
}

static inline sy_word
sy_rotr_word(sy_word word, size_t n)
{
  return word >> n | (word & (0xffffffffU >> (32 - n))) << (32 - n);
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
    dest[len/4] = (dest[len/4] & 0x00ffffffU) | (src[len/4] & 0xff000000U);
    break;

  case 2:
    dest[len/4] = (dest[len/4] & 0x0000ffffU) | (src[len/4] & 0xffff0000U);
    break;

  case 3:
    dest[len/4] = (dest[len/4] & 0x000000ffU) | (src[len/4] & 0xffffff00U);
    break;
  }
}

static inline void
sy_clear_words(sy_word *words, size_t from, size_t to)
{
  return sy_fill_words(words, 0, from, to);
}

#ifdef __cplusplus
}
#endif

#endif /* __SY_WORD_H__ */

