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
  sy_word w[4] = { 
    0xdb135345, 0xf20a225c, 0x01010101, 0xc6c6c6c6
  };
  sy_word e[4] = {
    0x8e4da1bc, 0x9fdc589d, 0x01010101, 0xc6c6c6c6
  };

  _sy_aes_mix_columns(w);
  cut_assert(sy_equal_words(e, 0, w, 0, 16));
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
#define KEYS        NR*4
#define KEYS_LEN    (KEYS*4)

  uint8_t key[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  sy_word round_keys[KEYS], expected_keys[KEYS];
  uint8_t tmp[KEYS_LEN];
  unsigned int nr;

  sy_clear_words(round_keys, KEYS);
  sy_load_bytes(tmp, "test_aes_key_128.txt", KEYS_LEN);
  sy_encode_words(expected_keys, 0, tmp, KEYS_LEN);
  nr = _sy_aes_expand_key(round_keys, key, SY_AES_KEY_128);
  cut_assert(NR == nr, cut_message("unexpected round count"));
  cut_assert(sy_equal_words(expected_keys, 0, round_keys, 0, KEYS));
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

