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
static inline void sy_hexify_bytes(char *buf, const uint8_t *bytes,
    size_t from, size_t to);
static inline void sy_hexify_byte(char *buf, uint8_t byte);

static inline void
sy_lxor_bytes(uint8_t *dest, const uint8_t *bytes1, const uint8_t *bytes2,
    size_t len)
{
  size_t i;

  for (i = 0; i < len; i++)
    dest[i] = bytes1[i] ^ bytes2[i];
}

static inline void
sy_hexify_bytes(char *buf, const uint8_t *bytes, size_t from, size_t to)
{
  size_t i;

  for (i = 0; i <= to - from; i++)
    sy_hexify_byte(buf+i*2, bytes[i+from]);
}

static inline void
sy_hexify_byte(char *buf, uint8_t byte)
{
  const char hex[] = "0123456789abcdef";

  buf[0] = hex[(byte >> 4) & 0x0fU];
  buf[1] = hex[byte & 0x0fU];
}

#ifdef __cplusplus
}
#endif

#endif /* __SY_BYTES_H__ */

