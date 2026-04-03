#include "s21_decimal.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
    int return_val = 0;
    *dst = 0.0;
    // if (src.state == S21_PLUS_INF || src.state == S21_MINUS_INF
    // || src.state == S21_NAN) {
    //     return_val = 1;
    // } else {
        int scale = s21_get_scale(src);
        for (int i = 0; i < START_INFO; i++) {
            if (s21_get_bit(src, i)) {
                *dst += pow (2, i);
            }
        }
        while (scale) {
            *dst /= 10;
            scale--;
        }
        if (s21_get_sign(src)) {
            *dst = *dst*(-1);
        }
    // }
    return return_val;
}
