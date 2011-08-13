#include <cutter/cutter.h>
#include <string.h>

#define cut_assert_equal_bytes(expected, actual, len, ...) \
  do { \
    cut_assert_true(memcmp(expected, actual, len) == 0, __VA_ARGS__); \
  } while (0)

#define cut_assert_not_equal_bytes(expected, actual, len, ...) \
  do { \
    cut_assert_false(memcmp(expected, actual, len) == 0, __VA_ARGS__); \
  } while (0)

#define cut_assert_equal_word cut_assert_equal_uint_fast32

