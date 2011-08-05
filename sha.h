#ifndef __SY_SHA_PRIVATE_H__
#define __SY_SHA_PRIVATE_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

extern void _sy_sha1_hash_block(uint8_t *state, const uint8_t *block);
extern void _sy_sha1_f(uint8_t *dest, unsigned int t,
    const uint8_t *b, const uint8_t *c, const uint8_t *d);

#ifdef __cplusplus
}
#endif

#endif /* __SY_SHA_H__ */

