#include "sayama/word.h"
#include "sayama/utils.h"
#include "cut-extends.h"

void test_encode_words(void);
void test_decode_words(void);
void test_equal_words(void);
void test_rotl_word(void);
void test_rotr_word(void);
void test_copy_words(void);
void test_fill_words1(void);
void test_fill_words2(void);
void test_fill_words3(void);
void test_fill_words4(void);
void test_clear_words(void);

void
test_encode_words()
{
  uint8_t w[] = { 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0 };
  sy_word a[2], e[] = {0x12345678, 0x9abcdef0};

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  sy_encode_words(a, 0, w, 8);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[1] = 0x9abcdeff;
  sy_encode_words(a, 0, w, 7);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[1] = 0x9abcffff;
  sy_encode_words(a, 0, w, 6);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[1] = 0x9affffff;
  sy_encode_words(a, 0, w, 5);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0x12345678;
  sy_encode_words(a, 0, w, 4);
  cut_assert(e[0] == a[0], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0x123456ff;
  sy_encode_words(a, 0, w, 3);
  cut_assert(e[0] == a[0], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0x1234ffff;
  sy_encode_words(a, 0, w, 2);
  cut_assert(e[0] == a[0], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0x12ffffff;
  sy_encode_words(a, 0, w, 1);
  cut_assert(e[0] == a[0], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0xffffff12; e[1] = 0x3456ffff;
  sy_encode_words(a, 3, w, 3);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0xffffffff; e[1] = 0xff123456;
  sy_encode_words(a, 5, w, 3);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0xff123456; e[1] = 0x789affff;
  sy_encode_words(a, 1, w, 5);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0xff1234ff; e[1] = 0xffffffff;
  sy_encode_words(a, 1, w, 2);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));
}

void
test_decode_words()
{
  uint8_t a[8], e[] = { 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0 };
  sy_word w[] = {0x12345678, 0x9abcdef0};

  memset(a, 0xff, 8);
  sy_decode_words(a, w, 0, 7);
  cut_assert_equal_bytes(e, a, 8, cut_message("unexpected word"));

  memset(a, 0xff, 8);
  e[7] = 0xff;
  sy_decode_words(a, w, 0, 6);
  cut_assert_equal_bytes(e, a, 8, cut_message("unexpected word"));

  memset(a, 0xff, 8);
  e[6] = 0xff;
  sy_decode_words(a, w, 0, 5);
  cut_assert_equal_bytes(e, a, 8, cut_message("unexpected word"));

  memset(a, 0xff, 8);
  e[5] = 0xff;
  sy_decode_words(a, w, 0, 4);
  cut_assert_equal_bytes(e, a, 8, cut_message("unexpected word"));

  memset(a, 0xff, 8);
  e[4] = 0xff;
  sy_decode_words(a, w, 0, 3);
  cut_assert_equal_bytes(e, a, 8, cut_message("unexpected word"));

  memset(a, 0xff, 8);
  e[3] = 0xff;
  sy_decode_words(a, w, 0, 2);
  cut_assert_equal_bytes(e, a, 8, cut_message("unexpected word"));

  memset(a, 0xff, 8);
  e[2] = 0xff;
  sy_decode_words(a, w, 0, 1);
  cut_assert_equal_bytes(e, a, 8, cut_message("unexpected word"));

  memset(a, 0xff, 8); memset(e, 0xff, 8);
  e[0] = 0x12;
  sy_decode_words(a, w, 0, 0);
  cut_assert_equal_bytes(e, a, 8, cut_message("unexpected word"));

  memset(a, 0xff, 8); memset(e, 0xff, 8);
  e[0] = 0x34; e[1] = 0x56; e[2] = 0x78;
  sy_decode_words(a, w, 1, 3);
  cut_assert_equal_bytes(e, a, 8, cut_message("unexpected word"));

  memset(a, 0xff, 8); memset(e, 0xff, 8);
  e[0] = 0x34; e[1] = 0x56; e[2] = 0x78; e[3] = 0x9a; e[4] = 0xbc;
  sy_decode_words(a, w, 1, 5);
  cut_assert_equal_bytes(e, a, 8, cut_message("unexpected word"));

  memset(a, 0xff, 8); memset(e, 0xff, 8);
  e[0] = 0x34; e[1] = 0x56; e[2] = 0x78; e[3] = 0x9a; e[4] = 0xbc;
  e[5] = 0xde; e[6] = 0xf0;
  sy_decode_words(a, w, 1, 7);
  cut_assert_equal_bytes(e, a, 8, cut_message("unexpected word"));

  memset(a, 0xff, 8); memset(e, 0xff, 8);
  e[0] = 0xbc; e[1] = 0xde; e[2] = 0xf0;
  sy_decode_words(a, w, 5, 7);
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
  cut_assert_true(sy_equal_words(ws1, 0, ws2, 0, 8), cut_message("fail 1"));
  cut_assert_true(sy_equal_words(ws1, 3, ws2, 3, 3), cut_message("fail 1"));
  cut_assert_true(sy_equal_words(ws1, 5, ws2, 5, 3), cut_message("fail 1"));
  cut_assert_true(sy_equal_words(ws1, 7, ws2, 7, 1), cut_message("fail 1"));
  cut_assert_false(sy_equal_words(ws1, 0, ws2, 1, 1), cut_message("fail 1"));
  cut_assert_false(sy_equal_words(ws1, 0, ws2, 1, 3), cut_message("fail 1"));
  cut_assert_false(sy_equal_words(ws1, 3, ws2, 1, 3), cut_message("fail 1"));
  cut_assert_false(sy_equal_words(ws1, 3, ws2, 5, 3), cut_message("fail 1"));

  ws1[0] = 0x00000000;
  cut_assert_false(sy_equal_words(ws1, 0, ws2, 0, 5), cut_message("fail 2"));
  cut_assert_false(sy_equal_words(ws1, 0, ws2, 0, 8), cut_message("fail 2"));
  cut_assert_false(sy_equal_words(ws1, 3, ws2, 3, 3), cut_message("fail 2"));
  cut_assert_true(sy_equal_words(ws1, 4, ws2, 4, 4), cut_message("fail 2"));
  ws1[0] = 0x12345678;

  ws1[1] = 0x00000000;
  cut_assert_true(sy_equal_words(ws1, 0, ws2, 0, 4), cut_message("fail 3"));
  cut_assert_false(sy_equal_words(ws1, 0, ws2, 0, 8), cut_message("fail 4"));
  ws1[1] = 0x12345678;

  ws2[0] = 0x00000000;
  cut_assert_false(sy_equal_words(ws1, 0, ws2, 0, 8), cut_message("fail 5"));
  ws2[0] = 0x12345678;

  ws2[1] = 0x00000000;
  cut_assert_true(sy_equal_words(ws1, 0, ws2, 0, 4), cut_message("fail 6"));
  cut_assert_false(sy_equal_words(ws1, 0, ws2, 0, 8), cut_message("fail 7"));
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

  sy_copy_words(a, 0, w, 0, 7);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[1] = 0xffffffff;
  e[1] = 0x9abcdeff;
  sy_copy_words(a, 0, w, 0, 6);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[1] = 0xffffffff;
  e[1] = 0x9abcffff;
  sy_copy_words(a, 0, w, 0, 5);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[1] = 0xffffffff;
  e[1] = 0x9affffff;
  sy_copy_words(a, 0, w, 0, 4);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0x12345678; e[1] = 0xffffffff;
  sy_copy_words(a, 0, w, 0, 3);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0x123456ff; e[1] = 0xffffffff;
  sy_copy_words(a, 0, w, 0, 2);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff;
  e[0] = 0x1234ffff;
  sy_copy_words(a, 0, w, 0, 1);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff;
  e[0] = 0x12ffffff;
  sy_copy_words(a, 0, w, 0, 0);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0xff1234ff; e[1] = 0xffffffff;
  sy_copy_words(a, 1, w, 0, 1);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0xff123456; e[1] = 0x78ffffff;
  sy_copy_words(a, 1, w, 0, 3);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0xffffff56; e[1] = 0x78ffffff;
  sy_copy_words(a, 3, w, 2, 3);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0xffffffff; e[1] = 0xff5678ff;
  sy_copy_words(a, 5, w, 2, 3);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));
}

void
test_fill_words1()
{
  sy_word a[2], e[2];

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0x00ffffff; e[1] = 0xffffffff;
  sy_fill_words(a, 0, 0, 0);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0x0000ffff; e[1] = 0xffffffff;
  sy_fill_words(a, 0, 0, 1);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0x000000ff; e[1] = 0xffffffff;
  sy_fill_words(a, 0, 0, 2);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0xff00ffff; e[1] = 0xffffffff;
  sy_fill_words(a, 0, 1, 1);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0xff0000ff; e[1] = 0xffffffff;
  sy_fill_words(a, 0, 1, 2);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0xff000000; e[1] = 0xffffffff;
  sy_fill_words(a, 0, 1, 3);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0xffff00ff; e[1] = 0xffffffff;
  sy_fill_words(a, 0, 2, 2);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0xffff0000; e[1] = 0xffffffff;
  sy_fill_words(a, 0, 2, 3);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0xffff0000; e[1] = 0x00ffffff;
  sy_fill_words(a, 0, 2, 4);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0xffffff00; e[1] = 0xffffffff;
  sy_fill_words(a, 0, 3, 3);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0xffffff00; e[1] = 0x00ffffff;
  sy_fill_words(a, 0, 3, 4);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0xffffff00; e[1] = 0x0000ffff;
  sy_fill_words(a, 0, 3, 5);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));
}

void
test_fill_words2()
{
  sy_word a[3], e[3];

  a[0] = 0xffffffff; a[1] = 0xffffffff; a[2] = 0xffffffff;
  e[0] = 0xffffffff; e[1] = 0x00ffffff; e[2] = 0xffffffff;
  sy_fill_words(a, 0, 4, 4);
  cut_assert(sy_equal_words(e, 0, a, 0, 12), cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff; a[2] = 0xffffffff;
  e[0] = 0xffffffff; e[1] = 0xff000000; e[2] = 0xffffffff;
  sy_fill_words(a, 0, 5, 7);
  cut_assert(sy_equal_words(e, 0, a, 0, 12), cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff; a[2] = 0xffffffff;
  e[0] = 0xffffffff; e[1] = 0xffff0000; e[2] = 0x00ffffff;
  sy_fill_words(a, 0, 6, 8);
  cut_assert(sy_equal_words(e, 0, a, 0, 12), cut_message("unexpected word"));
}

void
test_fill_words3()
{
  sy_word a[2], e[2];

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0; e[1] = 0;
  sy_fill_words(a, 0, 0, 7);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0; e[1] = 0x000000ff;;
  sy_fill_words(a, 0, 0, 6);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0; e[1] = 0x0000ffff;;
  sy_fill_words(a, 0, 0, 5);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0; e[1] = 0x00ffffff;
  sy_fill_words(a, 0, 0, 4);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0; e[1] = 0xffffffff;
  sy_fill_words(a, 0, 0, 3);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0x000000ff;
  sy_fill_words(a, 0, 0, 2);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0x0000ffff;;
  sy_fill_words(a, 0, 0, 1);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0x00ffffff;
  sy_fill_words(a, 0, 0, 0);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0xff000000; e[1] = 0x00ffffff;
  sy_fill_words(a, 0, 1, 4);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0xffff0000; e[1] = 0x0000ffff;
  sy_fill_words(a, 0, 2, 5);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0xffffff00; e[1] = 0x000000ff;
  sy_fill_words(a, 0, 3, 6);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0xffffffff; e[1] = 0x000000ff;
  sy_fill_words(a, 0, 4, 6);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff; a[1] = 0xffffffff;
  e[0] = 0xffffffff; e[1] = 0x00000000;
  sy_fill_words(a, 0, 4, 7);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));
}

void
test_fill_words4()
{
  sy_word a[3], e[3];

  a[0] = 0xffffffff; a[1] = 0xffffffff; a[2] = 0xffffffff;
  e[0] = 0xff000000; e[1] = 0x00000000; e[2] = 0x00ffffff;
  sy_fill_words(a, 0, 1, 8);
  cut_assert(e[0] == a[0] && e[1] == a[1] && e[2] == a[2]);
}

void
test_clear_words()
{
  sy_word a[2], e[2];

  a[0] = 0xffffffff;
  a[1] = 0xffffffff;
  e[0] = 0;
  e[1] = 0;
  sy_clear_words(a, 0, 7);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff;
  a[1] = 0xffffffff;
  e[0] = 0;
  e[1] = 0x000000ff;;
  sy_clear_words(a, 0, 6);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff;
  a[1] = 0xffffffff;
  e[0] = 0;
  e[1] = 0x0000ffff;;
  sy_clear_words(a, 0, 5);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff;
  a[1] = 0xffffffff;
  e[0] = 0;
  e[1] = 0x00ffffff;;
  sy_clear_words(a, 0, 4);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff;
  a[1] = 0xffffffff;
  e[0] = 0;
  e[1] = 0xffffffff;
  sy_clear_words(a, 0, 3);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff;
  a[1] = 0xffffffff;
  e[0] = 0x000000ff;
  sy_clear_words(a, 0, 2);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff;
  a[1] = 0xffffffff;
  e[0] = 0x0000ffff;;
  sy_clear_words(a, 0, 1);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));

  a[0] = 0xffffffff;
  a[1] = 0xffffffff;
  e[0] = 0x00ffffff;
  sy_clear_words(a, 0, 0);
  cut_assert(e[0] == a[0] && e[1] == a[1], cut_message("unexpected word"));
}

