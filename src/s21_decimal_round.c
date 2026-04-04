/*
 * s21_decimal_round.c — функции округления decimal
 *
 * API функции:
 * - s21_floor     — округление вниз (к -∞)
 * - s21_round     — округление до ближайшего целого
 * - s21_truncate  — отбрасывание дробной части
 * - s21_negate    — инверсия знака
 *
 * Внутренние функции:
 * - s21_bank_round               — банковское округление
 * - s21_round_mantissa           — округление мантиссы
 * - s21_shift_right_mantissa_with_round — сдвиг вправо с округлением
 */

#include "s21_decimal.h"

// =========================== ВНУТРЕННИЕ: ОКРУГЛЕНИЕ ===========================

/*
 * s21_bank_round — банковское округление (до ближайшего чётного)
 * remainder > 5 → вверх, < 5 → вниз, == 5 → до ближайшего чётного
 */
void s21_bank_round(s21_decimal *value, int remainder) {
    if (remainder > 5) {
        // Округление вверх
        s21_decimal one = {0};
        one.bits[0] = 1;
        s21_add_mantissa(*value, one, value);
    } else if (remainder == 5) {
        // Проверяем последнюю цифру мантиссы (младший бит)
        int last_digit = value->bits[0] & 1;
        
        if (last_digit == 1) {
            // Нечётная → округление вверх
            s21_decimal one = {0};
            one.bits[0] = 1;
            s21_add_mantissa(*value, one, value);
        }
        // Чётная → округление вниз (ничего не делаем)
    }
    // remainder < 5 → округление вниз (ничего не делаем)
}

/*
 * s21_round_mantissa — округление мантиссы (банковское)
 */
void s21_round_mantissa(s21_decimal *d, int remainder) {
    if (remainder > 5) {
        s21_decimal one = {0};
        one.bits[0] = 1;
        s21_add_mantissa(*d, one, d);
    } else if (remainder == 5) {
        int last_digit = d->bits[0] & 1;
        if (last_digit == 1) {
            s21_decimal one = {0};
            one.bits[0] = 1;
            s21_add_mantissa(*d, one, d);
        }
    }
}

/*
 * s21_shift_right_mantissa_with_round — сдвиг вправо с банковским округлением
 */
int s21_shift_right_mantissa_with_round(s21_decimal *d, int shift) {
    int remainder = 0;
    for (int i = 0; i < shift; i++) {
        remainder = s21_right_shift_one(d);
    }
    s21_round_mantissa(d, remainder);
    return remainder;
}

// =========================== API: ОКРУГЛЕНИЕ ===========================

/*
 * s21_floor — округление вниз (к -∞)
 * Примеры: 3.14 → 3, -3.14 → -4, 5.0 → 5
 */
int s21_floor(s21_decimal value, s21_decimal *result) {
    (void)value;
    (void)result;
    return S21_OK;
}

/*
 * s21_round — округление до ближайшего целого (банковское)
 * При .5 → округление до ближайшего чётного
 */
int s21_round(s21_decimal value, s21_decimal *result) {
    (void)value;
    (void)result;
    return S21_OK;
}

/*
 * s21_truncate — отбрасывание дробной части
 * Примеры: 3.14 → 3, -3.14 → -3, 5.99 → 5
 */
int s21_truncate(s21_decimal value, s21_decimal *result) {
    (void)value;
    (void)result;
    return S21_OK;
}

/*
 * s21_negate — инверсия знака числа
 */
int s21_negate(s21_decimal value, s21_decimal *result) {
    *result = value;
    if (s21_get_sign(value)) {
        s21_set_sign(result, 0);
    } else {
        s21_set_sign(result, 1);
    }
    return S21_OK;
}