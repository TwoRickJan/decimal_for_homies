#include "s21_decimal.h"

int scale_equalize(s21_decimal *first, s21_decimal *second) {
  s21_decimal *big = NULL;
  s21_decimal *small = NULL;
  s21_decimal temp;
  temp.bits[0] = temp.bits[1] = temp.bits[2] = temp.bits[3] = 0;
  int per = 0;
  int out = 1;
  if (s21_get_scale(*first) != s21_get_scale(*second)) {
    if (s21_get_scale(*first) > s21_get_scale(*second)) {
      big = first;
      small = second;
      per = 1;
    } else {
      big = second;
      small = first;
    }
    int scaleSmall = s21_get_scale(*small);
    int scaleBig = s21_get_scale(*big);
    int newscale = scaleBig - scaleSmall;
    while (newscale) {
      temp.bits[0] = small->bits[0];
      temp.bits[1] = small->bits[1];
      temp.bits[2] = small->bits[2];
      temp.bits[3] = small->bits[3];
      s21_scale_increase(small, 1);
      if (!s21_get_bit(*small, 96)) {
        s21_copy(&temp, *small);
        scaleSmall++;
        newscale--;
      } else {
        s21_copy(small, temp);
        break;
      }
    }
    s21_set_scale(&temp, scaleSmall);
    int newScale2 = scaleBig - scaleSmall;
    while (newScale2) {
      s21_scale_decrease(big, newScale2);
      s21_set_scale(big, scaleSmall);
      newScale2--;
    }
    if (per) {
      first = big;
      second = &temp;
    } else {
      first = &temp;
      second = big;
    }
    out = 0;
  }
  return out;
}

int bit_addition(s21_decimal *a, s21_decimal *b, s21_decimal *res) {
  int flag = 0;
  int tmp = 0;
  int highBit = 0;
  int downBit = 0;
  s21_decl_to_null(res);
  for (int i = 0; i < 96; i++) {
    highBit = s21_get_bit(*a, i);
    downBit = s21_get_bit(*b, i);
    if (highBit && downBit) {
      if (tmp == 1) {
      s21_set_bit(res, i, 1);
    }
      tmp = 1;
    }
    if (highBit ^ downBit) {
      if (tmp == 0) {
      s21_set_bit(res, i, 1);
      }
    }
    if (!highBit && !downBit && tmp) {
      s21_set_bit(res, i, 1);
      tmp = 0;
    }
  }
  if (tmp == 1) {
    // res->state = S21_PLUS_INF;
    flag = 1;
  } 
  // else {
  //   res->state = S21_NORMAL;
  // }
  return flag;
}

void s21_scale_decrease(s21_decimal *value, int shift) {
  int i, tmp, j = 0;
  unsigned long long overflow;
  while (j < shift) {
    overflow = value->bits[2];
    i = 2;
    while (i > 0) {
      tmp = overflow % 10;
      value->bits[i] = overflow / 10;
      i--;
      overflow = tmp * (S21_MAX_UINT + 1) + value->bits[i];
      if (i == 0) value->bits[i] = overflow / 10;
    }
    j++;
  }
  s21_set_scale(value, (s21_get_scale(*value) - shift));
}

void s21_scale_increase(s21_decimal *value, int shift) {
  if ((s21_get_scale(*value) + shift) < 29) {
    s21_set_scale(value, (s21_get_scale(*value) + shift));
    s21_decimal tmp;
    tmp = *value;
    s21_decimal tmp1;
    tmp1 = *value;
    left(&tmp, 1);
    left(&tmp1, 3);
    if (bit_addition(&tmp, &tmp1, value)) {
      s21_set_bit(value, 96, 1);
    }
  }
}

void left(s21_decimal *d, int g) {
  int lastbit = last_bit(*d);
  if (lastbit + g > 95) {
    return;
  }
  for (int i = 0; i < g; i++) {
    int bit31 = s21_get_bit(*d, 31);
    int bit63 = s21_get_bit(*d, 63);
    d->bits[0] <<= 1;
    d->bits[1] <<= 1;
    d->bits[2] <<= 1;
    if (bit31) s21_set_bit(d, 32, 1);
    if (bit63) s21_set_bit(d, 64, 1);
  }
}

void convert(s21_decimal *d) {
  s21_decimal result;
  s21_decimal add = {1, 0, 0, 0};
  d->bits[0] = ~d->bits[0];
  d->bits[1] = ~d->bits[1];
  d->bits[2] = ~d->bits[2];
  bit_addition(d, &add, &result);

  d->bits[0] = result.bits[0];
  d->bits[1] = result.bits[1];
  d->bits[2] = result.bits[2];
  s21_set_bit(d, 97, 1);
}

int last_bit(s21_decimal d) {
  int bit = 95;
  for (; bit >= 0 && s21_get_bit(d, bit) == 0; bit--) {
  }
  return bit;
}
