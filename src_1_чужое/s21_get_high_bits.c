#include "s21_decimal.h"

// int s21_get_high_bits(s21_decimal value, int number, s21_decimal *result) {  // add check number < scale
//     s21_decl_to_null(&result);
//     int i = START_HIGHT;
//     for (; !s21_get_bit(value, i); i--)
//         continue;
//     for (int j = number; j >= 0; j--) {
//         s21_set_bit(result, j, s21_get_bit(value, i));
//         i--;
//     }
//     return i;
// }

int s21_get_high_bits(s21_decimal value_1, s21_decimal value_2, s21_decimal *result_of_search, s21_decimal *final_result,
int prev_i) { 
    int answer = 0;
    s21_decl_to_null(result_of_search);

    int i = START_HIGHT;
    for (; !s21_get_bit(value_1, i) && i >= 0; i--)  // находим первый ненулевой бит
        continue;
    s21_set_sign(&value_1, 0);
    s21_set_sign(&value_2, 0);
    int dif = prev_i - i;
    while (dif > 0) {
        s21_left_shift(final_result);
        dif--;
    }
    while(s21_is_less(*result_of_search, value_2) && i >= 0) {
        s21_left_shift(result_of_search);
        s21_set_bit(result_of_search, 0, s21_get_bit(value_1, i));
        if (i <= prev_i) 
            s21_left_shift(final_result);
        i--;
    }
    return i; //return return_value;
}
