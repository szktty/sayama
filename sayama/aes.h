#ifndef __SY_AES_H__
#define __SY_AES_H__

#include <sayama/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum SY_AES_KEYLEN {
  SY_AES_KEY_128 = 128, 
  SY_AES_KEY_192 = 192,
  SY_AES_KEY_256 = 256
} SY_AES_KEYLEN;

extern bool sy_aes(uint8_t *dest, const uint8_t *key,
    SY_AES_KEYLEN keylen, SY_CIPHER_DIRECTION dir, const uint8_t *iv,
    const uint8_t *text, size_t textlen, SY_BLOCK_CIPHER_MODE mode);
extern bool sy_aes128_encrypt(uint8_t *dest, const uint8_t *key,
    const uint8_t *iv, const uint8_t *text, size_t textlen,
    SY_BLOCK_CIPHER_MODE mode);
extern bool sy_aes128_decrypt(uint8_t *dest, const uint8_t *key,
    const uint8_t *iv, const uint8_t *text, size_t textlen,
    SY_BLOCK_CIPHER_MODE mode);
extern bool sy_aes192_encrypt(uint8_t *dest, const uint8_t *key,
    const uint8_t *iv, const uint8_t *text, size_t textlen,
    SY_BLOCK_CIPHER_MODE mode);
extern bool sy_aes192_decrypt(uint8_t *dest, const uint8_t *key,
    const uint8_t *iv, const uint8_t *text, size_t textlen,
    SY_BLOCK_CIPHER_MODE mode);
extern bool sy_aes256_encrypt(uint8_t *dest, const uint8_t *key,
    const uint8_t *iv, const uint8_t *text, size_t textlen,
    SY_BLOCK_CIPHER_MODE mode);
extern bool sy_aes256_decrypt(uint8_t *dest, const uint8_t *key,
    const uint8_t *iv, const uint8_t *text, size_t textlen,
    SY_BLOCK_CIPHER_MODE mode);

#ifdef __cplusplus
}
#endif

#endif /* __SY_AES_H__ */

