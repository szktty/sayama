#include "sayama/memory.h"
#include "sayama/sha.h"
#include "sayama/utils.h"
#include "sayama/word.h"
#include "sha.h"

static void sha1_update(uint8_t *buf, size_t len, sy_sha1_context *context);
static inline void sha1_hash_block(sy_word *state, const uint8_t *block);
static inline sy_word sha1_f(unsigned int t, sy_word b, sy_word c, sy_word d);

static inline sy_word sha256_sigma0(sy_word w);
static inline sy_word sha256_sigma1(sy_word w);
static inline sy_word sha256_ch(sy_word w1, sy_word w2, sy_word w3);
static inline sy_word sha256_maj(sy_word w1, sy_word w2, sy_word w3);
static inline sy_word sha256_sum0(sy_word w);
static inline sy_word sha256_sum1(sy_word w);
static void sha256_update(uint8_t *block, size_t len,
    sy_sha256_context *context);
static inline void sha256_hash_block(uint8_t *state, const uint8_t *block);

static uint8_t sha1_K[][4] = {
  {0x5a, 0x82, 0x79, 0x99},
  {0x6e, 0xd9, 0xeb, 0xa1},
  {0x8f, 0x1b, 0xbc, 0xdc},
  {0xca, 0x62, 0xc1, 0xd6}
};

static uint32_t sha256_K[] = {
  0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
  0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
  0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
  0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
  0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
  0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
  0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
  0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
  0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
  0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
  0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
  0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
  0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
  0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
  0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
  0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };

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
  context->state[0] = 0x67452301U;
  context->state[1] = 0xefcdab89U;
  context->state[2] = 0x98badcfeU;
  context->state[3] = 0x10325476U;
  context->state[4] = 0xc3d2e1f0U;
  sy_memzero(context->buf, SY_SHA1_BLOCK_LEN);
  context->buf_len = 0;
  context->total_len = 0;
}

void
sy_sha1_update(sy_sha1_context *context, const uint8_t *bytes,
    size_t len)
{
  context->total_len += len;
  context->buf_len = sy_append_to_buffer(context->buf,
      context->buf_len, SY_SHA1_BLOCK_LEN, bytes, len,
      (sy_buffer_operator)sha1_update, context);
}

static void
sha1_update(uint8_t *block, size_t len, sy_sha1_context *context)
{
  uint64_t mlen;

  if (len >= SY_SHA1_BLOCK_LEN)
    sha1_hash_block(context->state, block);
  else {
    /* padding */
    block[len] = 0x80; /* 0b10000000 */

    if (len > SY_SHA1_BLOCK_LEN - 8 - 1) {
      sy_memzero(block + len + 1, SY_SHA1_BLOCK_LEN - len);
      sha1_hash_block(context->state, block);
      sy_memzero(block, SY_SHA1_BLOCK_LEN - 8);
    } else
      sy_memzero(block + len + 1, SY_SHA1_BLOCK_LEN - len - 8);


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
}

static inline void
sha1_hash_block(sy_word *block, const uint8_t *data)
{
  volatile sy_word w[80], a, b, c, d, e, tmp;
  unsigned int t;

  sy_encode_words(w, 0, data, 16*4);
  for (t = 16; t < 80; t++)
    w[t] = sy_rotl_word(w[t-3] ^ w[t-8] ^ w[t-14] ^ w[t-16], 1);

  a = block[0];
  b = block[1];
  c = block[2];
  d = block[3];
  e = block[4];

  for (t = 0; t < 80; t++) {
    tmp = sy_rotl_word(a, 5) + sha1_f(t, b, c, d) + e + w[t] + sha1_K[t/20];
    e = d;
    d = c;
    c = sy_rotl_word(b, 30);
    b = a;
    a = tmp;
  }

  block[0] += a;
  block[1] += b;
  block[2] += c;
  block[3] += d;
  block[4] += e;

  sy_memzero(w, 80*4);
  a = 0;
  b = 0;
  c = 0;
  d = 0;
  e = 0;
  tmp = 0;
}

void
_sy_sha1_hash_block(sy_word *block, const uint8_t *data)
{
  sha1_hash_block(block, data);
}

static inline sy_word
sha1_f(unsigned int t, sy_word b, sy_word c, sy_word d)
{
  if (t < 20)
    return (b & c) | (~b & d);
  else if (40 <= t && t < 60)
    return (b & c) | (b & d) | (c & d);
  else
    return b ^ c ^ d;
}

sy_word
_sy_sha1_f(unsigned int t, sy_word b, sy_word c, sy_word d)
{
  return sha1_f(t, b, c, d);
}

void
sy_sha1_final(sy_sha1_context *context, uint8_t *dest)
{
  sha1_update(context->buf, context->buf_len, context);
  sy_decode_words(dest, context->state, 0, SY_SHA1_STATE_LEN-1);
  sy_clear_words(context->state, 0, SY_SHA1_STATE_LEN-1);
  sy_memzero(context->buf, SY_SHA1_BLOCK_LEN);
}

static inline sy_word
sha256_sigma0(sy_word w)
{
  return sy_rotr_word(w, 7) ^ sy_rotr_word(w, 18) ^ (w >> 3);
}

sy_word
_sy_sha256_sigma0(sy_word w)
{
  return sha256_sigma0(w);
}

static inline sy_word
sha256_sigma1(sy_word w)
{
  return sy_rotr_word(w, 17) ^ sy_rotr_word(w, 19) ^ (w >> 10);
}

sy_word
_sy_sha256_sigma1(sy_word w)
{
  return sha256_sigma1(w);
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

void
sy_sha256(uint8_t *buf, const uint8_t *data, size_t len)
{
  sy_sha256_context context;

  sy_sha256_init(&context);
  sy_sha256_update(&context, data, len);
  sy_sha256_final(&context, buf);
}

void
sy_sha256_init(sy_sha256_context *context)
{
  static uint8_t init[] = { 0x6a, 0x09, 0xe6, 0x67,
    0xbb, 0x67, 0xae, 0x85, 0x3c, 0x6e, 0xf3, 0x72,
    0xa5, 0x4f, 0xf5, 0x3a, 0x51, 0x0e, 0x52, 0x7f,
    0x9b, 0x05, 0x68, 0x8c, 0x1f, 0x83, 0xd9, 0xab,
    0x5b, 0xe0, 0xcd, 0x19};

  sy_memmove(context->state, init, SY_SHA256_STATE_LEN);
  sy_memzero(context->buf, SY_SHA256_BLOCK_LEN);
  context->buf_len = 0;
  context->total_len = 0;
}

void
sy_sha256_update(sy_sha256_context *context,
    const uint8_t *data, size_t len)
{
  context->total_len += len;
  context->buf_len = sy_append_to_buffer(context->buf,
      context->buf_len, SY_SHA256_BLOCK_LEN, data, len,
      (sy_buffer_operator)sha256_update, context);
}

static void
sha256_update(uint8_t *block, size_t len, sy_sha256_context *context)
{
  uint64_t mlen;

  if (len >= SY_SHA256_BLOCK_LEN)
    sha256_hash_block(context->state, block);
  else {
    /* padding */
    block[len] = 0x80; /* 0b10000000 */

    if (len > SY_SHA256_BLOCK_LEN - 8 - 1) {
      sy_memzero(block + len + 1, SY_SHA256_BLOCK_LEN - len);
      sha256_hash_block(context->state, block);
      sy_memzero(block, SY_SHA256_BLOCK_LEN - 8);
    } else
      sy_memzero(block + len + 1, SY_SHA256_BLOCK_LEN - len - 8);

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
    sha256_hash_block(context->state, block);
  }
}

static inline void
sha256_hash_block(uint8_t *block, const uint8_t *data)
{
  uint8_t w[64*4], tmp1[4], tmp2[4], tmp3[4], a[4], b[4], c[4], d[4],
          e[4], f[4], g[4], h[4];
  unsigned int t;

  sy_copy_words(w, data, 16*4);
  for (t = 16; t < 64; t++) {
    sha256_sigma1(tmp1, W(w, t-2));
    sy_add_word(W(w, t), tmp1, W(w, t-7));
    sha256_sigma0(tmp1, W(w, t-15));
    sy_add_word(W(w, t), W(w, t), tmp1);
    sy_add_word(W(w, t), W(w, t), W(w, t-16));
  }

  sy_copy_word(a, block);
  sy_copy_word(b, W(block, 1));
  sy_copy_word(c, W(block, 2));
  sy_copy_word(d, W(block, 3));
  sy_copy_word(e, W(block, 4));
  sy_copy_word(f, W(block, 5));
  sy_copy_word(g, W(block, 6));
  sy_copy_word(h, W(block, 7));

  for (t = 0; t < 64; t++) {
    sha256_sum1(tmp2, e);
    sy_add_word(tmp1, h, tmp2);
    sha256_ch(tmp2, e, f, g);
    sy_add_word(tmp1, tmp1, tmp2);
    sy_set_word_value(tmp2, sha256_K[t]);
    sy_add_word(tmp1, tmp1, tmp2);
    sy_add_word(tmp1, tmp1, W(w, t));
    sha256_sum0(tmp2, a);
    sha256_maj(tmp3, a, b, c);
    sy_add_word(tmp2, tmp2, tmp3);

    sy_copy_word(h, g);
    sy_copy_word(g, f);
    sy_copy_word(f, e);
    sy_add_word(e, d, tmp1);
    sy_copy_word(d, c);
    sy_copy_word(c, b);
    sy_copy_word(b, a);
    sy_add_word(a, tmp1, tmp2);
  }

  sy_add_word(block, block, a);
  sy_add_word(W(block, 1), W(block, 1), b);
  sy_add_word(W(block, 2), W(block, 2), c);
  sy_add_word(W(block, 3), W(block, 3), d);
  sy_add_word(W(block, 4), W(block, 4), e);
  sy_add_word(W(block, 5), W(block, 5), f);
  sy_add_word(W(block, 6), W(block, 6), g);
  sy_add_word(W(block, 7), W(block, 7), h);

  sy_memzero(w, 64*4);
  sy_memzero_word(tmp1);
  sy_memzero_word(tmp2);
  sy_memzero_word(tmp3);
  sy_memzero_word(a);
  sy_memzero_word(b);
  sy_memzero_word(c);
  sy_memzero_word(d);
  sy_memzero_word(e);
  sy_memzero_word(f);
  sy_memzero_word(g);
  sy_memzero_word(h);
}

void
sy_sha256_final(sy_sha256_context *context, uint8_t *buf)
{
  sha256_update(context->buf, context->buf_len, context);
  sy_memmove(buf, context->state, SY_SHA256_STATE_LEN);
  sy_memzero(context->state, SY_SHA256_STATE_LEN);
  sy_memzero(context->buf, SY_SHA256_BLOCK_LEN);
}

