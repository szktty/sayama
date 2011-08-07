#ifndef __SY_BYTES_H__
#define __SY_BYTES_H__

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

static inline void sy_lxor_bytes(uint8_t *dest,
    const uint8_t *bytes1, const uint8_t *bytes2, size_t len);

static inline void
sy_lxor_bytes(uint8_t *dest, const uint8_t *bytes1, const uint8_t *bytes2,
    size_t len)
{
  size_t i;

  for (i = 0; i < len; i++)
    dest[i] = bytes1[i] ^ bytes2[i];
}

#ifdef __cplusplus
}
#endif

#endif /* __SY_BYTES_H__ */

