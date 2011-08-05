#include "sayama/memory.h"
#include "sayama/sha.h"
#include "sayama/utils.h"
#include "sayama/word.h"
#include "sha.h"

static inline void sha1_hash_block(uint8_t *state, const uint8_t *block);
static inline void sha1_f(uint8_t *dest, unsigned int t,
    const uint8_t *b, const uint8_t *c, const uint8_t *d);

static uint8_t sha1_K[][4] = {
  {0, 0, 0, 0x5a}, {0, 0, 0, 0x82}, {0, 0, 0, 0x79}, {0, 0, 0, 0x99},
  {0, 0, 0, 0x6e}, {0, 0, 0, 0xd9}, {0, 0, 0, 0xeb}, {0, 0, 0, 0xa1},
  {0, 0, 0, 0x8f}, {0, 0, 0, 0x1b}, {0, 0, 0, 0xbc}, {0, 0, 0, 0xdc},
  {0, 0, 0, 0xca}, {0, 0, 0, 0x62}, {0, 0, 0, 0xc1}, {0, 0, 0, 0xd6}
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
    sha1_hash_block(context->state, block);
  }

  sy_memzero(block, 64);
}

static inline void
sha1_hash_block(uint8_t *state, const uint8_t *block)
{
  uint8_t w[80*4], tmp[4], a[4], b[4], c[4], d[4], e[4];
  unsigned int t;

  sy_copy_words(w, block, 16);
  for (t = 16; t < 80; t++) {
    sy_lxor_word(tmp, W(w, t-3), W(w, t-8));
    sy_lxor_word(tmp, tmp, W(w, t-14));
    sy_lxor_word(tmp, tmp, W(w, t-16));
    sy_rotl_word(W(w, t), tmp, 1);
  }

  sy_copy_word(a, state);
  sy_copy_word(b, W(state, 1));
  sy_copy_word(c, W(state, 2));
  sy_copy_word(d, W(state, 3));
  sy_copy_word(e, W(state, 4));

  for (t = 0; t < 80; t++) {
    sy_rotl_word(tmp, a, 5);
    sha1_f(tmp, t, b, c, d);
    sy_add_word(tmp, tmp, e);
    sy_add_word(tmp, tmp, W(w, t));
    sy_add_word(tmp, tmp, sha1_K[t/20]);
    sy_copy_word(e, d);
    sy_copy_word(d, c);
    sy_rotl_word(c, b, 30);
    sy_copy_word(b, a);
    sy_copy_word(a, tmp);
  }

  sy_add_word(state, state, a);
  sy_add_word(W(state, 1), W(state, 1), b);
  sy_add_word(W(state, 2), W(state, 2), c);
  sy_add_word(W(state, 3), W(state, 3), d);
  sy_add_word(W(state, 4), W(state, 4), e);

  sy_memzero(w, 80*4);
  sy_memzero_word(tmp);
  sy_memzero_word(a);
  sy_memzero_word(b);
  sy_memzero_word(c);
  sy_memzero_word(d);
  sy_memzero_word(e);
}

void
_sy_sha1_hash_block(uint8_t *state, const uint8_t *block)
{
  sha1_hash_block(state, block);
}

static inline void
sha1_f(uint8_t *dest, unsigned int t,
    const uint8_t *b, const uint8_t *c, const uint8_t *d)
{
  uint8_t tmp1[4], tmp2[4], tmp3[4];

  if (t < 20) {
    /* b & c | ~b & d */
    sy_land_word(tmp1, b, c);
    sy_invert_word(tmp2, b);
    sy_land_word(tmp2, tmp2, d);
    sy_lor_word(dest, tmp1, tmp2);
  } else if (40 <= t && t < 60) {
    /* b & c | b & d | c & d */
    sy_land_word(tmp1, b, c);
    sy_land_word(tmp2, b, d);
    sy_land_word(tmp3, c, d);
    sy_lor_word(dest, tmp1, tmp2);
    sy_lor_word(dest, dest, tmp3);
  } else {
    /* b ^ c ^ d */
    sy_lxor_word(dest, b, c);
    sy_lxor_word(dest, dest, d);
  }

  sy_memzero_word(tmp1);
  sy_memzero_word(tmp2);
  sy_memzero_word(tmp3);
}

void
_sy_sha1_f(uint8_t *dest, unsigned int t,
    const uint8_t *b, const uint8_t *c, const uint8_t *d)
{
  sha1_f(dest, t, b, c, d);
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

