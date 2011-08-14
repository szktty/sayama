#include "sayama/word.h"

void
sy_fill_words(sy_word *words, uint8_t v, size_t from, size_t to)
{
  size_t i, wi;
  sy_word vw;

  if (to - from >= 4) {
    vw = v << 24 | v << 16 | v << 8 | v;
    wi = from / 4;
    words[wi] = (words[wi] &
        ((0xffffffffUL << ((4 - from % 4) * 8))) & 0xffffffffUL) |
        ((vw << (from % 4 * 8)) & 0xffffffffUL);

    i = (wi + 1) * 4;
    if (i <= to) {
      for (; i < to / 4 * 4; i += 4)
        words[i/4] = vw;

      if (i <= to) {
        wi = to / 4;
        words[wi] = (words[wi] &
            (0xffffffffUL >> ((to % 4 + 1) * 8))) |
          ((vw << ((3 - to % 4) * 8)) & 0xffffffffUL);
      }
    }
  } else {
    for (i = from; i <= to; i++) {
      words[i/4] = (words[i/4] & ~(0xffU << ((3-i%4)*8))) |
        (v << ((3-i%4)*8));
    }
  }
}

