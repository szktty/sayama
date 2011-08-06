#include "sayama/sha.h"
#include "sayama/word.h"
#include "sayama/utils.h"
#include "cut-extends.h"
#include "sha.h"

void test_sha1();
void test_sha1_f();

void
test_sha1()
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
test_sha1_f()
{
  unsigned int t;
  uint8_t ac[4], ex[4], b[4], c[4], d[4];

  /* t < 20 */
  t = 3;
  sy_set_word_value(b, 0x5d43e370);
  sy_set_word_value(c, 0xc7ed262c);
  sy_set_word_value(d, 0x59d148c0);
  sy_set_word_value(ex, 0x45d12aa0);
  _sy_sha1_f(ac, t, b, c, d);
  cut_assert_equal_bytes(ex, ac, 4, cut_message("t = %u failed", t));

  /* 20 <= t && t < 40 */
  t = 37;
  sy_set_word_value(b, 0xabbab988);
  sy_set_word_value(c, 0x0e47bc31);
  sy_set_word_value(d, 0x92c4d1f8);
  sy_set_word_value(ex, 0x3739d441);
  _sy_sha1_f(ac, t, b, c, d);
  cut_assert_equal_bytes(ex, ac, 4, cut_message("t = %u failed", t));

  /* 40 <= t && t < 60 */
  t = 45;
  sy_set_word_value(b, 0x291f2d96);
  sy_set_word_value(c, 0x85275613);
  sy_set_word_value(d, 0x8b9e38d1);
  sy_set_word_value(ex, 0x891f3c93);
  _sy_sha1_f(ac, t, b, c, d);
  cut_assert_equal_bytes(ex, ac, 4, cut_message("t = %u failed", t));

  /* 60 <= t && t <= 80 */
  t = 73;
  sy_set_word_value(b, 0xf10f8b24);
  sy_set_word_value(c, 0xa3dd0263);
  sy_set_word_value(d, 0xa7200b4f);
  sy_set_word_value(ex, 0xf5f28208);
  _sy_sha1_f(ac, t, b, c, d);
  cut_assert_equal_bytes(ex, ac, 4, cut_message("t = %u failed", t));
}

