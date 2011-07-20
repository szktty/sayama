#include <stdarg.h>
#include "sayama/utils.h"

void
sy_debug(const char *format, ...)
{
  va_list ap;

  printf("# DEBUG: ");
  va_start(ap, format);
  vprintf(format, ap);
  va_end(ap);
}

void
sy_trace(const char *format, ...)
{
  va_list ap;

  printf("# TRACE: ");
  va_start(ap, format);
  vprintf(format, ap);
  va_end(ap);
}

void
sy_print_bytes(uint8_t *bytes, size_t len, unsigned int options)
{
  sy_fprint_bytes(stdout, bytes, len, options);
}

void
sy_fprint_bytes(FILE *f, uint8_t *bytes, size_t len,
    unsigned int options)
{
  size_t i, per, line;
  uint8_t v;

  if (options & SY_PRINT_BIN)
    per = 8;
  else
    per = 16;

  line = 0;
  for (i = 0; i < len; i++) {
    v = bytes[i];

    if (!(options & SY_PRINT_QUIET) && i % per == 0) {
      fprintf(f, "#   ");
      if (options & SY_PRINT_INDEX)
        fprintf(f, "%04lu: ", i / per * per);
    }

    if (options & SY_PRINT_BIN)
      fprintf(f, "%08u%08u%08u%08u%08u%08u%08u%08u", v >> 7 & 0x01,
          v >> 6 & 0x01, v >> 5 & 0x01, v >> 4 & 0x01, v >> 3 & 0x01,
          v >> 2 & 0x01, v >> 1 & 0x01, v & 0x01);
    else if (options & SY_PRINT_OCT)
      fprintf(f, "%03o", v);
    else if (options & SY_PRINT_DEC)
      fprintf(f, "%03u", v);
    else
      fprintf(f, "%02x", v);

    if ((options & SY_PRINT_NL) && (i+1) < len && (i+1) % per == 0)
      fprintf(f, "\n");
    else if ((options & SY_PRINT_SP) && i+1 < len)
      fprintf(f, " ");
  }
  fprintf(f, "\n");
}

void
sy_dump_bytes(const char *path, uint8_t *bytes,
    size_t len, unsigned int options)
{
  FILE *f;

  f = fopen(path, "w");
  if (f != NULL) {
    sy_fprint_bytes(f, bytes, len, options);
    fclose(f);
  } else
    printf("# error: %s: cannot open - %s\n", __func__, path);
}

_Bool
sy_load_bytes(uint8_t *dest, const char *path, size_t len)
{
  uint8_t v;
  FILE *fp = NULL;
  size_t i;
  const char *e;
  char buf;
  _Bool islow = false;

  fp = fopen(path, "r");
  if (fp == NULL) {
    e = "no such file";
    goto error;
  }

  for (i = 0; i < len;) {
    if (fread(&buf, 1, 1, fp) != 1) {
      e = "eof";
      goto error;
    }

    switch (buf) {
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
      v = buf - '0';
      break;
    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
      v = buf - 'a' + 10;
      break;
    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
      v = buf - 'A' + 10;
      break;
    case ' ': case '\t': case '\r': case '\n':
      /* ignore */
      continue;
    default:
      e = "unknown char";
      goto error;
    }

    if (islow) {
      dest[i] = dest[i] << 4 | v;
      i++;
    } else
      dest[i] = v;
    islow = !islow;
  }
  fclose(fp);
  return true;

error:
  if (fp != NULL)
    fclose(fp);
  return false;
}

