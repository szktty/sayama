#ifndef __SY_WORD_H__
#define __SY_WORD_H__

#include <sayama/bytes.h>
#include <sayama/memory.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef uint32_t sy_word;

static inline sy_word sy_create_word(uint8_t b0, uint8_t b1,
    uint8_t b2, uint8_t b3);
static inline void sy_encode_words(sy_word *words,
    const uint8_t *bytes, size_t wlen);
static inline void sy_decode_words(uint8_t *bytes, const sy_word *words,
    size_t wlen);
static inline bool sy_equal_words(const sy_word *words1, size_t from1,
    const sy_word *words2, size_t from2, size_t len);
static inline sy_word sy_rotl_word(sy_word word, size_t n);
static inline sy_word sy_rotr_word(sy_word word, size_t n);
static inline void sy_copy_words(sy_word *dest, size_t from_dest,
    const sy_word *src, size_t from_src, size_t to_src);
static inline void sy_clear_words(volatile sy_word *words, size_t wlen);
static inline uint8_t sy_word_get0(sy_word w);
static inline uint8_t sy_word_get1(sy_word w);
static inline uint8_t sy_word_get2(sy_word w);
static inline uint8_t sy_word_get3(sy_word w);
static inline uint8_t sy_word_get(const sy_word *words, size_t i);
static inline void sy_word_set(sy_word *words, size_t i, uint8_t v);
static inline void sy_hexify_words(char *buf, const sy_word *words,
    size_t from, size_t to);

extern void sy_fill_words(sy_word *words, uint8_t v,
    size_t from, size_t to);

#define SY_WORD_BYTE_SHIFT(i)   ((3-(i)%4)*8)
#define SY_WORD_BYTE_MASK(i)    (0xffU << SY_WORD_BYTE_SHIFT(i))

static inline sy_word
sy_create_word(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3)
{
  return (b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
}

static inline void
sy_encode_words(sy_word *words, const uint8_t *bytes, size_t wlen)
{
  size_t i;

  for (i = 0; i < wlen; i++)
    words[i] = sy_create_word(bytes[i*4], bytes[i*4+1],
        bytes[i*4+2], bytes[i*4+3]);
}

static inline void
sy_decode_words(uint8_t *bytes, const sy_word *words, size_t wlen)
{
  size_t i;
  sy_word w;

  for (i = 0; i < wlen; i++) {
    w = words[i];
    bytes[i*4] = sy_word_get0(w);
    bytes[i*4+1] = sy_word_get1(w);
    bytes[i*4+2] = sy_word_get2(w);
    bytes[i*4+3] = sy_word_get3(w);
  }
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
    const sy_word *src, size_t from_src, size_t to_src)
{
  size_t i;

  for (i = from_src; i <= to_src; i++)
    sy_word_set(dest, from_dest + i - from_src, sy_word_get(src, i));
}

static inline void
sy_clear_words(volatile sy_word *words, size_t wlen)
{
  size_t i;

  for (i = 0; i < wlen; i++)
    words[i] = 0;
}

static inline uint8_t
sy_word_get(const sy_word *words, size_t i)
{
  return ((words[i/4] & SY_WORD_BYTE_MASK(i)) >>
      SY_WORD_BYTE_SHIFT(i)) & 0xffU;
}

static inline uint8_t
sy_word_get0(sy_word w)
{
  return (w >> 24) & 0xffU;
}

static inline uint8_t
sy_word_get1(sy_word w)
{
  return (w >> 16) & 0xffU;
}

static inline uint8_t
sy_word_get2(sy_word w)
{
  return (w >> 8) & 0xffU;
}

static inline uint8_t
sy_word_get3(sy_word w)
{
  return w & 0xffU;
}

static inline void
sy_word_set(sy_word *words, size_t i, uint8_t v)
{
  words[i/4] = (words[i/4] & ~SY_WORD_BYTE_MASK(i)) |
    (v << SY_WORD_BYTE_SHIFT(i));
}

static inline void
sy_hexify_words(char *buf, const sy_word *words, size_t from, size_t to)
{
  size_t i;

  for (i = 0; i <= to - from; i++)
    sy_hexify_byte(buf+i*2, sy_word_get(words, i+from));
}

#ifdef __cplusplus
}
#endif

#endif /* __SY_WORD_H__ */

