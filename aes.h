#ifndef __SY_AES_PRIVATE_H__
#define __SY_AES_PRIVATE_H__

#ifdef __cplusplus
extern "C"
{
#endif

extern unsigned int _sy_aes_expand_key(uint8_t *dest,
    const uint8_t *key, SY_AES_KEYLEN keylen);

static inline void _sy_aes_mix_columns(uint8_t *block);
static inline uint8_t _sy_aes_mul2(uint8_t v);

static inline void
_sy_aes_mix_columns(uint8_t *block)
{
  unsigned int i;
  uint8_t o0, o1, o2, o3, t0, t1, t2, t3;

  for (i = 0; i < 16; i += 4) {
    o0 = block[i];
    o1 = block[i+1];
    o2 = block[i+2];
    o3 = block[i+3];
    t0 = _sy_aes_mul2(o0);
    t1 = _sy_aes_mul2(o1);
    t2 = _sy_aes_mul2(o2);
    t3 = _sy_aes_mul2(o3);
    block[i]   = t0        ^ (o1 ^ t1) ^ o2        ^ o3;
    block[i+1] = o0        ^ t1        ^ (o2 ^ t2) ^ o3;
    block[i+2] = o0        ^ o1        ^ t2        ^ (o3 ^ t3);
    block[i+3] = (o0 ^ t0) ^ o1        ^ o2        ^ t3;
  }
}

static inline uint8_t
_sy_aes_mul2(uint8_t v)
{
  if ((v & 0x80) != 0)
    return (v << 1) ^ 0x011b;
  else
    return v << 1;
}

#ifdef __cplusplus
}
#endif

#endif /* __SY_AES_PRIVATE_H__ */

