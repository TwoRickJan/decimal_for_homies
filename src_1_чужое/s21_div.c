#include "s21_decimal.h"
void s21_mul_10(s21_decimal* value) {
    s21_left_shift(value);
    s21_decimal five = {0,0,0,0};
    s21_from_int_to_decimal(5, &five);
    s21_mul(*value, five, value);
    return;
}
int s21_first_unzero_bit(s21_decimal value) {
    int i = START_INFO;
    for (; !s21_get_bit(value, i) && i >= 0; i--) {
        continue;
    }
    return i;
}
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int shift = 0;
    int left_shift_flag = 0;
    float v1;
    if (!s21_get_scale(value_1) && !s21_get_scale(value_2)) {
        s21_natural_div(value_1, value_2, result);
    } else {
        int scale1 = s21_get_scale(value_1), scale2 = s21_get_scale(value_2);
        int scale = 0;
        while (s21_first_unzero_bit(value_1) < START_INFO - 3) {
            s21_mul_10(&value_1);  /// or left_shift - don't know
            scale1++;
            shift++;
        }
        while (s21_is_less(value_1, value_2) && scale2 > 0) {
            s21_set_scale(&value_2, 1);
            s21_round(value_2, &value_2);
            s21_set_scale(&value_2, --scale2);
        }
        s21_set_scale(&value_1, 0);
        s21_set_scale(&value_2, 0);
        s21_natural_div(value_1, value_2, result);
        scale = scale1;
        while (scale > 0 && scale2 > 0) {
            scale--;
            scale2--;
        }
        while (scale2 > 0) {
            s21_mul_10(result);
            scale2--;
        }
        s21_set_scale(result, scale);

        
        
        
        // float a;

        // int scales_diff = s21_get_scale(value_1) - s21_get_scale(value_2);
        // int scale_1 = s21_get_scale(value_1);
        // int scale_2 = s21_get_scale(value_2);
        // s21_set_scale(&value_1, 0);
        // s21_set_scale(&value_2, 0);
       
        // s21_natural_div(value_1, value_2, result);
        // while (s21_is_less(value_1, value_2)) {
        //     s21_left_shift(&value_1);
        //     shift++;
        // }
        // while (scales_diff < 0) {
        //     s21_mul_10(result);
        //     scales_diff++;
        // }
        // s21_set_scale(result, scales_diff);
    }
    return 0;
}
