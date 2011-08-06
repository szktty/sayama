#include "sayama/word.h"
#include "sayama/utils.h"
#include "cut-extends.h"

void test_word_value(void);
void test_land_word1(void);
void test_land_word2(void);
void test_land_words(void);
void test_lor_word1(void);
void test_lor_word2(void);
void test_lor_words(void);
void test_lxor_word(void);
void test_lxor_words(void);
void test_invert_word(void);
void test_invert_words(void);
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
test_land_word1()
{
  uint8_t s[] = {0,1,2,3};
  uint8_t d[] = {8,9,10,11};
  uint8_t e[] = {0,1,2,3};

  sy_land_word(d, d, s);
  cut_assert_equal_bytes(e, d, 4, cut_message("land failed"));
}

void
test_land_word2()
{
  uint8_t s[] = {0,1,2,3};
  uint8_t d[] = {1,0,2,1};
  uint8_t e[] = {0,0,2,1};

  sy_land_word(d, d, s);
  cut_assert_equal_bytes(e, d, 4, cut_message("land failed"));
}

void
test_land_words()
{
  uint8_t s[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
  uint8_t d[] = {8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7};
  uint8_t e[] = {0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7};

  sy_land_words(d, d, s, 4);
  cut_assert_equal_bytes(e, d, 16, cut_message("land failed"));
}

void
test_lor_word1()
{
  uint8_t s[] = {0,1,2,3};
  uint8_t d[] = {8,9,10,11};
  uint8_t e[] = {8,9,10,11};

  sy_lor_word(d, d, s);
  cut_assert_equal_bytes(e, d, 4, cut_message("lor failed"));
}

void
test_lor_word2()
{
  uint8_t s[] = {0x12, 0x34, 0x56, 0x78};
  uint8_t d[] = {0x0f, 0xed, 0xcb, 0xa9};
  uint8_t e[] = {0x1f, 0xfd, 0xdf, 0xf9};

  sy_lor_word(d, d, s);
  cut_assert_equal_bytes(e, d, 4, cut_message("lor failed"));
}

void
test_lor_words()
{
  uint8_t s[] = {0, 1, 2, 3, 0x12, 0x34, 0x56, 0x78};
  uint8_t d[] = {8, 9, 10, 11, 0x0f, 0xed, 0xcb, 0xa9};
  uint8_t e[] = {8, 9, 10, 11, 0x1f, 0xfd, 0xdf, 0xf9};

  sy_lor_words(d, d, s, 2);
  cut_assert_equal_bytes(e, d, 8, cut_message("lor failed"));
}

void
test_lxor_word()
{
  uint8_t s[] = {0,1,2,3};
  uint8_t d[] = {8,9,10,11};
  uint8_t e[] = {8,8,8,8};

  sy_lxor_word(d, d, s);
  cut_assert_equal_bytes(e, d, 4, cut_message("lxor failed"));
}

void
test_lxor_words()
{
  uint8_t s[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
  uint8_t d[] = {8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7};
  uint8_t e[] = {8,8,8,8,8,8,8,8};

  sy_lxor_words(d, d, s, 2);
  cut_assert_equal_bytes(e, d, 8, cut_message("lxor failed"));
}

void
test_invert_word()
{
  uint8_t s[] = {0,1,2,3};
  uint8_t d[4];
  uint8_t e[] = {255,254,253,252};

  sy_invert_word(d, s);
  cut_assert_equal_bytes(e, d, 4, cut_message("invert failed"));
}

void
test_invert_words()
{
  uint8_t s[] = {0,1,2,3,4,5,6,7};
  uint8_t d[4];
  uint8_t e[] = {255,254,253,252,251,250,249,248};

  sy_invert_words(d, s, 2);
  cut_assert_equal_bytes(e, d, 8, cut_message("invert failed"));
}

void
test_rotl_word()
{
  uint8_t s[] = {0x6f, 0x2c, 0x20, 0x77};
  uint8_t e[] = {0xde, 0x58, 0x40, 0xee};
  uint8_t a[4];

  sy_rotl_word(a, s, 1);
  cut_assert_equal_bytes(e, a, 4, cut_message("unexpected bytes"));

  sy_set_word_value(s, 0x81010000);
  sy_set_word_value(e, 0x02020001);
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

