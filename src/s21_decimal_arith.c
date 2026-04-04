/*
 * s21_decimal_arith.c — арифметические операции с decimal
 *
 * Функции:
 * - s21_add        — сложение двух чисел
 * - s21_sub        — вычитание двух чисел
 * - s21_mul        — умножение двух чисел (заглушка)
 * - s21_div        — деление двух чисел (заглушка)
 *
 * Внутренние функции:
 * - s21_handle_zeros            — обработка нулевых операндов
 * - s21_handle_operation_overflow — обработка переполнения
 * - s21_finalize_result         — финальная запись результата
 */

#include "s21_decimal.h"

// =========================== API: АРИФМЕТИКА ===========================

/*
 * s21_add — сложение двух decimal чисел
 */
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    s21_zero_decimal(result);
    
    if (s21_handle_zeros(&value_1, &value_2, result)) {
        return S21_OK;
    }
    
    // Нормализация
    if (!s21_normalize(&value_1, &value_2)) {
        return S21_TOO_LARGE;
    }
    int max_scale = (s21_get_scale(value_1) > s21_get_scale(value_2)) ? 
                     s21_get_scale(value_1) : s21_get_scale(value_2);
    
    // Сложение/вычитание
    int sign = s21_get_sign(value_1);
    s21_decimal temp_result = {0};
    int overflow = 0;
    
    if (s21_add_sub_mantissa(value_1, value_2, &temp_result, sign, &overflow)) {
        return S21_TOO_LARGE;
    }
    
    // Обработка переполнения
    if (overflow) {
        if (max_scale > 0) {
            s21_divide_by_10(&temp_result);
            max_scale--;
        } else {
            return (sign == 0) ? S21_TOO_LARGE : S21_TOO_SMALL;
        }
    }
    
    s21_finalize_result(&temp_result, result, max_scale, sign);
    return S21_TOO_LARGE;
}

/*
 * s21_sub — вычитание двух decimal чисел (через сложение с инвертированным знаком)
 */
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    s21_decimal temp_value;
    
    // Меняем знак у value_2
    s21_negate(value_2, &temp_value);
    
    // Складываем
    return s21_add(value_1, temp_value, result);
}

/*
 * s21_mul — умножение двух decimal чисел (заглушка)
 */
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    (void)value_1;
    (void)value_2;
    (void)result;

    /*
     * 1. Проверить и обнулить result
     * 2. Проверка на ноль value_1 и value_2
     * 3. Определение знака: sign_result = sign1 ^ sign2 (XOR)
     * 4. Определение масштаба: scale_result = scale1 + scale2
     * 5. Умножение мантисс (96×96 = 192 бита)
     * 6. Приведение к 96 битам с уменьшением масштаба
     * 7. Банковское округление
     * 8. Проверка переполнения
     * 9. Запись результата
     * 10. Вернуть S21_OK
     */
    return S21_OK;
}

/*
 * s21_div — деление двух decimal чисел (заглушка)
 */
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    (void)value_1;
    (void)value_2;
    (void)result;
    return S21_OK;
}





// =========================== ВНУТРЕННИЕ ФУНКЦИИ ===========================

/*
 * s21_handle_zeros — обработка нулевых операндов
 * Если один из операндов 0, записывает другой в result
 */
int s21_handle_zeros(s21_decimal *a, s21_decimal *b, s21_decimal *result) {
    if (s21_is_zero(*a)) {
        *result = *b;
        return 1;
    }
    if (s21_is_zero(*b)) {
        *result = *a;
        return 1;
    }
    return 0;
}



/*
 * s21_handle_operation_overflow — обработка переполнения после операции
 * (УДАЛИТЬ? БЕСПОЛЕЗНАЯ)
 */
int s21_handle_operation_overflow(int was_overflow, s21_decimal *temp, int *max_scale, int sign) {
    if (was_overflow) {
        if (*max_scale > 0) {
            s21_divide_by_10(temp);
            (*max_scale)--;
            return 0;
        } else {
            return (sign == 0) ? S21_TOO_LARGE : S21_TOO_SMALL;
        }
    }
    return 0;
}

/*
 * s21_finalize_result — финальная запись результата
 * Устанавливает scale и sign, копирует temp в result
 */
void s21_finalize_result(s21_decimal *temp, s21_decimal *result, int max_scale, int sign) {
    s21_set_scale(temp, max_scale);
    s21_set_sign(temp, sign);
    *result = *temp;
}
