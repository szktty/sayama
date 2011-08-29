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

static inline void sy_encode_dwords(sy_dword *dest, const uint8_t *src,
    size_t len);
static inline void sy_decode_dwords(uint8_t *dest, const sy_dword *src,
    size_t len);

static inline bool sy_equal_dwords(const sy_dword *words1,
    const sy_dword *words2, size_t len);
static inline void sy_copy_dwords(sy_dword *dest,
    const sy_dword *src, size_t len);
static inline void sy_clear_dwords(volatile sy_dword *words, size_t len);

static inline sy_dword sy_rotr_dword(sy_dword w, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* __SY_DWORD_H__ */

