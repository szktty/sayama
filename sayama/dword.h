#ifndef __SY_DWORD_H__
#define __SY_DWORD_H__

#include <sayama/bytes.h>
#include <sayama/memory.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef uint64_t sy_dword;

static inline uint8_t sy_dword_get(sy_dword w);
static inline uint8_t sy_dwords_get(const sy_dword *words);
static inline sy_dword sy_dword_set(sy_dword w, uint8_t i, uint8_t v);
static inline void sy_dwords_set(const sy_dword *words, uint8_t i, uint8_t v);

static inline sy_dword sy_create_dword(uint8_t b0, uint8_t b1,
    uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7);
static inline void sy_encode_dwords(sy_dword *dest, const uint8_t *src,
    size_t dwlen);
static inline void sy_decode_dwords(uint8_t *dest, const sy_dword *src,
    size_t len);

static inline bool sy_equal_dwords(const sy_dword *words1,
    const sy_dword *words2, size_t len);
static inline void sy_copy_dwords(sy_dword *dest,
    const sy_dword *src, size_t len);
static inline void sy_clear_dwords(volatile sy_dword *words, size_t len);

static inline sy_dword sy_rotr_dword(sy_dword w, size_t n);

static inline sy_dword
sy_create_dword(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3,
    uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7)
{
  return ((sy_dword)b0 << 56) | ((sy_dword)b1 << 48) |
    ((sy_dword)b2 << 40) | ((sy_dword)b3 << 32) |
    ((sy_dword)b4 << 24) | ((sy_dword)b5 << 16) |
    ((sy_dword)b6 << 8) | (sy_dword)b7;
}

static inline void
sy_encode_dwords(sy_dword *dwords, const uint8_t *bytes, size_t dwlen)
{
  size_t i, j;

  for (i = 0; i < dwlen; i++) {
    j = i * 8;
    dwords[i] = sy_create_dword(bytes[j], bytes[j+1], bytes[j+2],
        bytes[j+3], bytes[j+4], bytes[j+5], bytes[j+6], bytes[j+7]);
  }
}

#ifdef __cplusplus
}
#endif

#endif /* __SY_DWORD_H__ */

