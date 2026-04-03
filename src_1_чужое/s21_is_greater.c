#include "s21_decimal.h"

int s21_is_greater(s21_decimal first, s21_decimal second) {
  int is_greater = 0;
  int tmp_raz = 0;
  int bit_1 = 0;
  int bit_2 = 0;
  int first_flag = 1;
  int end_loop_flag = 0;
  s21_decimal *one, *two;

  // if (second.state == S21_PLUS_INF || first.state == S21_MINUS_INF ||
  //     first.state == S21_NAN || second.state == S21_NAN) {
  //   is_greater = 0;
  // } else 
  if (s21_is_zero(first) && !s21_get_sign(second)) {
    is_greater = 0;
  } else if (s21_get_sign(first) == 1 && s21_get_sign(second) == 0) {
    is_greater = 0;
  } else if (s21_get_sign(first) == 0 && s21_get_sign(second) == 1) {
    is_greater = 1;
  } else {
    is_greater = 0;

    if (s21_get_scale(first) <= s21_get_scale(second)) {
      one = &first;
      two = &second;
    } else {
      first_flag = 0;
      one = &second;
      two = &first;
    }
    tmp_raz = s21_get_scale(*one) - s21_get_scale(*two);

    for (int i = 95; i >= tmp_raz && !end_loop_flag; --i) {
      if (i < 0) {
        bit_1 = 0;
      } else {
        bit_1 = s21_get_bit(*one, i);
      }
      if (i - tmp_raz > 95) {
        bit_2 = 0;
      } else {
        bit_2 = s21_get_bit(*two, i - tmp_raz);
      }
      if (bit_1 > bit_2) {
        if (first_flag) {
          is_greater = 1;
        } else {
          is_greater = 0;
        }
        end_loop_flag = 1;
      } else if (bit_1 < bit_2) {
        if (first_flag) {
          is_greater = 0;
        } else {
          is_greater = 1;
        }
        end_loop_flag = 1;
      }
    }

    if (s21_get_sign(first) && s21_get_sign(second)) {
      is_greater = !is_greater;
    }
  }
  return is_greater;
}


/*
#include "s21_decimal.h"

int zero(s21_decimal a, s21_decimal b) {
  int res = 0;
  s21_decimal *p1 = &a;
  s21_decimal *p2 = &b;

  if (p1 && p2) {
    if (!a.bits[0] && !b.bits[0] && !a.bits[1] && !b.bits[1] && !a.bits[2] &&
        !b.bits[2])
      res = 1;
  }
  return res;
}
int scale_equalize(s21_decimal *a, s21_decimal *b) {
  s21_decimal *big = NULL;
  s21_decimal *small = NULL;
  s21_decimal temp;
  temp.bits[0] = temp.bits[1] = temp.bits[2] = temp.bits[3] = 0;
  int per = 0;
  int out = 1;
  if (get_scale(a) != get_scale(b)) {
    if (get_scale(a) > get_scale(b)) {
      big = a;
      small = b;
      per = 1;
    } else {
      big = b;
      small = a;
    }
    int scaleSmall = s21_get_scale(*small);
    int scaleBig = s21_get_scale(*big);
    int newscale = scaleBig - scaleSmall;
    while (newscale) {
      temp.bits[0] = small->bits[0];
      temp.bits[1] = small->bits[1];
      temp.bits[2] = small->bits[2];
      temp.bits[3] = small->bits[3];
      s21_scale_increase1(small, 1);
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
      a = big;
      b = &temp;
    } else {
      a = &temp;
      b = big;
    }
    out = 0;
  }
  return out;
}
int s21_is_greater(s21_decimal first, s21_decimal second) {
  int res = 0;
  int sign1 = s21_get_sign(first);
  int sign2 = s21_get_sign(second);
  if (!zero(first, second)) {
    if (sign1 == 0 && sign2 == 1) {
      res = 1;
    } else if (sign1 == 1 && sign2 == 0) {
      res = 0;
    } else if (sign1 == sign2) {
      scale_equalize(&first, &second);
      for (int i = 95; i >= 0; i--) {
        int bit1 = get_bit(first, i);
        int bit2 = get_bit(second, i);
        if (bit1 != bit2) {
          if (!(bit1 == 0) && bit2 == 0 && sign1 == 0) {
            res = 1;
            break;
          } else if (!(bit1 == 0) && bit2 == 0 && sign1 == 1) {
            res = 0;
            break;
          } else if (!(bit2 == 0) && sign1 == 0) {
            res = 0;
            break;
          } else if (!(bit2 == 0) && sign1 == 1) {
            res = 1;
            break;
          }
        }
      }
    }
  }
  return res;
}


 */