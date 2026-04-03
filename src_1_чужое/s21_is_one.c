#include "s21_decimal.h"

int s21_is_one(s21_decimal value) {
  int is_one = 0;
  int scale = s21_get_scale(value);
  if (!value.bits[1] && !value.bits[2] && (value.bits[0] >>= scale) == 1)
      is_one = 1;

  return is_one;
}