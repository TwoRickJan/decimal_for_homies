// =========================== ДРУГИЕ ФУНКЦИИ ===========================
#include "s21_decimal.h"





/*
 * Банковское округление (округление до ближайшего чётного)
 * 
 * Применяется после деления мантиссы на 10^scale
 * 
 * @param value число для округления
 * @param scale количество десятичных знаков для отбрасывания
 * @param remainder остаток при делении (отброшенная часть)
 * 
 * Алгоритм:
 *   if (remainder > 5) → округление вверх
 *   if (remainder < 5) → округление вниз
 *   if (remainder == 5) → смотрим на последнюю цифру мантиссы:
 *       if (последняя цифра чётная) → округление вниз
 *       if (последняя цифра нечётная) → округление вверх
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



// Округление мантиссы (банковское округление)
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

// Сдвиг вправо с банковским округлением
int s21_shift_right_mantissa_with_round(s21_decimal *d, int shift) {
    int remainder = 0;
    for (int i = 0; i < shift; i++) {
        remainder = s21_right_shift_one(d);
    }
    s21_round_mantissa(d, remainder);
    return remainder;
}


int s21_floor(s21_decimal value, s21_decimal *result) {
    (void)value;
    (void)result;
    /*
    * Округление вниз (к отрицательной бесконечности)
    * 
    * Последовательность действий:
    * 1. Получить знак и масштаб
    * 2. Если масштаб == 0 → result = value (ничего не делаем)
    * 3. Иначе:
    *      - Отбросить дробную часть (как в truncate)
    *      - Если число отрицательное И есть дробная часть → вычесть 1
    * 4. Установить масштаб = 0
    * 5. Записать в result
    * 6. Вернуть S21_OK
    * 
    * Примеры:
    *   3.14 → 3
    *   -3.14 → -4
    *   5.0 → 5
    */
    return S21_OK;
}

int s21_round(s21_decimal value, s21_decimal *result) {
    (void)value;
    (void)result;
    /*
 * Округление до ближайшего целого (банковское округление)
 * 
 * Последовательность действий:
 * 1. Получить знак и масштаб
 * 2. Если масштаб == 0 → result = value
 * 3. Иначе:
 *      - Сохранить дробную часть (мантисса % 10^scale)
 *      - Отбросить дробную часть (целая часть)
 *      - Сравнить дробную часть с 0.5 * 10^scale
 *      - Применить банковское округление:
 *          - Если дробная часть > 0.5 → округлить вверх
 *          - Если дробная часть < 0.5 → округлить вниз
 *          - Если дробная часть == 0.5:
 *              - Если целая часть чётная → округлить вниз
 *              - Если нечётная → округлить вверх
 * 4. Установить масштаб = 0
 * 5. Записать в result
 * 6. Вернуть S21_OK
 */
    return S21_OK;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
    (void)value;
    (void)result;
    /*
 * Отбрасывание дробной части (усечение)
 * 
 * Последовательность действий:
 * 1. Получить масштаб
 * 2. Если масштаб == 0 → result = value
 * 3. Иначе:
 *      - Разделить мантиссу на 10^scale (целочисленное деление)
 *      - Установить масштаб = 0
 *      - Знак оставить прежним
 * 4. Записать в result
 * 5. Вернуть S21_OK
 * 
 * Примеры:
 *   3.14 → 3
 *   -3.14 → -3
 *   5.99 → 5
 */
    return S21_OK;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
    // 1. Скопировать value в result
    *result = value;
    
    // 2. Инвертировать бит знака (бит 31 в bits[3])
    if (s21_get_sign(value)) {
        s21_set_sign(result, 0);  // был минус → стал плюс
    } else {
        s21_set_sign(result, 1);  // был плюс → стал минус
    }
    
    // 3. Вернуть S21_OK
    return S21_OK;
}