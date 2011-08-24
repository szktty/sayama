#ifndef __SY_SHA_H__
#define __SY_SHA_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sayama/word.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct sy_sha1_context      sy_sha1_context;
typedef struct sy_sha256_context    sy_sha256_context;

#define SY_SHA1_STATE_LEN       20
#define SY_SHA1_STATE_WLEN      (SY_SHA1_STATE_LEN/4)
#define SY_SHA1_BLOCK_LEN       64
#define SY_SHA256_STATE_LEN     32
#define SY_SHA256_BLOCK_LEN     64

struct sy_sha1_context {
  sy_word state[SY_SHA1_STATE_WLEN];
  uint8_t buf[SY_SHA1_BLOCK_LEN];
  size_t buf_len;
  size_t total_len;
};

struct sy_sha256_context {
  sy_word state[SY_SHA256_STATE_LEN/4];
  uint8_t buf[SY_SHA256_BLOCK_LEN];
  size_t buf_len;
  size_t total_len;
};

extern void sy_sha1(uint8_t *buf, const uint8_t *data, size_t len);
extern void sy_sha1_init(sy_sha1_context *context);
extern void sy_sha1_update(sy_sha1_context *context, const uint8_t *bytes,
    size_t len);
extern void sy_sha1_final(sy_sha1_context *context, uint8_t *dest);
extern void sy_sha1_copy(sy_sha1_context *dest, const sy_sha1_context *src);

extern void sy_sha256(uint8_t *buf, const uint8_t *data, size_t len);
extern void sy_sha256_init(sy_sha256_context *context);
extern void sy_sha256_update(sy_sha256_context *context,
    const uint8_t *data, size_t len);
extern void sy_sha256_final(sy_sha256_context *context, uint8_t *buf);

#ifdef __cplusplus
}
#endif

#endif /* __SY_SHA_H__ */

