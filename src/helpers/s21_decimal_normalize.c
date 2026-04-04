/*
 * s21_decimal_normalize.c — нормализация decimal чисел
 *
 * Функции:
 * - s21_normalize             — приведение двух чисел к одинаковому масштабу
 * - s21_remove_trailing_zeros — удаление конечных нулей из мантиссы
 */

#include "../s21_decimal.h"

// =========================== НОРМАЛИЗАЦИЯ ===========================

/*
 * Нормализация чисел (приведение к одинаковому масштабу)
 * При переполнении уменьшает масштаб и округляет
 * 
 * Возвращает:
 *   S21_OK — нормализация успешна
 *   S21_NORMOLIZATION_ERROR — ошибка (число не удалось нормализовать)
 */
int s21_normalize(s21_decimal *a, s21_decimal *b) {
    int scale_a = s21_get_scale(*a);
    int scale_b = s21_get_scale(*b);
    
    if (scale_a == scale_b) return S21_OK;
    
    if (scale_a < scale_b) {
        int diff = scale_b - scale_a;
        for (int i = 0; i < diff; i++) {
            if (s21_multiply_by_10(a) == S21_TOO_LARGE) {
                // Переполнение при умножении → нужно уменьшить масштаб
                if (scale_a > 0) {
                    // Уменьшаем масштаб a и делим мантиссу на 10
                    s21_divide_by_10(a);
                    scale_a--;
                    i--;  // повторяем умножение
                } else {
                    return S21_NORMOLIZATION_ERROR;  // не удалось нормализовать
                }
            }
        }
        s21_set_scale(a, scale_b);
    } else {
        int diff = scale_a - scale_b;
        for (int i = 0; i < diff; i++) {
            if (s21_multiply_by_10(b) == S21_TOO_LARGE) {
                if (scale_b > 0) {
                    s21_divide_by_10(b);
                    scale_b--;
                    i--;
    } else {
                    return S21_NORMOLIZATION_ERROR;
    }
            }
        }
        s21_set_scale(b, scale_a);
    }
    
    return S21_OK;
}


/*
 * s21_remove_trailing_zeros — удаление конечных нулей из мантиссы
 */
void s21_remove_trailing_zeros(s21_decimal *d) {
    int scale = s21_get_scale(*d);
    if (scale == 0) return;
    
    while (scale > 0 && (d->bits[0] % 10 == 0)) {
        // Проверяем, что число делится на 10
        s21_decimal temp = *d;
        int remainder = s21_divide_by_10_with_remainder(&temp);
        
        if (remainder == 0) {
            *d = temp;
            scale--;
            s21_set_scale(d, scale);
        } else {
            break;
        }
    }
}
