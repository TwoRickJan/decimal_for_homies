#include "s21_decimal.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int flag = 0;
    int sign_control = 0;
    s21_decimal tmp;
    s21_decl_to_null(&tmp);
    s21_decl_to_null(result);
    if (s21_get_sign(value_1) != s21_get_sign(value_2)) {
        sign_control = 1;
    }
    s21_set_sign(&value_1, 0);
    s21_set_sign(&value_2, 0);
    scale_equalize(&value_1, &value_2);
    s21_copy(&tmp, value_1);

    //int b = 0;
    //s21_from_decimal_to_int(tmp, &b);
    //printf("|%d|\n", b);
    int b = 5;
    for (int i = 0; i < 96 && b > 0; i++) {
        if (s21_get_bit(value_2, i)) {
            flag = s21_add(*result, tmp, result);
        }
        left(&tmp, 1);

        s21_from_decimal_to_int(tmp, &b);
    }
    s21_set_sign(result, sign_control);
    return flag;
}
