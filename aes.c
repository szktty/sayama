#include <stdio.h>
#include <string.h>
#include "sayama/aes.h"
#include "sayama/word.h"
#include "aes.h"

typedef struct context context;

#define BLOCK_LEN       16
#define MAX_KEY_LEN     64
#define MAX_IV_LEN      BLOCK_LEN
#define MAX_ROUND_KEYS  (SY_AES_KEY_256+6)
#define ROUND_KEYS_LEN  ((MAX_ROUND_KEYS+1)*BLOCK_LEN*4)

struct context {
  SY_CIPHER_DIRECTION dir;              /* direction */
  SY_AES_KEYLEN keylen;                 /* key length */
  unsigned int nrounds;                 /* number of rounds */
  uint8_t round_keys[ROUND_KEYS_LEN];   /* expanded key */
  uint8_t iv[MAX_IV_LEN];               /* initialization vector */
  SY_BLOCK_CIPHER_MODE mode;            /* block cipher mode */
  uint8_t state[BLOCK_LEN];             /* state */
};

static const uint8_t rcon[] = {
  0x8d, 0, 0, 0, 0x01, 0, 0, 0, 0x02, 0, 0, 0, 0x04, 0, 0, 0,
  0x08, 0, 0, 0, 0x10, 0, 0, 0, 0x20, 0, 0, 0, 0x40, 0, 0, 0,
  0x80, 0, 0, 0, 0x1b, 0, 0, 0, 0x36, 0, 0, 0, 0x6c, 0, 0, 0
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

static inline void sub_word(uint8_t *w);
static inline void rot_word(uint8_t *w);
static inline void set_block(uint8_t *dest, const uint8_t *src);
static void encrypt_block(uint8_t *block, uint8_t *round_keys,
    unsigned int round_count);
static inline void sub_bytes(uint8_t *block);
static inline void add_round_key(uint8_t *block, uint8_t *round_key);
static inline void shift_rows(uint8_t *block);

bool
sy_aes(uint8_t *dest, const uint8_t *key, SY_AES_KEYLEN keylen,
    SY_CIPHER_DIRECTION dir, const uint8_t *iv,
    const uint8_t *text, size_t textlen, SY_BLOCK_CIPHER_MODE mode)
{
  context ctxt;
  size_t i;

  /* validates key length */
  switch (keylen) {
  case SY_AES_KEY_128:
  case SY_AES_KEY_192:
  case SY_AES_KEY_256:
    ctxt.keylen = keylen;
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
  if (textlen % BLOCK_LEN != 0)
    return false;

  ctxt.dir = dir;
  ctxt.mode = mode;

  /* FIXME */
  if (iv != NULL)
    memcpy(ctxt.iv, iv, MAX_IV_LEN);

  /* expands key */
  ctxt.nrounds = _sy_aes_expand_key((uint8_t *)ctxt.round_keys,
      key, keylen);

  memset(ctxt.state, 0, BLOCK_LEN);
  switch (mode) {
  case SY_ECB_MODE:
    for (i = 0; i < textlen; i += BLOCK_LEN) {
      set_block(ctxt.state, text + i);
      encrypt_block(ctxt.state, ctxt.round_keys, ctxt.nrounds);
      set_block(dest + i, ctxt.state);
    }
    break;

    /* TODO: CBC, CFB, OFB, CTR */
  }

  /* clear cipher data */
  memset(ctxt.state, 0, BLOCK_LEN);
  memset(ctxt.round_keys, 0, BLOCK_LEN);
  return true;
}

static inline void
sub_word(uint8_t *w)
{
  w[0] = sbox[w[0]];
  w[1] = sbox[w[1]];
  w[2] = sbox[w[2]];
  w[3] = sbox[w[3]];
}

static inline void
rot_word(uint8_t *w)
{
  uint8_t tmp;

  tmp = w[0];
  w[0] = w[1];
  w[1] = w[2];
  w[2] = w[3];
  w[3] = tmp;
}

/* Expands key into round keys. Returns number of rounds. */
unsigned int
_sy_aes_expand_key(uint8_t *dest, const uint8_t *key,
    SY_AES_KEYLEN keylen)
{
  unsigned int nk; /* key length (word) */
  unsigned int nr; /* round count */
  unsigned int i, n;
  uint8_t x[4];

  /* first round key */
  nk = keylen / 32;
  nr = nk + 6;
  sy_copy_words(dest, key, nk * 4);

  /* rest rounds */
  n = (nr + 1) * 4;
  sy_copy_words(x, dest + (nk-1)*4, 1);
  for (i = nk; i < n; i++) {
    if (i % nk == 0) {
      rot_word(x);
      sub_word(x);
      sy_lxor_words(x, x, rcon + i/nk*4, 1);
    }

    /* 256 bit key length */
    if (nk == 8 && i % 8 == 4)
      sub_word(x);

    sy_lxor_words(x, x, dest + (i-nk)*4, 1);
    sy_copy_words(dest + i*4, x, 1);
  }

  return nr + 1;
}

static inline void
set_block(uint8_t *dest, const uint8_t *src)
{
  memcpy(dest, src, BLOCK_LEN);
}

static void
encrypt_block(uint8_t *block, uint8_t *round_keys,
    unsigned int round_count)
{
  unsigned int i;

  add_round_key(block, round_keys);
  for (i = 1; i < round_count; i++) {
    sub_bytes(block);
    shift_rows(block);
    if (i < round_count - 1)
      _sy_aes_mix_columns(block);
    add_round_key(block, round_keys + i*BLOCK_LEN);
  }
}

static inline void
sub_bytes(uint8_t *block)
{
  unsigned int i;

  for (i = 0; i < BLOCK_LEN; i += 4)
    sub_word(block + i);
}

static inline void
add_round_key(uint8_t *block, uint8_t *round_key)
{
  sy_lxor_words(block, block, round_key, 4);
}

static inline void
shift_rows(uint8_t *block)
{
  uint8_t tmp;

  /* S[1,n] */
  tmp = block[1];
  block[1] = block[5];
  block[5] = block[9];
  block[9] = block[13];
  block[13] = tmp;

  /* S[2,n] */
  tmp = block[2];
  block[2] = block[10];
  block[10] = tmp;
  tmp = block[6];
  block[6] = block[14];
  block[14] = tmp;

  /* S[3,n] */
  tmp = block[15];
  block[15] = block[11];
  block[11] = block[7];
  block[7] = block[3];
  block[3] = tmp;
}

bool
sy_aes128_encrypt(uint8_t *dest, const uint8_t *key,
    const uint8_t *iv, const uint8_t *text, size_t textlen,
    SY_BLOCK_CIPHER_MODE mode)
{
  return sy_aes(dest, key, 128, SY_ENCRYPT, iv, text,
      textlen, mode);
}

bool
sy_aes128_decrypt(uint8_t *dest, const uint8_t *key,
    const uint8_t *iv, const uint8_t *text, size_t textlen,
    SY_BLOCK_CIPHER_MODE mode)
{
  return sy_aes(dest, key, 128, SY_DECRYPT, iv, text,
      textlen, mode);
}

bool
sy_aes192_encrypt(uint8_t *dest, const uint8_t *key,
    const uint8_t *iv, const uint8_t *text, size_t textlen,
    SY_BLOCK_CIPHER_MODE mode)
{
  return sy_aes(dest, key, 192, SY_ENCRYPT, iv, text,
      textlen, mode);
}

bool
sy_aes192_decrypt(uint8_t *dest, const uint8_t *key,
    const uint8_t *iv, const uint8_t *text, size_t textlen,
    SY_BLOCK_CIPHER_MODE mode)
{
  return sy_aes(dest, key, 192, SY_DECRYPT, iv, text,
      textlen, mode);
}

bool
sy_aes256_encrypt(uint8_t *dest, const uint8_t *key,
    const uint8_t *iv, const uint8_t *text, size_t textlen,
    SY_BLOCK_CIPHER_MODE mode)
{
  return sy_aes(dest, key, 256, SY_ENCRYPT, iv, text,
      textlen, mode);
}

bool
sy_aes256_decrypt(uint8_t *dest, const uint8_t *key,
    const uint8_t *iv, const uint8_t *text, size_t textlen,
    SY_BLOCK_CIPHER_MODE mode)
{
  return sy_aes(dest, key, 256, SY_DECRYPT, iv, text,
      textlen, mode);
}

