#include "sayama/large_int.h"
#include "sayama/utils.h"
#include "cut-extends.h"

void test_init_large_int_128(void);

void
test_init_large_int_128()
{
  sy_large_int_128 v;

  sy_init_large_int_128(&v);
  cut_assert_equal_boolean(v.hdr.sign, true);
  cut_assert_equal_uint(128/SY_DIGIT_BITS, v.hdr.len);
}

