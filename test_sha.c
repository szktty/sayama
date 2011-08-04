#include "sayama/sha.h"
#include "sayama/utils.h"
#include "cut-extends.h"
#include "sha.h"

void test_sha1();

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

