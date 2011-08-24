#include "sayama/bytes.h"
#include "sayama/utils.h"
#include "cut-extends.h"

void test_hexify_bytes(void);

void
test_hexify_bytes(void)
{
  uint8_t bytes[] = {0, 1, 128, 3, 187, 5, 245, 7};
  char buf[100];

  memset(buf, 0, 100);
  sy_hexify_bytes(buf, bytes, 0, 7);
  cut_assert_equal_string("00018003bb05f507", buf);

  memset(buf, 0, 100);
  sy_hexify_bytes(buf, bytes, 2, 5);
  cut_assert_equal_string("8003bb05", buf);
}

