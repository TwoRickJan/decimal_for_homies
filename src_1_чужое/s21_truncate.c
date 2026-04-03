#include "s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
    int return_val = 0;
    // if (value.state == S21_PLUS_INF || value.state == S21_MINUS_INF
    // || value.state == S21_NAN) {
    //     result->state = value.state;
    //     return_val = 1;
    // } else {
        s21_decl_to_null(result);
        if (!s21_get_scale(value)) {
            s21_copy(result, value);
        } else {
            float float_value = 0.0;
            s21_from_decimal_to_float(value, &float_value);
            float_value = trunc(float_value);
            if (float_value < 0.0) {
                s21_set_sign(result, 1);
                float_value *= -1;
            }
            for (int i = 0; float_value >= 1 && i < START_INFO; i++) {
                float_value = floorl(float_value) / 2;
                if (float_value - floorl(float_value) > powl(10.0, -1 * FLOAT_ACCURACY)) {
                    s21_set_bit(result, i, 1);
                }
            }
            s21_set_scale(result, 0);
        }
    // }
    return return_val;
}
