#include "sayama/large_int.h"
#include "sayama/utils.h"
#include "cut-extends.h"

void test_init_large_int_128(void);

void
test_init_large_int_128()
{
  sy_large_int_128 v;
  char s[20];

  s[0] = '\0';
  sy_init_large_int_128(&v);
  sy_large_int_128_get_str(s, 10, &v);
  cut_assert_equal_boolean(v.hdr.sign, true);
  cut_assert_equal_uint(128/SY_DIGIT_BITS, v.hdr.len);
  cut_assert_equal_string("0", s);
}

