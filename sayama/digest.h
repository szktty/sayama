#ifndef __SY_DIGEST_H__
#define __SY_DIGEST_H__

#include <sayama/sha.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct sy_digest_context    sy_digest_context;
typedef struct sy_digester          sy_digester;

struct sy_digest_context {
  const sy_digester *digester;
  union {
    sy_sha1_context sha1;
    void *p; /* custom digester */
  } context;
};

struct sy_digester {
  void (*init)(sy_digest_context *context);
  void (*update)(sy_digest_context *context, const uint8_t *bytes,
      size_t len);
  void (*final)(sy_digest_context *context, uint8_t *dest);
  void (*copy)(sy_digest_context *dest, const sy_digest_context *src);
  size_t block_len;
  size_t digest_len;
};

extern const sy_digester sy_digester_sha1;

extern void sy_digest_digest(uint8_t *dest, const uint8_t *bytes,
    size_t len, const sy_digester *digester);
extern void sy_digest_init(sy_digest_context *context,
    const sy_digester *digester);
extern void sy_digest_update(sy_digest_context *context,
    const uint8_t *bytes, size_t len);
extern void sy_digest_final(sy_digest_context *context, uint8_t *dest);
extern void sy_digest_copy(sy_digest_context *dest,
    const sy_digest_context *src);
extern size_t sy_digest_block_len(const sy_digest_context *context);
extern size_t sy_digest_digest_len(const sy_digest_context *context);

#ifdef __cplusplus
}
#endif

#endif /* __SY_DIGEST_H__ */

