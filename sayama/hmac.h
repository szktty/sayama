#ifndef __SY_HMAC_H__
#define __SY_HMAC_H__

#include <sayama/digest.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct sy_hmac_context  sy_hmac_context;

#define SY_HMAC_BLOCK_LEN       64

struct sy_hmac_context {
  const sy_digester *digester;
  uint8_t key[SY_HMAC_BLOCK_LEN];
  sy_digest_context inner;
};

extern void sy_hmac(uint8_t *dest, const uint8_t *key,
    size_t key_len, const uint8_t *msg, size_t msg_len,
    const sy_digester *digester);
extern void sy_hmac_init(sy_hmac_context *context, const uint8_t *key,
    size_t key_len, const sy_digester *digester);
extern void sy_hmac_update(sy_hmac_context *context, const uint8_t *bytes,
    size_t len);
extern void sy_hmac_final(sy_hmac_context *context, uint8_t *dest);
extern void sy_hmac_copy(sy_hmac_context *dest,
    const sy_hmac_context *src);
extern const sy_digester *sy_hmac_digester(
    const sy_hmac_context *context);
extern size_t sy_hmac_digest_block_len(const sy_hmac_context *context);
extern size_t sy_hmac_digest_len(const sy_hmac_context *context);

#ifdef __cplusplus
}
#endif

#endif /* __SY_HMAC_H__ */

