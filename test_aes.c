#include "sayama/aes.h"
#include "sayama/utils.h"
#include "aes.h"
#include "cut-extends.h"

void test_aes_mix_columns(void);
void test_aes_mul2(void);
void test_aes_expand_key_128(void);
void test_aes_encrypt_ecb_128(void);

void
test_aes_mix_columns()
{
  uint8_t b[16] = { 
    0xdb, 0x13, 0x53, 0x45, 0xf2, 0x0a, 0x22, 0x5c,
    0x01, 0x01, 0x01, 0x01, 0xc6, 0xc6, 0xc6, 0xc6
  };
  uint8_t e[16] = {
    0x8e, 0x4d, 0xa1, 0xbc, 0x9f, 0xdc, 0x58, 0x9d,
    0x01, 0x01, 0x01, 0x01, 0xc6, 0xc6, 0xc6, 0xc6
  };

  _sy_aes_mix_columns(b);
  cut_assert_equal_bytes(e, b, 16, cut_message("mix column failed"));
}

void
test_aes_mul2(void)
{
  uint8_t e[256], v;
  unsigned int i;

  sy_load_bytes(e, "test_aes_mul2.txt", 256);
  for (i = 0; i < 256; i++) {
    v = _sy_aes_mul2((uint8_t)i);
    cut_assert(v == e[i], cut_message("unexpected value"));
  }
}

void
test_aes_expand_key_128()
{
#define NR          11
#define KEYS        (NR*4)
#define KEYS_LEN    (KEYS*4)

  uint8_t key[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t round_keys[KEYS_LEN], expected_keys[KEYS_LEN];
  unsigned int nr;

  memset(round_keys, 0, KEYS);
  sy_load_bytes(expected_keys, "test_aes_key_128.txt", KEYS_LEN);
  nr = _sy_aes_expand_key(round_keys, key, SY_AES_KEY_128);
  cut_assert(NR == nr, cut_message("unexpected round count"));
  cut_assert_equal_bytes(expected_keys, round_keys, KEYS_LEN,
      cut_message("unexpected key"));
}

void
test_aes_encrypt_ecb_128(void)
{
  uint8_t key[16], text[16], enc[16], expected[16];

  sy_load_bytes(key, "test_aes_encrypt_ecb_128_key.txt", 16);
  sy_load_bytes(text, "test_aes_encrypt_ecb_128_text.txt", 16);
  sy_load_bytes(expected, "test_aes_encrypt_ecb_128_enc.txt", 16);
  cut_assert(sy_aes(enc, key, SY_AES_KEY_128, SY_ENCRYPT,
        NULL, text, 16, SY_ECB_MODE),
      cut_message("encrypt failed"));
  cut_assert_equal_bytes(expected, enc, 16,
      cut_message("unexpected cipher text"));
}

