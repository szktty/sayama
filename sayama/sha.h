#ifndef __SY_SHA_H__
#define __SY_SHA_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct sy_sha1_context  sy_sha1_context;

#define SY_SHA1_STATE_LEN       20
#define SY_SHA1_BLOCK_LEN       64

struct sy_sha1_context {
  uint8_t state[SY_SHA1_STATE_LEN];
  uint8_t buf[SY_SHA1_BLOCK_LEN];
  size_t buf_len;
  size_t total_len;
};

extern void sy_sha1_digest(uint8_t *digest, const uint8_t *bytes);
extern void sy_sha1_init(sy_sha1_context *context);
extern void sy_sha1_update(sy_sha1_context *context, const uint8_t *bytes,
    size_t len);
extern void sy_sha1_final(sy_sha1_context *context, uint8_t *dest);

#ifdef __cplusplus
}
#endif

#endif /* __SY_SHA_H__ */

