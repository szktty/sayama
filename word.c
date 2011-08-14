#include "sayama/word.h"

void
sy_fill_words(sy_word *words, uint8_t v, size_t from, size_t to)
{
  size_t i, wi;
  sy_word vw;

  if (to - from >= 4) {
    vw = v << 24 | v << 16 | v << 8 | v;
    wi = from / 4;
    switch (from % 4) {
    case 0:
      words[wi] = vw;
      break;

    case 1:
      words[wi] = (words[wi] & 0xff000000) | (vw & 0x00ffffff);
      break;

    case 2:
      words[wi] = (words[wi] & 0xffff0000) | (vw & 0x0000ffff);
      break;

    case 3:
      words[wi] = (words[wi] & 0xffffff00) | (vw & 0x000000ff);
      break;
    }

    i = (wi + 1) * 4;
    if (i <= to) {
      for (; i < to / 4 * 4; i += 4)
        words[i/4] = vw;

      wi = to / 4;
      switch (to % 4) {
      case 0:
        words[wi] = (words[wi] & 0x00ffffff) | (vw & 0xff000000);
        break;

      case 1:
        words[wi] = (words[wi] & 0x0000ffff) | (vw & 0xffff0000);
        break;

      case 2:
        words[wi] = (words[wi] & 0x000000ff) | (vw & 0xffffff00);
        break;

      case 3:
        words[wi] = vw;
        break;
      }
    }
  } else {
    for (i = from; i <= to; i++) {
      words[i/4] = (words[i/4] & ~(0xff << ((3-i%4)*8))) |
        (v << ((3-i%4)*8));
    }
  }
}

