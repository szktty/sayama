#ifndef __SY_UTILS_H__
#define __SY_UTILS_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef void (*sy_buffer_operator)(uint8_t *buf, size_t len,
    void *context);

#ifdef DEBUG
#define debug                   sy_debug
#define debug_bytes(bs, l, os)  sy_print_bytes((bs), (l), (os))
#else
#define debug(f, ...)
#define debug_bytes(bs, l, os)
#endif

#ifdef TRACE
#define trace                   sy_trace
#define trace_bytes(bs, l, os)  sy_print_bytes((bs), (l), (os))
#else
#define trace(f, ...)
#define trace_bytes(bs, l, os)
#endif

/* sy_print_bytes() options */
enum {
  /* base */
  SY_PRINT_BIN    = 1,        /* binary */
  SY_PRINT_OCT    = 1 << 1,   /* octal */
  SY_PRINT_DEC    = 1 << 2,   /* decimal */
  SY_PRINT_HEX    = 1 << 3,   /* hexdecimal */

  /* format */
  SY_PRINT_QUIET  = 1 << 4,   /* print no information */
  SY_PRINT_SP_SEP = 1 << 5,   /* separate each byte by space */
  SY_PRINT_NL_SEP = 1 << 6,   /* separate bytes by new line */
  SY_PRINT_INDEX  = 1 << 7,   /* print first index in each line */
  SY_PRINT_NL     = 1 << 8,   /* print new line at end */

  /* compound options */
  SY_PRINT_VERBOSE = SY_PRINT_NL_SEP | SY_PRINT_SP_SEP |
    SY_PRINT_INDEX | SY_PRINT_NL
};

extern void sy_debug(const char *format, ...);
extern void sy_trace(const char *format, ...);

extern void sy_print_bytes(uint8_t *bytes, size_t len,
    unsigned int options);
extern void sy_fprint_bytes(FILE *f, uint8_t *bytes, size_t len,
    unsigned int options);
extern void sy_dump_bytes(const char *path, uint8_t *bytes,
    size_t len, unsigned int options);

extern bool sy_load_bytes(uint8_t *dest, const char *path, size_t len);

extern size_t sy_append_to_buffer(uint8_t *buf, size_t buf_len,
    size_t max_buf_len, const uint8_t *bytes, size_t len,
    sy_buffer_operator op, void *context);

#ifdef __cplusplus
}
#endif

#endif /* __SY_UTILS_H__ */

