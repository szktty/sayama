#include "sayama/dword.h"
#include "sayama/utils.h"
#include "cut-extends.h"

void test_dword_get(void);
void test_dword_set(void);
void test_encode_dwords(void);

void
test_dword_get()
{
  sy_dword w = 0x0123456789abcdef;

  cut_assert_equal_uint(0x01, sy_dword_get(w, 0));
  cut_assert_equal_uint(0x23, sy_dword_get(w, 1));
  cut_assert_equal_uint(0x45, sy_dword_get(w, 2));
  cut_assert_equal_uint(0x67, sy_dword_get(w, 3));
  cut_assert_equal_uint(0x89, sy_dword_get(w, 4));
  cut_assert_equal_uint(0xab, sy_dword_get(w, 5));
  cut_assert_equal_uint(0xcd, sy_dword_get(w, 6));
  cut_assert_equal_uint(0xef, sy_dword_get(w, 7));
}

void
test_dword_set()
{
  sy_dword w = 0x0123456789abcdef;

  cut_assert_equal_dword(0x2823456789abcdef, sy_dword_set(w, 0, 0x28));
  cut_assert_equal_dword(0x0135456789abcdef, sy_dword_set(w, 1, 0x35));
  cut_assert_equal_dword(0x0123aa6789abcdef, sy_dword_set(w, 2, 0xaa));
  cut_assert_equal_dword(0x012345aa89abcdef, sy_dword_set(w, 3, 0xaa));
  cut_assert_equal_dword(0x01234567aaabcdef, sy_dword_set(w, 4, 0xaa));
  cut_assert_equal_dword(0x0123456789aacdef, sy_dword_set(w, 5, 0xaa));
  cut_assert_equal_dword(0x0123456789abaaef, sy_dword_set(w, 6, 0xaa));
  cut_assert_equal_dword(0x0123456789abcdaa, sy_dword_set(w, 7, 0xaa));
}

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

