#include "s21_decimal.h"

int s21_is_zero(s21_decimal value) {
  int is_zero = 0;

  if (!value.bits[0] && !value.bits[1] && !value.bits[2])
      is_zero = 1;

  return is_zero;
}