#include "sayama/dword.h"
#include "sayama/utils.h"
#include "cut-extends.h"

void test_encode_dwords(void);

void
test_encode_dwords()
{
  uint8_t w[] = { 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
    0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
  };
  sy_dword a[2], e[2];

  a[0] = 0; a[1] = 0;
  sy_encode_dwords(a, w, 1);
  cut_assert_equal_dword(0x123456789abcdef0, a[0]);
  cut_assert_equal_dword(0, a[1]);

  a[0] = 0; a[1] = 0;
  sy_encode_dwords(a, w, 2);
  cut_assert_equal_dword(0x123456789abcdef0, a[0]);
  cut_assert_equal_dword(0x123456789abcdef0, a[1]);
}

