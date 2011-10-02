#include "sayama/large_int.h"

size_t
sy_large_int_get_str(char *s, unsigned int base,
      const sy_large_int_header *hdr, const SY_DIGIT *d)
{
  size_t len = 0;

  // TODO
  if (!hdr->sign) {
    if (s != NULL)
      *s++ = '-';
    len++;
  }
  return len;
}

