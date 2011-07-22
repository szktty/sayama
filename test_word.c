#include "sayama/word.h"
#include "cut-extends.h"

void test_word_value(void);
void test_xor_words(void);
void test_rotl_word(void);
void test_add_word(void);
void test_copy_words(void);
void test_copy_word(void);

void
test_word_value()
{
  uint8_t w[] = { 0x12, 0x34, 0x56, 0x78 };
  uint8_t w2[] = { 0, 0, 0, 0 };
  uint32_t a, e = 0x12345678;

  a = sy_word_value(w);
  cut_assert(e == a, cut_message("unexpected value"));

  sy_set_word_value(w2, a);
  cut_assert_equal_bytes(w, w2, 4, cut_message("unexpected word"));
}

void
test_xor_words()
{
  uint8_t s[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
  uint8_t d[] = {8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7};
  uint8_t e[] = {8,8,8,8,8,8,8,8};

  sy_xor_words(d, d, s, 2);
  cut_assert_equal_bytes(e, d, 8, cut_message("xor failed"));
}

void
test_rotl_word()
{
  uint8_t s[] = {0x6f, 0x2c, 0x20, 0x77};
  uint8_t e[] = {0xde, 0x58, 0x40, 0xee};
  uint8_t a[4];

  sy_rotl_word(a, s, 1);
  cut_assert_equal_bytes(e, a, 4, cut_message("unexpected bytes"));
}

void
test_add_word()
{
  uint8_t s1[] = {0x83, 0x64, 0x06, 0xbf};
  uint8_t s2[] = {0xac, 0x50, 0x78, 0x3d};
  uint8_t a[4], e[] = {0x2f, 0xb4, 0x7e, 0xfc};

  sy_add_word(a, s1, s2);
  cut_assert_equal_bytes(e, a, 4, cut_message("unexpected bytes"));
}

void
test_copy_words()
{
  uint8_t s[4] = {0,1,2,3};
  uint8_t d[4] = {0,0,0,0};

  sy_copy_words(d, s, 1);
  cut_assert_equal_bytes(s, d, 4, cut_message("copy failed"));
}

void
test_copy_word()
{
  uint8_t s[4] = {0,1,2,3};
  uint8_t a[] = {0,0,0,0,0,0,0,0};
  uint8_t e[] = {0,1,0,1,2,3,0,0};

  sy_copy_word(a, s);
  cut_assert_equal_bytes(s, a, 4, cut_message("copy failed"));

  sy_copy_word(a+2, a);
  cut_assert_equal_bytes(e, a, 8, cut_message("overlap copy failed"));
}

