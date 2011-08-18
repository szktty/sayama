#ifndef __SY_SHA_PRIVATE_H__
#define __SY_SHA_PRIVATE_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

extern void _sy_sha1_hash_block(sy_word *block, const uint8_t *data);
extern sy_word _sy_sha1_f(unsigned int t, sy_word b, sy_word c, sy_word d);

extern sy_word _sy_sha256_sigma0(sy_word w);
extern sy_word _sy_sha256_sigma1(sy_word w);
extern sy_word _sy_sha256_ch(sy_word w1, sy_word w2, sy_word w3);
extern sy_word _sy_sha256_maj(sy_word w1, sy_word w2, sy_word w3);
extern sy_word _sy_sha256_sum0(sy_word w);
extern sy_word _sy_sha256_sum1(sy_word w);

#ifdef __cplusplus
}
#endif

#endif /* __SY_SHA_H__ */

