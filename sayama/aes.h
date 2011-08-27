#ifndef __SY_AES_H__
#define __SY_AES_H__

#include <sayama/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum SY_AES_KEY_LEN {
  SY_AES_KEY_128 = 128, 
  SY_AES_KEY_192 = 192,
  SY_AES_KEY_256 = 256
} SY_AES_KEY_LEN;

extern bool sy_aes(uint8_t *dest, SY_AES_KEY_LEN key_len,
    SY_BLOCK_CIPHER_MODE mode, SY_CIPHER_DIRECTION dir,
    const uint8_t *key,  const uint8_t *iv,
    const uint8_t *text, size_t text_len);

#ifdef __cplusplus
}
#endif

#endif /* __SY_AES_H__ */

