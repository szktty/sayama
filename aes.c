#include <stdio.h>
#include <string.h>
#include "sayama/aes.h"
#include "sayama/word.h"
#include "aes.h"

typedef struct context context;

#define STATE_LEN       16
#define STATE_WLEN      (STATE_LEN/4)
#define MAX_KEY_LEN     64
#define MAX_IV_LEN      STATE_LEN
#define MAX_ROUND_KEYS  (SY_AES_KEY_256+6)
#define ROUND_KEYS_WLEN ((MAX_ROUND_KEYS+1)*STATE_LEN)
#define ROUND_KEYS_LEN  (ROUND_KEYS_WLEN*4)

struct context {
  SY_CIPHER_DIRECTION dir;              /* direction */
  SY_AES_KEY_LEN key_len;               /* key length */
  unsigned int nrounds;                 /* number of rounds */
  sy_word round_keys[ROUND_KEYS_WLEN];  /* expanded key */
  uint8_t iv[MAX_IV_LEN];               /* initialization vector */
  SY_BLOCK_CIPHER_MODE mode;            /* block cipher mode */
  sy_word state[STATE_WLEN];            /* state */
};

static const sy_word rcon[] = {
  0x8d000000U, 0x01000000U, 0x02000000U, 0x04000000U,
  0x08000000U, 0x10000000U, 0x20000000U, 0x40000000U,
  0x80000000U, 0x1b000000U, 0x36000000U, 0x6c000000U,
};

static const uint8_t sbox[] = {
  /* 0 */
  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
  0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
  /* 1 */
  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
  0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
  /* 2 */
  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
  0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
  /* 3 */
  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
  0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
  /* 4 */
  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
  0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
  /* 5 */
  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
  0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
  /* 6 */
  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85,
  0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
  /* 7 */
  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
  0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
  /* 8 */
  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
  0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
  /* 9 */
  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
  0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
  /* a */
  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
  0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
  /* b */
  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
  0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
  /* c */
  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
  0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
  /* d */
  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
  0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
  /* e */
  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
  0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
  /* f */
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
  0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

static inline void mix_columns(sy_word *block);
static inline uint8_t mul2(uint8_t v);
static inline sy_word sub_word(sy_word w);
static inline sy_word rot_word(sy_word w);
static inline unsigned int expand_key(sy_word *dest,
    const uint8_t *key, SY_AES_KEY_LEN key_len);
static inline void set_block(sy_word *dest, const uint8_t *src);
static void encrypt_block(sy_word *block, sy_word *round_keys,
    unsigned int round_count);
static inline void sub_bytes(sy_word *block);
static inline void add_round_key(sy_word *block, sy_word *round_key);
static inline void shift_rows(sy_word *block);

extern bool
sy_aes(uint8_t *dest, SY_AES_KEY_LEN key_len,
    SY_BLOCK_CIPHER_MODE mode, SY_CIPHER_DIRECTION dir,
    const uint8_t *key,  const uint8_t *iv,
    const uint8_t *text, size_t text_len)
{
  context ctxt;
  size_t i;
  volatile sy_word *state, *round_keys;

  state = (volatile sy_word *)ctxt.state;
  round_keys = (volatile sy_word *)ctxt.round_keys;

  /* validates key length */
  switch (key_len) {
  case SY_AES_KEY_128:
  case SY_AES_KEY_192:
  case SY_AES_KEY_256:
    ctxt.key_len = key_len;
    break;
  default:
    return false;
  }

  /* validates block cipher mode */
  switch (mode) {
  case SY_ECB_MODE:
  case SY_CBC_MODE:
  case SY_CFB_MODE:
  case SY_OFB_MODE:
  case SY_CTR_MODE:
    break;
  default:
    return false;
  }

  /* validate text length */
  if (text_len % STATE_LEN != 0)
    return false;

  ctxt.dir = dir;
  ctxt.mode = mode;

  /* FIXME */
  if (iv != NULL)
    memcpy(ctxt.iv, iv, MAX_IV_LEN);

  /* expands key */
  ctxt.nrounds = expand_key(round_keys, key, key_len);

  sy_clear_words(state, STATE_WLEN);
  switch (mode) {
  case SY_ECB_MODE:
    for (i = 0; i < text_len; i += STATE_LEN) {
      set_block(state, text + i);
      encrypt_block(state, round_keys, ctxt.nrounds);
      sy_decode_words(dest + i/4, state, 0, STATE_LEN-1);
    }
    break;

    /* TODO: CBC, CFB, OFB, CTR */
  }

  /* clear cipher data */
  sy_clear_words(state, STATE_WLEN);
  sy_clear_words(round_keys, STATE_WLEN);
  return true;
}

void
_sy_aes_mix_columns(sy_word *block)
{
  mix_columns(block);
}

static inline void
mix_columns(sy_word *block)
{
  unsigned int i;
  uint8_t o0, o1, o2, o3, t0, t1, t2, t3;

  for (i = 0; i < 4; i++) {
    o0 = sy_word_get0(block[i]);
    o1 = sy_word_get1(block[i]);
    o2 = sy_word_get2(block[i]);
    o3 = sy_word_get3(block[i]);
    t0 = mul2(o0);
    t1 = mul2(o1);
    t2 = mul2(o2);
    t3 = mul2(o3);
    block[i] = sy_create_word(
        t0        ^ (o1 ^ t1) ^ o2        ^ o3,
        o0        ^ t1        ^ (o2 ^ t2) ^ o3,
        o0        ^ o1        ^ t2        ^ (o3 ^ t3),
        (o0 ^ t0) ^ o1        ^ o2        ^ t3);
  }
}

uint8_t
_sy_aes_mul2(uint8_t v)
{
  return mul2(v);
}

static inline uint8_t
mul2(uint8_t v)
{
  if ((v & 0x80) != 0)
    return (v << 1) ^ 0x011b;
  else
    return v << 1;
}

static inline sy_word
sub_word(sy_word w)
{
  return sy_create_word(sbox[sy_word_get0(w)], sbox[sy_word_get1(w)],
      sbox[sy_word_get2(w)], sbox[sy_word_get3(w)]);
}

static inline sy_word
rot_word(sy_word w)
{
  return sy_create_word(sy_word_get1(w), sy_word_get2(w),
      sy_word_get3(w), sy_word_get0(w));
}

/* Expands key into round keys. Returns number of rounds. */
unsigned int
_sy_aes_expand_key(sy_word *dest, const uint8_t *key,
    SY_AES_KEY_LEN key_len)
{
  return expand_key(dest, key, key_len);
}

static inline unsigned int
expand_key(sy_word *dest, const uint8_t *key, SY_AES_KEY_LEN key_len)
{
  unsigned int nk; /* key length (word) */
  unsigned int nr; /* round count */
  unsigned int i, n;
  sy_word x;

  /* first round key */
  nk = key_len / 32;
  nr = nk + 6;
  sy_encode_words(dest, 0, key, nk * 4);

  /* rest rounds */
  n = (nr + 1) * 4;
  x = dest[nk-1];
  for (i = nk; i < n; i++) {
    if (i % nk == 0)
      x = sub_word(rot_word(x)) ^ rcon[i/nk];

    /* 256 bit key length */
    if (nk == 8 && i % 8 == 4)
      x = sub_word(x);

    x ^= dest[i-nk];
    dest[i] = x;
  }

  return nr + 1;
}

static inline void
set_block(sy_word *dest, const uint8_t *src)
{
  sy_encode_words(dest, 0, src, STATE_LEN);
}

static void
encrypt_block(sy_word *block, sy_word *round_keys,
    unsigned int round_count)
{
  unsigned int i;

  add_round_key(block, round_keys);
  for (i = 1; i < round_count; i++) {
    sub_bytes(block);
    shift_rows(block);
    if (i < round_count - 1)
      mix_columns(block);
    add_round_key(block, round_keys + i*STATE_WLEN);
  }
}

static inline void
sub_bytes(sy_word *block)
{
  unsigned int i;

  for (i = 0; i < STATE_WLEN; i++)
    block[i] = sub_word(block[i]);
}

static inline void
add_round_key(sy_word *block, sy_word *round_key)
{
  block[0] ^= round_key[0];
  block[1] ^= round_key[1];
  block[2] ^= round_key[2];
  block[3] ^= round_key[3];
}

static inline void
shift_rows(sy_word *block)
{
  uint8_t tmp;

  /* S[1,n] */
  tmp = sy_word_get(block, 1);
  sy_word_set(block, 1, sy_word_get(block, 5));
  sy_word_set(block, 5, sy_word_get(block, 9));
  sy_word_set(block, 9, sy_word_get(block, 13));
  sy_word_set(block, 13, tmp);

  /* S[2,n] */
  tmp = sy_word_get(block, 2);
  sy_word_set(block, 2, sy_word_get(block, 10));
  sy_word_set(block, 10, tmp);
  tmp = sy_word_get(block, 6);
  sy_word_set(block, 6, sy_word_get(block, 14));
  sy_word_set(block, 14, tmp);

  /* S[3,n] */
  tmp = sy_word_get(block, 15);
  sy_word_set(block, 15, sy_word_get(block, 11));
  sy_word_set(block, 11, sy_word_get(block, 7));
  sy_word_set(block, 7, sy_word_get(block, 3));
  sy_word_set(block, 3, tmp);
}

