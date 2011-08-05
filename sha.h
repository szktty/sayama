#ifndef __SY_SHA_PRIVATE_H__
#define __SY_SHA_PRIVATE_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

static inline void
_sy_sha1_f(uint8_t *dest, unsigned int t,
    const uint8_t *b, const uint8_t *c, const uint8_t *d)
{
  uint8_t tmp1[4], tmp2[4], tmp3[4];

  if (t < 20) {
    /* b & c | ~b & d */
    sy_and_word(tmp1, b, c);
    sy_invert_word(tmp2, tmp2, b);
    sy_and_word(tmp2, tmp2, d);
    sy_xor_word(dest, tmp1, tmp2);
  } else if (40 <= t && t < 60) {
    /* b & c | b & d | c & d */
    sy_and_word(tmp1, b, c);
    sy_and_word(tmp2, b, d);
    sy_and_word(tmp3, c, d);
    sy_xor_word(dest, tmp1, tmp2);
    sy_xor_word(dest, dest, tmp3);
  } else {
    /* b ^ c ^ d */
    sy_xor_word(dest, b, c);
    sy_xor_word(dest, dest, d);
  }

  sy_memzero_word(tmp1);
  sy_memzero_word(tmp2);
  sy_memzero_word(tmp3);
}

static inline void
_sy_sha1_hash_block(uint8_t *state, const uint8_t *block)
{
  uint8_t w[80*4], tmp[4], a[4], b[4], c[4], d[4], e[4];
  unsigned int t;

  sy_copy_words(w, block, 16);
  for (t = 16; t < 80; t++) {
    sy_xor_word(tmp, W(w, t-3), W(w, t-8));
    sy_xor_word(tmp, tmp, W(w, t-14));
    sy_xor_word(tmp, tmp, W(w, t-16));
    sy_rotl_word(W(w, t), tmp, 1);
  }

  sy_copy_word(a, state);
  sy_copy_word(b, W(state, 1));
  sy_copy_word(c, W(state, 2));
  sy_copy_word(d, W(state, 3));
  sy_copy_word(e, W(state, 4));

  for (t = 0; t < 80; t++) {
    sy_rotl_word(tmp, a, 5);
    _sy_sha1_f(tmp, t, b, c, d);
    sy_add_word(tmp, tmp, e);
    sy_add_word(tmp, tmp, W(w, t));
    sy_add_word(tmp, tmp, sha1_K(t/20));
    e = d;
    d = c;
    sy_rotl_word(c, b, 30);
    b = a;
    a = tmp;
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

#ifdef __cplusplus
}
#endif

#endif /* __SY_SHA_H__ */

