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

struct sy_sha1_context {
  uint8_t state[SY_SHA1_STATE_LEN];
};

extern void sy_sha1_digest(uint8_t *state, const uint8_t *block);
extern void sy_sha1_init(sy_sha1_context *context);
extern void sy_sha1_update(sy_sha1_context *context, const uint8_t *bytes,
    size_t len);
extern void sy_sha1_final(sy_sha1_context *context, uint8_t *dest);
extern void sy_sha1_copy(sy_sha1_context *src, sy_sha1_context *dest);

#ifdef __cplusplus
}
#endif

#endif /* __SY_SHA_H__ */

