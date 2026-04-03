#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 5;
  int sign1 = s21_get_sign(value_1);
  int sign2 = s21_get_sign(value_2);

  // float a = 0;
  // s21_from_decimal_to_float(value_1, &a);
  // printf("|%f|+", a);

  // float b = 0;
  // s21_from_decimal_to_float(value_2, &b);
  // printf("|%f|=", b);

  if (sign1 == sign2) {
    scale_equalize(&value_1, &value_2);
    if (bit_addition(&value_1, &value_2, result)) {
      if ((s21_get_scale(value_1) == 0 || s21_get_scale(value_2) == 0) &&
          sign1 == 0) {
        res = 1;
      } else if ((s21_get_scale(value_1) == 0 || s21_get_scale(value_2) == 0) &&
          sign1 == 1) {
        res = 2;
      } else if (s21_get_scale(value_1) > 0 && s21_get_scale(value_2) > 0) {
        s21_scale_decrease(&value_1, 1);
        s21_scale_decrease(&value_2, 1);
        s21_set_scale(&value_1, s21_get_scale(value_1) - 1);
        s21_set_scale(&value_2, s21_get_scale(value_2) - 1);
        res = s21_add(value_1, value_2, result);
      }
    } else {
      s21_set_sign(result, sign1);
      s21_set_scale(result, s21_get_scale(value_1));
      res = 0;
    }
  } else if (s21_get_sign(value_1) && !s21_get_sign(value_2)) {
    s21_set_sign(&value_1, 0);
    res = s21_sub(value_2, value_1, result);
  } else if (!s21_get_sign(value_1) && s21_get_sign(value_2)) {
    s21_set_sign(&value_2, 0);
    res = s21_sub(value_1, value_2, result);
  }
  
  // float c = 0;
  // s21_from_decimal_to_float(*result, &c);
  // printf("!!!|%f|\n", c);

  return res;
}
