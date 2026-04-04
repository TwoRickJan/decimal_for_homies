
/*
 * s21_decimal_convert.c — преобразование типов
 *
 * Функции (все заглушки):
 * - s21_from_int_to_decimal    — int → decimal
 * - s21_from_float_to_decimal  — float → decimal
 * - s21_from_decimal_to_int    — decimal → int
 * - s21_from_decimal_to_float  — decimal → float
 */

#include "s21_decimal.h"

// =========================== API: КОНВЕРТАЦИЯ ===========================

/*
 * s21_from_int_to_decimal — преобразование int в decimal (заглушка)
 *
 * Алгоритм:
 * 1. Обнулить dst
 * 2. Если src < 0: установить знак, взять |src|
 * 3. Записать |src| в bits[0]
 * 4. Scale = 0
 * 5. Вернуть S21_OK
 */
int s21_from_int_to_decimal(int src, s21_decimal *dst) {
    (void)src;
    (void)dst;
    return S21_OK;
}

/*
 * s21_from_float_to_decimal — преобразование float в decimal (заглушка)
 *
 * Алгоритм:
 * 1. Проверить: inf/nan → ошибка, |x|<1e-28 → ошибка, |x|>max → ошибка
 * 2. Разобрать float на sign, exponent, mantissa
 * 3. Преобразовать в десятичное число
 * 4. Округлить до 7 значащих цифр
 * 5. Записать в dst
 * 6. Вернуть S21_OK
 */
int s21_from_float_to_decimal(float src, s21_decimal *dst) {
    (void)src;
    (void)dst;
    return S21_OK;
}

/*
 * s21_from_decimal_to_int — преобразование decimal в int (заглушка)
 *
 * Алгоритм:
 * 1. Получить знак и масштаб
 * 2. Взять мантиссу (bits[0-2])
 * 3. Если scale > 0: разделить мантиссу на 10^scale
 * 4. Проверить что помещается в int
 * 5. Применить знак, записать в *dst
 * 6. Вернуть S21_OK (или 1 при ошибке)
 */
int s21_from_decimal_to_int(s21_decimal src, int *dst) {
    (void)src;
    (void)dst;
    return S21_OK;
}

/*
 * s21_from_decimal_to_float — преобразование decimal в float (заглушка)
 *
 * Алгоритм:
 * 1. Получить знак, мантиссу, масштаб
 * 2. value = мантисса / 10^scale
 * 3. Применить знак
 * 4. Проверить что помещается в float
 * 5. Записать в *dst
 * 6. Вернуть S21_OK (или 1 при ошибке)
 */
int s21_from_decimal_to_float(s21_decimal src, float *dst) {
    (void)src;
    (void)dst;
    return S21_OK;
}