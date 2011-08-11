#include "sayama/memory.h"
#include "sayama/sha.h"
#include "sayama/utils.h"
#include "sayama/word.h"
#include "sha.h"

static void sha1_update(sy_sha1_context *context);
static inline void sha1_hash_block(uint8_t *state, const uint8_t *block);
static inline void sha1_f(uint8_t *dest, unsigned int t,
    const uint8_t *b, const uint8_t *c, const uint8_t *d);

static inline void sha256_sigma0(uint8_t *buf, const uint8_t *w);
static inline void sha256_sigma1(uint8_t *buf, const uint8_t *w);
static inline void sha256_ch(uint8_t *buf, const uint8_t *w1,
    const uint8_t *w2, const uint8_t *w3);
static inline void sha256_maj(uint8_t *buf, const uint8_t *w1,
    const uint8_t *w2, const uint8_t *w3);
static inline void sha256_sum0(uint8_t *buf, const uint8_t *w);
static inline void sha256_sum1(uint8_t *buf, const uint8_t *w);

static uint8_t sha1_K[][4] = {
  {0x5a, 0x82, 0x79, 0x99},
  {0x6e, 0xd9, 0xeb, 0xa1},
  {0x8f, 0x1b, 0xbc, 0xdc},
  {0xca, 0x62, 0xc1, 0xd6}
};

void
sy_sha1(uint8_t *buf, const uint8_t *data, size_t len)
{
  sy_sha1_context context;

  sy_sha1_init(&context);
  sy_sha1_update(&context, data, len);
  sy_sha1_final(&context, buf);
}

void
sy_sha1_init(sy_sha1_context *context)
{
  static uint8_t init[] = { 0x67, 0x45, 0x23, 0x01, 0xef, 0xcd, 0xab,
    0x89, 0x98, 0xba, 0xdc, 0xfe, 0x10, 0x32, 0x54, 0x76, 0xc3, 0xd2,
    0xe1, 0xf0 };

  sy_memmove(context->state, init, SY_SHA1_STATE_LEN);
  sy_memzero(context->buf, SY_SHA1_BLOCK_LEN);
  context->buf_len = 0;
  context->total_len = 0;
}

void
sy_sha1_update(sy_sha1_context *context, const uint8_t *bytes,
    size_t len)
{
  size_t copy_len, rest_len;

  context->total_len += len;

  /* copy bytes to buffer */
  if (context->buf_len + len > SY_SHA1_BLOCK_LEN)
    copy_len = SY_SHA1_BLOCK_LEN - context->buf_len;
  else
    copy_len = len;

  if (copy_len > 0) {
    sy_memmove(context->buf + context->buf_len, bytes, copy_len);
    context->buf_len += copy_len;
  }

  /* update state with buffer */
  if (context->buf_len >= SY_SHA1_BLOCK_LEN)
    sha1_update(context);

  /* copy rest bytes to buffer */
  rest_len = len - copy_len;
  if (rest_len > 0) {
    sy_memmove(context->buf, bytes + copy_len, rest_len);
    context->buf_len = rest_len;
  }
}

static void
sha1_update(sy_sha1_context *context)
{
  uint8_t *block;
  uint64_t mlen;

  block = context->buf;
  if (context->buf_len >= SY_SHA1_BLOCK_LEN)
    sha1_hash_block(context->state, block);
  else {
    /* padding */
    block[context->buf_len] = 0x80; /* 0b10000000 */

    if (context->buf_len > SY_SHA1_BLOCK_LEN - 8 - 1)
      sha1_hash_block(context->state, block);

    /* message length (bits) */
    mlen = context->total_len * 8;
    block[56] = (mlen >> 56) & 0xff;
    block[57] = (mlen >> 48) & 0xff;
    block[58] = (mlen >> 40) & 0xff;
    block[59] = (mlen >> 32) & 0xff;
    block[60] = (mlen >> 24) & 0xff;
    block[61] = (mlen >> 16) & 0xff;
    block[62] = (mlen >> 8) & 0xff;
    block[63] = mlen & 0xff;
    sha1_hash_block(context->state, block);
  }

  context->buf_len = 0;
  sy_memzero(block, SY_SHA1_BLOCK_LEN);
}

static inline void
sha1_hash_block(uint8_t *block, const uint8_t *data)
{
  uint8_t w[80*4], tmp1[4], tmp2[4], a[4], b[4], c[4], d[4], e[4];
  unsigned int t;

  sy_copy_words(w, data, 16*4);
  for (t = 16; t < 80; t++) {
    sy_lxor_word(tmp1, W(w, t-3), W(w, t-8));
    sy_lxor_word(tmp1, tmp1, W(w, t-14));
    sy_lxor_word(tmp1, tmp1, W(w, t-16));
    sy_rotl_word(W(w, t), tmp1, 1);
  }

  sy_copy_word(a, block);
  sy_copy_word(b, W(block, 1));
  sy_copy_word(c, W(block, 2));
  sy_copy_word(d, W(block, 3));
  sy_copy_word(e, W(block, 4));

  for (t = 0; t < 80; t++) {
    sy_rotl_word(tmp1, a, 5);
    sha1_f(tmp2, t, b, c, d);
    sy_add_word(tmp1, tmp1, tmp2);
    sy_add_word(tmp1, tmp1, e);
    sy_add_word(tmp1, tmp1, W(w, t));
    sy_add_word(tmp1, tmp1, sha1_K[t/20]);
    sy_copy_word(e, d);
    sy_copy_word(d, c);
    sy_rotl_word(c, b, 30);
    sy_copy_word(b, a);
    sy_copy_word(a, tmp1);
  }

  sy_add_word(block, block, a);
  sy_add_word(W(block, 1), W(block, 1), b);
  sy_add_word(W(block, 2), W(block, 2), c);
  sy_add_word(W(block, 3), W(block, 3), d);
  sy_add_word(W(block, 4), W(block, 4), e);

  sy_memzero(w, 80*4);
  sy_memzero_word(tmp1);
  sy_memzero_word(tmp2);
  sy_memzero_word(a);
  sy_memzero_word(b);
  sy_memzero_word(c);
  sy_memzero_word(d);
  sy_memzero_word(e);
}

void
_sy_sha1_hash_block(uint8_t *block, const uint8_t *data)
{
  sha1_hash_block(block, data);
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
  sha1_update(context);
  sy_memmove(dest, context->state, SY_SHA1_STATE_LEN);
  sy_memzero(context->state, SY_SHA1_STATE_LEN);
  sy_memzero(context->buf, SY_SHA1_BLOCK_LEN);
}

static inline void
sha256_sigma0(uint8_t *buf, const uint8_t *w)
{
  uint8_t tmp1[4], tmp2[4], tmp3[4];

  sy_rotr_word(tmp1, w, 7);
  sy_rotr_word(tmp2, w, 18);
  sy_rshift_word(tmp3, w, 3);
  sy_lxor_word3(buf, tmp1, tmp2, tmp3);
}

void
_sy_sha256_sigma0(uint8_t *buf, const uint8_t *w)
{
  sha256_sigma0(buf, w);
}

static inline void
sha256_sigma1(uint8_t *buf, const uint8_t *w)
{
  uint8_t tmp1[4], tmp2[4], tmp3[4];

  sy_rotr_word(tmp1, w, 17);
  sy_rotr_word(tmp2, w, 19);
  sy_rshift_word(tmp3, w, 10);
  sy_lxor_word3(buf, tmp1, tmp2, tmp3);
}

void
_sy_sha256_sigma1(uint8_t *buf, const uint8_t *w)
{
  sha256_sigma1(buf, w);
}

static inline void
sha256_ch(uint8_t *buf, const uint8_t *w1,
    const uint8_t *w2, const uint8_t *w3)
{
  uint8_t tmp1[4], tmp2[4];

  sy_land_word(tmp1, w1, w2);
  sy_invert_word(tmp2, w1);
  sy_land_word(tmp2, tmp2, w3);
  sy_lxor_word(buf, tmp1, tmp2);
}

void
_sy_sha256_ch(uint8_t *buf, const uint8_t *w1,
    const uint8_t *w2, const uint8_t *w3)
{
  sha256_ch(buf, w1, w2, w3);
}

static inline void
sha256_maj(uint8_t *buf, const uint8_t *w1,
    const uint8_t *w2, const uint8_t *w3)
{
  uint8_t tmp1[4], tmp2[4], tmp3[4];

  sy_land_word(tmp1, w1, w2);
  sy_land_word(tmp2, w1, w3);
  sy_land_word(tmp3, w2, w3);
  sy_lxor_word3(buf, tmp1, tmp2, tmp3);
}

void
_sy_sha256_maj(uint8_t *buf, const uint8_t *w1,
    const uint8_t *w2, const uint8_t *w3)
{
  sha256_maj(buf, w1, w2, w3);
}

static inline void
sha256_sum0(uint8_t *buf, const uint8_t *w)
{
  uint8_t tmp1[4], tmp2[4], tmp3[4];

  sy_rotr_word(tmp1, w, 2);
  sy_rotr_word(tmp2, w, 13);
  sy_rotr_word(tmp3, w, 22);
  sy_lxor_word3(buf, tmp1, tmp2, tmp3);
}

void
_sy_sha256_sum0(uint8_t *buf, const uint8_t *w)
{
  sha256_sum0(buf, w);
}

static inline void
sha256_sum1(uint8_t *buf, const uint8_t *w)
{
  uint8_t tmp1[4], tmp2[4], tmp3[4];

  sy_rotr_word(tmp1, w, 6);
  sy_rotr_word(tmp2, w, 11);
  sy_rotr_word(tmp3, w, 25);
  sy_lxor_word3(buf, tmp1, tmp2, tmp3);
}

void
_sy_sha256_sum1(uint8_t *buf, const uint8_t *w)
{
  sha256_sum1(buf, w);
}

