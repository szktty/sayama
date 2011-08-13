#include "sayama/word.h"
#include "sayama/utils.h"
#include "cut-extends.h"

void test_encode_words(void);
void test_decode_words(void);
void test_equal_words(void);
void test_rotl_word(void);
void test_rotr_word(void);
void test_copy_words(void);
/*
void test_memset_words(void);
void test_memzero_words(void);
*/

void
test_encode_words()
{
  uint8_t w[] = { 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0 };
  sy_word a[2], e[] = {0x12345678, 0x9abcdef0};

  sy_encode_words(a, w, 8, 0xff);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  e[1] = 0x9abcdeff;
  sy_encode_words(a, w, 7, 0xff);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  e[1] = 0x9abcffff;
  sy_encode_words(a, w, 6, 0xff);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  e[1] = 0x9affffff;
  sy_encode_words(a, w, 5, 0xff);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  e[0] = 0x12345678;
  sy_encode_words(a, w, 4, 0xff);
  cut_assert(e[0] == a[0], cut_message("unexpected word"));

  e[0] = 0x123456ff;
  sy_encode_words(a, w, 3, 0xff);
  cut_assert(e[0] == a[0], cut_message("unexpected word"));

  e[0] = 0x1234ffff;
  sy_encode_words(a, w, 2, 0xff);
  cut_assert(e[0] == a[0], cut_message("unexpected word"));

  e[0] = 0x12ffffff;
  sy_encode_words(a, w, 1, 0xff);
  cut_assert(e[0] == a[0], cut_message("unexpected word"));
}

void
test_decode_words()
{
  uint8_t a[8], e[] = { 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0 };
  sy_word w[] = {0x12345678, 0x9abcdef0};

  memset(a, 0xff, 8);
  sy_decode_words(a, w, 8);
  cut_assert_equal_bytes(e, a, 8, cut_message("unexpected word"));

  memset(a, 0xff, 8);
  e[7] = 0xff;
  sy_decode_words(a, w, 7);
  cut_assert_equal_bytes(e, a, 8, cut_message("unexpected word"));

  memset(a, 0xff, 8);
  e[6] = 0xff;
  sy_decode_words(a, w, 6);
  cut_assert_equal_bytes(e, a, 8, cut_message("unexpected word"));

  memset(a, 0xff, 8);
  e[5] = 0xff;
  sy_decode_words(a, w, 5);
  cut_assert_equal_bytes(e, a, 8, cut_message("unexpected word"));

  memset(a, 0xff, 8);
  e[4] = 0xff;
  sy_decode_words(a, w, 4);
  cut_assert_equal_bytes(e, a, 8, cut_message("unexpected word"));

  memset(a, 0xff, 8);
  e[3] = 0xff;
  sy_decode_words(a, w, 3);
  cut_assert_equal_bytes(e, a, 8, cut_message("unexpected word"));

  memset(a, 0xff, 8);
  e[2] = 0xff;
  sy_decode_words(a, w, 2);
  cut_assert_equal_bytes(e, a, 8, cut_message("unexpected word"));

  memset(a, 0xff, 8);
  e[1] = 0xff;
  sy_decode_words(a, w, 1);
  cut_assert_equal_bytes(e, a, 8, cut_message("unexpected word"));
}

void
test_equal_words()
{
  sy_word ws1[2], ws2[2];

  ws1[0] = 0x12345678;
  ws1[1] = 0x12345678;
  ws2[0] = 0x12345678;
  ws2[1] = 0x12345678;
  cut_assert_true(sy_equal_words(ws1, ws2, 8), cut_message("fail 1"));

  ws1[0] = 0x00000000;
  cut_assert_false(sy_equal_words(ws1, ws2, 8), cut_message("fail 2"));
  ws1[0] = 0x12345678;

  ws1[1] = 0x00000000;
  cut_assert_true(sy_equal_words(ws1, ws2, 4), cut_message("fail 3"));
  cut_assert_false(sy_equal_words(ws1, ws2, 8), cut_message("fail 4"));
  ws1[1] = 0x12345678;

  ws2[0] = 0x00000000;
  cut_assert_false(sy_equal_words(ws1, ws2, 8), cut_message("fail 5"));
  ws2[0] = 0x12345678;

  ws2[1] = 0x00000000;
  cut_assert_true(sy_equal_words(ws1, ws2, 4), cut_message("fail 6"));
  cut_assert_false(sy_equal_words(ws1, ws2, 8), cut_message("fail 7"));
}

void
test_rotl_word()
{
  cut_assert_equal_word(0xde5840ee,
      sy_rotl_word(0x6f2c2077, 1), cut_message("unexpected bytes"));

  cut_assert_equal_word(0x02020001,
      sy_rotl_word(0x81010000, 1), cut_message("unexpected bytes"));

  cut_assert_equal_word(0xa8dbe0eb,
      sy_rotl_word(0xa36f83ae, 30), cut_message("unexpected bytes"));
}

void
test_rotr_word()
{
  cut_assert_equal_word(0x091a2b3c,
      sy_rotr_word(0x12345678, 1), cut_message("unexpected bytes"));

  cut_assert_equal_word(0x048d159e,
      sy_rotr_word(0x12345678, 2), cut_message("unexpected bytes"));

  cut_assert_equal_word(0x02468acf,
      sy_rotr_word(0x12345678, 3), cut_message("unexpected bytes"));
}

void
test_copy_words()
{
  sy_word a[2], e[] = {0x12345678, 0x9abcdef0};
  sy_word w[] = {0x12345678, 0x9abcdef0};

  sy_copy_words(a, w, 8);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[1] = 0xffffffff;
  e[1] = 0x9abcdeff;
  sy_copy_words(a, w, 7);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[1] = 0xffffffff;
  e[1] = 0x9abcffff;
  sy_copy_words(a, w, 6);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[1] = 0xffffffff;
  e[1] = 0x9affffff;
  sy_copy_words(a, w, 5);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff;
  a[1] = 0xffffffff;
  e[0] = 0x12345678;
  e[1] = 0x9abcdef0;
  sy_copy_words(a, w, 4);
  cut_assert(e[0] == a[0] && e[1] != a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff;
  e[0] = 0x123456ff;
  sy_copy_words(a, w, 3);
  cut_assert(e[0] == a[0] && e[1] != a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff;
  e[0] = 0x1234ffff;
  sy_copy_words(a, w, 2);
  cut_assert(e[0] == a[0] && e[1] != a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff;
  e[0] = 0x12ffffff;
  sy_copy_words(a, w, 1);
  cut_assert(e[0] == a[0] && e[1] != a[1], cut_message("unexpected word"));
}

