#include "sayama/bytes.h"
#include "sayama/hmac.h"
#include "sayama/memory.h"
#include "sayama/sha.h"

static void hmac_sha1_init(sy_hmac_context *context);
static void hmac_sha1_update(sy_hmac_context *context, const uint8_t *bytes,
    size_t len);
static void hmac_sha1_final(sy_hmac_context *context, uint8_t *dest);

const sy_hmac_digester sy_hmac_sha1_digester = {
  hmac_sha1_init,
  hmac_sha1_update,
  hmac_sha1_final,
  20
};

static void
hmac_sha1_init(sy_hmac_context *context)
{
  sy_sha1_init(&context->digest_context.sha1);
}

static void
hmac_sha1_update(sy_hmac_context *context, const uint8_t *bytes,
    size_t len)
{
  sy_sha1_update(&context->digest_context.sha1, bytes, len);
}

static void
hmac_sha1_final(sy_hmac_context *context, uint8_t *dest)
{
  sy_sha1_final(&context->digest_context.sha1, dest);
}

void
sy_hmac_digest(uint8_t *dest, const uint8_t *key, size_t key_len,
    const uint8_t *msg, size_t msg_len,
    const sy_hmac_digester *digester)
{
  sy_hmac_context context;

  sy_hmac_init(&context, key, key_len, digester);
  sy_hmac_update(&context, msg, msg_len);
  sy_hmac_final(&context, dest);
}

void
sy_hmac_init(sy_hmac_context *context, const uint8_t *key,
  size_t key_len, const sy_hmac_digester *digester)
{
  size_t blen;

  context->digester = digester;
  sy_memmove(context->key, key, key_len);
  context->digester->init(context);
  blen = digester->block_len;

  /* inner pad: 00110110 */
  sy_memset(context->ipad, 0x36, blen);

  /* outer pad: 01011100 */
  sy_memset(context->opad, 0x5c, blen);

  /* xor */
  sy_lxor_bytes(context->ipad, context->key, context->ipad, blen);
  sy_lxor_bytes(context->opad, context->key, context->opad, blen);

  context->digester->update(context, context->ipad, blen);
}

void
sy_hmac_update(sy_hmac_context *context, const uint8_t *bytes,
    size_t len)
{
  context->digester->update(context, bytes, len);
}

void
sy_hmac_final(sy_hmac_context *context, uint8_t *dest)
{
  uint8_t inner[SY_HMAC_STATE_LEN];
  size_t blen;

  blen = context->digester->block_len;
  context->digester->final(context, inner);
  context->digester->init(context);
  context->digester->update(context, context->opad, blen);
  context->digester->update(context, inner, blen);
  context->digester->final(context, dest);

  sy_memzero(context->key, SY_HMAC_STATE_LEN);
  sy_memzero(context->ipad, SY_HMAC_STATE_LEN);
  sy_memzero(context->opad, SY_HMAC_STATE_LEN);
}

size_t
sy_hmac_block_len(sy_hmac_context *context)
{
  return context->digester->block_len;
}

