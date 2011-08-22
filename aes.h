#ifndef __SY_AES_PRIVATE_H__
#define __SY_AES_PRIVATE_H__

#include "sayama/word.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern unsigned int _sy_aes_expand_key(sy_word *dest,
    const uint8_t *key, SY_AES_KEYLEN keylen);
extern void _sy_aes_mix_columns(sy_word *block);
extern uint8_t _sy_aes_mul2(uint8_t v);

#ifdef __cplusplus
}
#endif

#endif /* __SY_AES_PRIVATE_H__ */

