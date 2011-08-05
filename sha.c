#include "sayama/memory.h"
#include "sayama/sha.h"
#include "sayama/utils.h"
#include "sayama/word.h"
#include "sha.h"

static uint8_t sha1_K[] = {
  0x5a, 0x82, 0x79, 0x99,
  0x6e, 0xd9, 0xeb, 0xa1,
  0x8f, 0x1b, 0xbc, 0xdc,
  0xca, 0x62, 0xc1, 0xd6
};

void
sy_sha1_init(sy_sha1_context *context)
{
  static uint8_t init[] = { 0x67, 0x45, 0x23, 0x01, 0xef, 0xcd, 0xab,
    0x89, 0x98, 0xba, 0xdc, 0xfe, 0x10, 0x32, 0x54, 0x76, 0xc3, 0xd2,
    0xe1, 0xf0 };

  sy_memmove(context->state, init, SY_SHA1_STATE_LEN);
}

void
sy_sha1_update(sy_sha1_context *context, const uint8_t *bytes,
    size_t len)
{
  uint8_t block[64];
  size_t i;
  uint64_t mlen;

  while (len > 0) {
    if (len >= 56) {
      sy_memmove(block, bytes, 56);
      len -= 56;
    } else {
      /* padding */
      sy_memmove(block, bytes, len);
      block[len] = 128; /* 0b10000000 */
      for (i = len + 1; i < 56; i++)
        block[i] = 0;
      len = 0;
    }

    /* message length (bits) */
    mlen = len * 8;
    block[56] = (mlen >> 56) & 0xff;
    block[57] = (mlen >> 48) & 0xff;
    block[58] = (mlen >> 40) & 0xff;
    block[59] = (mlen >> 32) & 0xff;
    block[60] = (mlen >> 24) & 0xff;
    block[61] = (mlen >> 16) & 0xff;
    block[62] = (mlen >> 8) & 0xff;
    block[63] = mlen & 0xff;

    /* hash */
    printf("hash:\n");
    sy_print_bytes(block, 64, SY_PRINT_SP);
    _sy_sha1_hash_block(context->state, block);
  }

  sy_memzero(block, 64);
}

void
sy_sha1_final(sy_sha1_context *context, uint8_t *dest)
{
  sy_memmove(dest, context->state, SY_SHA1_STATE_LEN);
}

void
sy_sha1_copy(sy_sha1_context *src, sy_sha1_context *dest)
{
  sy_memmove(dest->state, src->state, SY_SHA1_STATE_LEN);
}


