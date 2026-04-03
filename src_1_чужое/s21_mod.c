#include "s21_decimal.h"

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int res = 0;
    s21_decimal res_without_mantissa = {0,0,0,0};
    s21_natural_div(value_1, value_2, &res_without_mantissa);
    s21_mul(value_2, res_without_mantissa, &res_without_mantissa);
    s21_sub(value_1, res_without_mantissa, result);
    return 0;
}
