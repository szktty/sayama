#include "sayama/sha.h"
#include "sayama/word.h"
#include "sayama/utils.h"
#include "cut-extends.h"
#include "sha.h"

void test_sha1_short_bytes(void);
void test_sha1_long_bytes(void);
void test_sha1_block_len_bytes(void);
void test_sha1_updates(void);
void test_sha1_f(void);
void test_sha1_hash_block(void);

void test_sha256_sigma0(void);
void test_sha256_sigma1(void);
void test_sha256_ch(void);
void test_sha256_maj(void);
void test_sha256_sum0(void);
void test_sha256_sum1(void);
void test_sha256_one_block_msg(void);
void test_sha256_multi_block_msg(void);
void test_sha256_long_msg(void);

void
test_sha1_short_bytes()
{
  const char *src = "hello, world!";
  uint8_t a[200], e[200];
  sy_sha1_context context;

  sy_load_bytes(e, "test_sha1_short_bytes_digest.txt", 20);
  sy_sha1_init(&context);
  sy_sha1_update(&context, (uint8_t *)src, strlen(src));
  sy_sha1_final(&context, a);
  cut_assert_equal_bytes(e, a, 20, cut_message("unexpected digest"));
}

void
test_sha1_long_bytes()
{
  const char *src = "0000000000111111111122222222223333333333"
    "444444444455555555556666666666777777777788888888889999999999";
  uint8_t a[200], e[200];
  sy_sha1_context context;

  sy_load_bytes(e, "test_sha1_digest.txt", 20);
  sy_sha1_init(&context);
  sy_sha1_update(&context, (uint8_t *)src, strlen(src));
  sy_sha1_final(&context, a);
  cut_assert_equal_bytes(e, a, 20, cut_message("unexpected digest"));
}

void
test_sha1_block_len_bytes(void)
{
  const char src[] = "00000000000000000000000000000000"
    "00000000000000000000000000000000";
  uint8_t a[20], e[20];
  sy_sha1_context context;

  sy_load_bytes(e, "test_sha1_block_len_bytes_digest.txt", 20);
  sy_sha1_init(&context);
  sy_sha1_update(&context, (uint8_t *)src, strlen(src));
  sy_sha1_final(&context, a);
  cut_assert_equal_bytes(e, a, 20, cut_message("unexpected digest"));
}

void
test_sha1_updates(void)
{
  const char m1[] = "hello", m2[] = ", ", m3[] = "world!";
  uint8_t a[20], e[20];
  sy_sha1_context context;

  sy_load_bytes(e, "test_sha1_updates.txt", 20);
  sy_sha1_init(&context);
  sy_sha1_update(&context, (uint8_t *)m1, strlen(m1));
  sy_sha1_update(&context, (uint8_t *)m2, strlen(m2));
  sy_sha1_update(&context, (uint8_t *)m3, strlen(m3));
  sy_sha1_final(&context, a);
  cut_assert_equal_bytes(e, a, 20, cut_message("unexpected digest"));
}

void
test_sha1_f()
{
  /* t < 20 */
  cut_assert(0x45d12aa0 ==
      _sy_sha1_f(3, 0x5d43e370, 0xc7ed262c, 0x59d148c0));

  /* 20 <= t && t < 40 */
  cut_assert(0x3739d441 ==
      _sy_sha1_f(37, 0xabbab988, 0x0e47bc31, 0x92c4d1f8));

  /* 40 <= t && t < 60 */
  cut_assert(0x891f3c93 ==
      _sy_sha1_f(45, 0x291f2d96, 0x85275613, 0x8b9e38d1));

  /* 60 <= t && t <= 80 */
  cut_assert(0xf5f28208 ==
      _sy_sha1_f(73, 0xf10f8b24, 0xa3dd0263, 0xa7200b4f));
}

void
test_sha1_hash_block()
{
  sy_word block[] = {0x3361587f, 0xa36f83ae, 0x9c559116,
    0x1f66f2ab, 0x8dfa5554};
  uint8_t data[] = {0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
    0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37,
    0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38,
    0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39,
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x20};
  sy_word ex[] = {0x9fbfcff0, 0xa3a71555,
    0xdca89b81, 0x3a2db3fe, 0x1e8395f4};

  _sy_sha1_hash_block(block, data);
  cut_assert(sy_equal_words(ex, 0, block, 0, 20),
      cut_message("unexpected hashed block"));
}

void
test_sha256_sigma0(void)
{
  cut_assert_equal_word(0xe7fce6ee, _sy_sha256_sigma0(0x12345678));
  cut_assert_equal_word(0x5abb9899, _sy_sha256_sigma0(0x13579bdf));
}

void
test_sha256_sigma1(void)
{
  cut_assert_equal_word(0xa1f78649, _sy_sha256_sigma1(0x12345678));
  cut_assert_equal_word(0x3e90be27, _sy_sha256_sigma1(0x13579bdf));
}

void
test_sha256_ch(void)
{
  cut_assert_equal_word(0x3e67b715,
      _sy_sha256_ch(0x6a09e667, 0xbb67ae85, 0x3c6ef372));
}

void
test_sha256_maj(void)
{
  cut_assert_equal_word(0x3a6fe667,
      _sy_sha256_maj(0x6a09e667, 0xbb67ae85, 0x3c6ef372));
}

void
test_sha256_sum0(void)
{
  cut_assert_equal_word(0x66146474, _sy_sha256_sum0(0x12345678));
}

void
test_sha256_sum1(void)
{
  cut_assert_equal_word(0x3561abda, _sy_sha256_sum1(0x12345678));
}

void
test_sha256_one_block_msg()
{
  const char msg[] = "abc";
  uint8_t e[32], a[32];
  sy_sha256_context context;

  sy_load_bytes(e, "test_sha256_one_block_msg_digest.txt", 32);
  sy_sha256_init(&context);
  sy_sha256_update(&context, (uint8_t *)msg, strlen(msg));
  sy_sha256_final(&context, a);
  cut_assert_equal_bytes(e, a, 32, cut_message("unexpected digest"));
}

void
test_sha256_multi_block_msg()
{
  const char msg[] = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
  uint8_t e[32], a[32];
  sy_sha256_context context;

  sy_load_bytes(e, "test_sha256_multi_block_msg_digest.txt", 32);
  sy_sha256_init(&context);
  sy_sha256_update(&context, (uint8_t *)msg, strlen(msg));
  sy_sha256_final(&context, a);
  cut_assert_equal_bytes(e, a, 32, cut_message("unexpected digest"));
}

void
test_sha256_long_msg()
{
  char *msg;
  size_t len = 1000000;
  uint8_t e[32], a[32];
  sy_sha256_context context;

  msg = (char *)malloc(len);
  memset(msg, 'a', len);

  sy_load_bytes(e, "test_sha256_long_msg_digest.txt", 32);
  sy_sha256_init(&context);
  sy_sha256_update(&context, (uint8_t *)msg, len);
  sy_sha256_final(&context, a);
  cut_assert_equal_bytes(e, a, 32, cut_message("unexpected digest"));
  free(msg);
}

