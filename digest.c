#include "sayama/digest.h"
#include "sayama/utils.h"

static void digest_sha1_init(sy_digest_context *context);
static void digest_sha1_update(sy_digest_context *context,
    const uint8_t *bytes, size_t len);
static void digest_sha1_final(sy_digest_context *context, uint8_t *dest);
static void digest_sha1_copy(sy_digest_context *dest,
    const sy_digest_context *src);

const sy_digester sy_digester_sha1 = {
  digest_sha1_init,
  digest_sha1_update,
  digest_sha1_final,
  digest_sha1_copy,
  64,
  20
};

static void
digest_sha1_init(sy_digest_context *context)
{
  sy_sha1_init(&context->context.sha1);
}

static void
digest_sha1_update(sy_digest_context *context, const uint8_t *bytes,
    size_t len)
{
  sy_sha1_update(&context->context.sha1, bytes, len);
}

static void
digest_sha1_final(sy_digest_context *context, uint8_t *dest)
{
  sy_sha1_final(&context->context.sha1, dest);
}

static void
digest_sha1_copy(sy_digest_context *dest, const sy_digest_context *src)
{
  sy_sha1_copy(&dest->context.sha1, &src->context.sha1);
}

void
sy_digest(uint8_t *dest, const uint8_t *bytes,
    size_t len, const sy_digester *digester)
{
  sy_digest_context context;

  sy_digest_init(&context, digester);
  sy_digest_update(&context, bytes, len);
  sy_digest_final(&context, dest);
}

void
sy_digest_init(sy_digest_context *context, const sy_digester *digester)
{
  context->digester = digester;
  digester->init(context);
}

void
sy_digest_update(sy_digest_context *context, const uint8_t *bytes,
    size_t len)
{
  context->digester->update(context, bytes, len);
}

void
sy_digest_final(sy_digest_context *context, uint8_t *dest)
{
  context->digester->final(context, dest);
}

void
sy_digest_copy(sy_digest_context *dest, const sy_digest_context *src)
{
  dest->digester = src->digester;
  src->digester->copy(dest, src);
}

size_t
sy_digest_block_len(const sy_digest_context *context)
{
  return context->digester->block_len;
}

size_t
sy_digest_len(const sy_digest_context *context)
{
  return context->digester->digest_len;
}

