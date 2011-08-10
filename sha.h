#ifndef __SY_SHA_PRIVATE_H__
#define __SY_SHA_PRIVATE_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

extern void _sy_sha1_hash_block(uint8_t *block, const uint8_t *data);
extern void _sy_sha1_f(uint8_t *dest, unsigned int t,
    const uint8_t *b, const uint8_t *c, const uint8_t *d);

extern void _sy_sha256_sigma0(uint8_t *buf, const uint8_t *w);
extern void _sy_sha256_sigma1(uint8_t *buf, const uint8_t *w);
extern void _sy_sha256_ch(uint8_t *buf, const uint8_t *w1,
    const uint8_t *w2, const uint8_t *w3);
extern void _sy_sha256_maj(uint8_t *buf, const uint8_t *w1,
    const uint8_t *w2, const uint8_t *w3);
extern void _sy_sha256_sum0(uint8_t *buf, const uint8_t *w);
extern void _sy_sha256_sum1(uint8_t *buf, const uint8_t *w);

#ifdef __cplusplus
}
#endif

#endif /* __SY_SHA_H__ */

