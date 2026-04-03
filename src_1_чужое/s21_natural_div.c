#include "s21_decimal.h"

int s21_natural_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int flag = 0, i = 0, count = 0, answer = 0, sign = 0, prev_i = 0;
    s21_decimal null, tmp;
    s21_decl_to_null(result);
    s21_decl_to_null(&null);
    // s21_set_scale(&value_2, 0);
    s21_decimal value_1_copy = value_1;
    s21_decimal value_2_copy = value_2;
    s21_set_sign(&value_1_copy, 0);
    s21_set_sign(&value_2_copy, 0);
    if (s21_is_zero(value_2)) {
        flag = 3;
    } else if (s21_is_one(value_2)) {
        s21_copy(result, value_1);
    } else if (s21_is_greater_or_equal(value_1_copy, value_2_copy)) {
        if ((s21_is_less(value_1, null) && s21_is_greater(value_2, null)) ||
            (s21_is_greater(value_1, null) && s21_is_less(value_2, null))) {
            sign = 1;
        }
        s21_set_sign(&value_1, 0);
        s21_set_sign(&value_2, 0);
        while (s21_is_less_or_equal(value_2, value_1)) {  // делим до тех пор, пока делитель меньше делимого             
            s21_decl_to_null(&tmp);
            i = s21_get_high_bits(value_1, value_2, &tmp, result, prev_i);  // i - где end делимое на данном шаге
                                                                    // tmp - левая часть числа с нулями на конце
            prev_i = i;   
            s21_copy(&tmp, value_2);
            for (int j = i; j >= 0; j--) {
                s21_left_shift(&tmp);
            }
            s21_sub(value_1, tmp, &value_1);
            s21_set_bit(result, 0, 1);
            if (!s21_is_less_or_equal(value_2, value_1)) {
                while (i >= 0) {
                    s21_left_shift(result);
                    i--;
                }
            }
        }
        s21_set_sign(result, sign);

    }
    return flag;
}
