#ifndef __SY_TYPES_H__
#define __SY_TYPES_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* direction */
typedef enum {
  SY_ENCRYPT,
  SY_DECRYPT
} SY_CIPHER_DIRECTION;

/* block cipher mode */
typedef enum {
  SY_ECB_MODE,
  SY_CBC_MODE,
  SY_CFB_MODE,
  SY_OFB_MODE,
  SY_CTR_MODE
} SY_BLOCK_CIPHER_MODE;

#ifdef __cplusplus
}
#endif

#endif /* __SY_TYPES_H__ */

