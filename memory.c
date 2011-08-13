#include <stdio.h>
#include <stdint.h>
#include "sayama/memory.h"

volatile void *
sy_memset(volatile void *buf, int ch, size_t n)
{
  volatile uint8_t *vbuf;
  size_t i;

  vbuf = (volatile uint8_t *)buf;
  for (i = 0; i < n; i++)
    vbuf[i] = ch;
  return buf;
}

volatile void *
sy_memcpy(volatile void *dest, const volatile void *src, size_t n)
{
  volatile uint8_t *vdest, *vsrc;

  vdest = (volatile uint8_t *)dest;
  vsrc = (volatile uint8_t *)src;
  while (n--)
    vdest[n] = vsrc[n];
  return dest;
}

volatile void *
sy_memmove(volatile void *dest, const volatile void *src, size_t n)
{
  volatile uint8_t *vdest, *vsrc;
  size_t i;

  vdest = (volatile uint8_t *)dest;
  vsrc = (volatile uint8_t *)src;
  if (vdest < vsrc && vsrc < vdest + n) {
    for (i = 0; i < n; i++)
      vdest[i] = vsrc[i];
  } else {
    while (n--)
      vdest[n] = vsrc[n];
  }
  return dest;
}

