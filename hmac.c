#include <string.h>
#include "sayama/bytes.h"
#include "sayama/hmac.h"
#include "sayama/memory.h"
#include "sayama/sha.h"
#include "sayama/utils.h"

void
sy_hmac(uint8_t *dest, const uint8_t *key, size_t key_len,
    const uint8_t *msg, size_t msg_len,
    const sy_digester *digester)
{
  sy_hmac_context context;

  sy_hmac_init(&context, key, key_len, digester);
  sy_hmac_update(&context, msg, msg_len);
  sy_hmac_final(&context, dest);
}

void
sy_hmac_init(sy_hmac_context *context, const uint8_t *key,
  size_t key_len, const sy_digester *digester)
{
  size_t blen;
  uint8_t ipad_key[SY_HMAC_BLOCK_LEN];

  context->digester = digester;

  /* padding key */
  sy_memzero(context->key, SY_HMAC_BLOCK_LEN);
  sy_memmove(context->key, key, key_len);

  /* inner pad */
  blen = digester->block_len;
  sy_memset(ipad_key, 0x36, blen);
  sy_lxor_bytes(ipad_key, context->key, ipad_key, blen);
  sy_digest_init(&context->inner, digester);
  sy_digest_update(&context->inner, ipad_key, blen);

  sy_memzero(ipad_key, SY_HMAC_BLOCK_LEN);
}

void
sy_hmac_update(sy_hmac_context *context, const uint8_t *bytes,
    size_t len)
{
  sy_digest_update(&context->inner, bytes, len);
}

void
sy_hmac_final(sy_hmac_context *context, uint8_t *dest)
{
  size_t blen;
  uint8_t inner[SY_HMAC_BLOCK_LEN];
  uint8_t opad_key[SY_HMAC_BLOCK_LEN];
  sy_digest_context outer;

  /* inner */
  blen = context->digester->block_len;
  sy_digest_final(&context->inner, inner);

  /* outer pad */
  sy_memset(opad_key, 0x5c, blen);
  sy_lxor_bytes(opad_key, context->key, opad_key, blen);

  /* outer */
  sy_digest_init(&outer, context->digester);
  sy_digest_update(&outer, opad_key, blen);
  sy_digest_update(&outer, inner, context->digester->digest_len);
  sy_digest_final(&outer, dest);

  sy_memzero(inner, SY_HMAC_BLOCK_LEN);
  sy_memzero(opad_key, SY_HMAC_BLOCK_LEN);
  sy_memzero(context->key, SY_HMAC_BLOCK_LEN);
}

void
sy_hmac_copy(sy_hmac_context *dest, const sy_hmac_context *src)
{
  dest->digester = src->digester;
  memcpy(dest->key, src->key, SY_HMAC_BLOCK_LEN);
  sy_digest_copy(&dest->inner, &src->inner);
}

const sy_digester *
sy_hmac_digester(const sy_hmac_context *context)
{
  return context->digester;
}

size_t
sy_hmac_digest_block_len(const sy_hmac_context *context)
{
  return context->digester->block_len;
}

size_t
sy_hmac_digest_len(const sy_hmac_context *context)
{
  return context->digester->digest_len;
}

