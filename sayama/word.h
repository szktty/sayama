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
    size_t from, size_t to);
static inline bool sy_equal_words(const sy_word *words1, size_t from1,
    const sy_word *words2, size_t from2, size_t len);
static inline sy_word sy_rotl_word(sy_word word, size_t n);
static inline sy_word sy_rotr_word(sy_word word, size_t n);
static inline void sy_copy_words(sy_word *dest, size_t from_dest,
    const sy_word *src, size_t from_src, size_t len);
static inline void sy_clear_words(sy_word *words,
    size_t from, size_t to);
static inline uint8_t sy_word_get(const sy_word *words, size_t i);
static inline void sy_word_set(sy_word *words, size_t i, uint8_t v);

extern void sy_fill_words(sy_word *words, uint8_t v,
    size_t from, size_t to);

#define SY_WORD_BYTE_SHIFT(i)   ((3-(i)%4)*8)
#define SY_WORD_BYTE_MASK(i)    (0xffU << SY_WORD_BYTE_SHIFT(i))

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
sy_decode_words(uint8_t *bytes, const sy_word *words,
    size_t from, size_t to)
{
  size_t i;

  for (i = from; i <= to; i++)
    bytes[i] = sy_word_get(words, i);
}

static inline bool
sy_equal_words(const sy_word *words1, size_t from1,
    const sy_word *words2, size_t from2, size_t len)
{
  size_t i;

  for (i = 0; i < len; i++) {
    if (sy_word_get(words1, from1 + i) != sy_word_get(words2, from2 + i))
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
sy_copy_words(sy_word *dest, size_t from_dest,
    const sy_word *src, size_t from_src, size_t len)
{
  size_t i;

  for (i = 0; i < len; i++)
    sy_word_set(dest, from_dest + i, sy_word_get(src, from_src + i));
}

static inline void
sy_clear_words(sy_word *words, size_t from, size_t to)
{
  return sy_fill_words(words, 0, from, to);
}

static inline uint8_t
sy_word_get(const sy_word *words, size_t i)
{
  return ((words[i/4] & SY_WORD_BYTE_MASK(i)) >>
      SY_WORD_BYTE_SHIFT(i)) & 0xffU;
}

static inline void
sy_word_set(sy_word *words, size_t i, uint8_t v)
{
  words[i/4] = (words[i/4] & ~SY_WORD_BYTE_MASK(i)) |
    (v << SY_WORD_BYTE_SHIFT(i));
}

#ifdef __cplusplus
}
#endif

#endif /* __SY_WORD_H__ */

