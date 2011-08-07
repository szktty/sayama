#ifndef __SY_HMAC_H__
#define __SY_HMAC_H__

#include <sayama/sha.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct sy_hmac_context      sy_hmac_context;
typedef struct sy_hmac_digester     sy_hmac_digester;

struct sy_hmac_context {
  sy_hmac_digester *digester;
  union {
    sy_sha1_context sha1;
    void *p; /* custom digester */
  } digest_context;
};

struct sy_hmac_digester {
  void (*init)(sy_hmac_context *context);
  void (*update)(sy_hmac_context *context, const uint8_t *bytes,
      size_t len);
  void (*final)(sy_hmac_context *context, uint8_t *digest);
  size_t block_len;
};

extern sy_hmac_digester *sy_hmac_sha1_digester;

extern void sy_hmac_digest(uint8_t *digest, const uint8_t *bytes,
    sy_hmac_digester *digester);
extern void sy_hmac_init(sy_hmac_context *context,
    sy_hmac_digester *digester);
extern void sy_hmac_update(sy_hmac_context *context, const uint8_t *bytes,
    size_t len);
extern void sy_hmac_final(sy_hmac_context *context, uint8_t *dest);
extern void sy_hmac_copy(sy_hmac_context *src, sy_hmac_context *dest);
extern size_t sy_hmac_block_len(sy_hmac_context *context);

#ifdef __cplusplus
}
#endif

#endif /* __SY_HMAC_H__ */

