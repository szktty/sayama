#include <stdint.h>
#include "sayama/memory.h"
#include "cut-extends.h"

void test_memory_memset(void);
void test_memory_memzero(void);
void test_memory_memmove(void);

void
test_memory_memset()
{
  uint8_t buf[8] = {0,1,2,3,4,5,6,7};
  uint8_t ex[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

  sy_memset(buf, 0xff, 8);
  cut_assert_equal_bytes(ex, buf, 8);
}

void
test_memory_memzero()
{
  uint8_t buf[8] = {0,1,2,3,4,5,6,7};
  uint8_t ex[8] = {0,0,0,0,0,0,0,0};

  sy_memzero(buf, 8);
  cut_assert_equal_bytes(ex, buf, 8);
}

void
test_memory_memmove()
{
  uint8_t buf1[8] = {0,1,2,3,4,5,6,7};
  uint8_t buf2[8] = {0,1,2,3,4,5,6,7};
  uint8_t buf3[8] = {7,6,5,4,3,2,1,0};
  uint8_t ex1[8] = {0,1,0,1,2,3,6,7};
  uint8_t ex2[8] = {0,1,7,6,5,4,6,7};

  sy_memmove(buf1+2, buf1, 4);
  cut_assert_equal_bytes(ex1, buf1, 8,
      cut_message("copying bytes to overlapped block failed"));

  sy_memmove(buf2+2, buf3, 4);
  cut_assert_equal_bytes(ex2, buf2, 8,
      cut_message("copying bytes to non-overlapped block failed"));
}

