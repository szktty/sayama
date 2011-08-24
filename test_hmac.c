#include "sayama/digest.h"
#include "sayama/hmac.h"
#include "sayama/sha.h"
#include "sayama/word.h"
#include "sayama/utils.h"
#include "cut-extends.h"

void test_hmac_sha1(void);
void test_hmac_copy(void);

void
test_hmac_sha1()
{
  const char *key = "hello", *msg = "world";
  uint8_t a[200], e[200];
  sy_hmac_context context;

  sy_load_bytes(e, "test_hmac_sha1_digest.txt", 20);
  sy_hmac_init(&context, (uint8_t *)key, strlen(key), &sy_digester_sha1);
  sy_hmac_update(&context, (uint8_t *)msg, strlen(msg));
  sy_hmac_final(&context, a);
  cut_assert_equal_bytes(e, a, 20, cut_message("unexpected digest"));
}

void
test_hmac_copy()
{
  const char *key = "hello", *msg = "world";
  uint8_t a[200], e[200];
  sy_hmac_context c1, c2;

  sy_load_bytes(e, "test_hmac_sha1_digest.txt", 20);
  sy_hmac_init(&c1, (uint8_t *)key, strlen(key), &sy_digester_sha1);
  sy_hmac_update(&c1, (uint8_t *)msg, strlen(msg));
  sy_hmac_copy(&c2, &c1);
  sy_hmac_final(&c1, a);
  cut_assert_equal_bytes(e, a, 20, cut_message("unexpected digest"));

  memset(a, 0, 20);
  sy_hmac_final(&c2, a);
  cut_assert_equal_bytes(e, a, 20, cut_message("unexpected digest"));
}

