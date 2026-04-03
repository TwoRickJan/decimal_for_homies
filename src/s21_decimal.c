// =========================== ПОБИТОВЫЕ ОПЕРАЦИИ (СПРАВКА) ===========================
/*
 * РАБОТА С БИТАМИ В s21_decimal:
 * 
 * Общее представление 4*32 бит:
 * мантиса little-endian,для bits[3] little-endian не применяется
 * ┌────────────────────────────────────────────────────────────────────────────────────────┐
 * │      bits[0]      │      bits[1]      │      bits[2]      │      bits[3]               │
 * │    биты 31←0      │    биты 63←32     │    биты 95←64     │    биты 127←96             │
 * │   (младшие)       │   (средние)       │   (старшие)       │   (самые старшие)          │
 * ├───────────────────┼───────────────────┼───────────────────┼────────────────────────────┤
 * │                   │                   │                   │ ┌────────────────────────┐ │
 * │    Мантисса       │    Мантисса       │    Мантисса       │ │ 31 │30-24 │23-16│15-0  │ │
 * │    (96 бит)       │    (96 бит)       │    (96 бит)       │ │знак│резерв│масшт│резерв│ │
 * │                   │                   │                   │ └────────────────────────┘ │
 * └───────────────────┴───────────────────┴───────────────────┴────────────────────────────┘
 * Детальное представление bits[3] (32 бита):
 * bits[3] не является частью мантиссы, поэтому для него используется фиксированная нумерация 
 * битов (бит 31 — знак, биты 16-23 — масштаб), а не little-endian порядок.
 * 
 * bits[3] = x(xxx xxxx) (xxxx xxxx) (xxxx xxxx xxxx xxxx)
 *           │        │           │                    │
 *           │        │           │                    └── биты 15-0   (резерв, 16 бит)
 *           │        │           └─── биты 23-16  (масштаб, 8 бит)
 *           │        └─── биты 30-24  (резерв, 6 бит)
 *           └──бит 31      (знак, 1 бит)
 * 
 * Значение Decimal вычисляется по формуле:
 * 
 *   decimal = (-1)^sign * (мантисса) / 10^scale
 * 
 * где:
 *   sign    = (bits[3] >> 31) & 1          (0 — положительное, 1 — отрицательное)
 *   scale   = (bits[3] >> 16) & 0xFF       (0..28)
 *   мантисса = bits[2] * 2^64 + bits[1] * 2^32 + bits[0]
 * 
 * Таким образом, число представляется как 96‑битное целое, делённое на степень 10,
 * со знаком в старшем бите служебного слова.
 */



 /*
 * =========================== ФОРМУЛЫ ОПЕРАЦИЙ ===========================
 * 
 * Исходное определение: decimal = (-1)^sign * mantissa / 10^scale
 * 
 * 1. УМНОЖЕНИЕ:
 *    a * b = (-1)^(sign_a + sign_b) * (mantissa_a * mantissa_b) / 10^(scale_a + scale_b)
 * 
 * 2. ДЕЛЕНИЕ:
 *    a / b = (-1)^(sign_a + sign_b) * (mantissa_a / mantissa_b) * 10^(scale_b - scale_a)
 * 
 * 3. СЛОЖЕНИЕ (после нормализации к scale = max(scale_a, scale_b)):
 *    a + b = (-1)^sign * (mantissa_a' + mantissa_b') / 10^scale
 *    где mantissa_a' = mantissa_a * 10^(scale - scale_a)
 *        mantissa_b' = mantissa_b * 10^(scale - scale_b)
 * 
 * 4. ВЫЧИТАНИЕ (после нормализации):
 *    a - b = (-1)^sign * (mantissa_a' - mantissa_b') / 10^scale
 * 
 * 5. УМНОЖЕНИЕ НА 10:
 *    a * 10 = (-1)^sign * (mantissa * 10) / 10^scale
 * 
 * 6. ДЕЛЕНИЕ НА 10:
 *    a / 10 = (-1)^sign * (mantissa / 10) / 10^scale
 * 
 * 7. УВЕЛИЧЕНИЕ МАСШТАБА НА 1 (число ÷ 10):
 *    a = (-1)^sign * mantissa / 10^(scale + 1)
 * 
 * 8. УМЕНЬШЕНИЕ МАСШТАБА НА 1 (число × 10):
 *    a = (-1)^sign * mantissa / 10^(scale - 1)
 * 
 * 9. КОМПЕНСИРУЮЩИЕ ОПЕРАЦИИ (число не меняется):
 *    - Умножить mantissa на 10 И увеличить scale на 1
 *      (-1)^sign * (mantissa * 10) / 10^(scale + 1) = (-1)^sign * mantissa / 10^scale
 *    
 *    - Разделить mantissa на 10 И уменьшить scale на 1
 *      (-1)^sign * (mantissa / 10) / 10^(scale - 1) = (-1)^sign * mantissa / 10^scale
 * 
 * 10. ОТБРАСЫВАНИЕ ДРОБНОЙ ЧАСТИ (truncate):
 *     result = (-1)^sign * (mantissa / 10^scale) / 10^0
 *     где mantissa / 10^scale — целочисленное деление
 * 
 * 11. ОКРУГЛЕНИЕ ДО ЦЕЛОГО (round):
 *     result = (-1)^sign * round(mantissa / 10^scale) / 10^0
 *     где round() — банковское округление
 * 
 * 12. СМЕНА ЗНАКА (negate):
 *     result = (-1)^(sign + 1) * mantissa / 10^scale
 * 
 * 13. СРАВНЕНИЕ (после нормализации к одинаковому scale):
 *     a > b  →  mantissa_a' > mantissa_b'
 *     a < b  →  mantissa_a' < mantissa_b'
 *     a == b →  mantissa_a' == mantissa_b'
 * 
 * 14. ПРОВЕРКА НА НОЛЬ:
 *     a == 0  →  mantissa == 0
 * 
 * 15. ПРЕОБРАЗОВАНИЕ В INT:
 *     result = (-1)^sign * (mantissa / 10^scale)
 *     где mantissa / 10^scale — целочисленное деление
 * 
 * 16. ПРЕОБРАЗОВАНИЕ ИЗ INT:
 *     decimal = (-1)^sign * |src| / 10^0
 * 
 * 17. ПРЕОБРАЗОВАНИЕ В FLOAT:
 *     result = (-1)^sign * (float)mantissa / 10^scale
 * 
 * 18. ПРЕОБРАЗОВАНИЕ ИЗ FLOAT:
 *     decimal = (-1)^sign * round(float_mantissa * 10^scale) / 10^scale
 */
#include "s21_decimal.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// =========================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ (ПОТОМ СДЕЛАТЬ STATIC) ===========================

// Получение знака числа (0 - положительный, 1 - отрицательный)
int s21_get_sign(s21_decimal d) {
    //`>> 31` == 0000 0000 0000 0000 0000 0000 0000 000x
    return (d.bits[3] >> 31) & 1; // `& 1` - для подстраховки. Так то и после сдвига все занулиться 
}


// Установка знака числа (0 - положительный, 1 - отрицательный)
void s21_set_sign(s21_decimal *d, int sign) {
    if (sign) {
        d->bits[3] |= S21_MASK_BIT31;
    } else {
        d->bits[3] &= ~S21_MASK_BIT31;
    }
}

/* Получение масштаба (степени 10) от 0 до 28. 
   Выделит 8 битов стоящих с 23←16 у bits[4]*/
int s21_get_scale(s21_decimal d) {
    /* Маской удаляем старшие биты 24-31 (резерв и знак) */
    return (d.bits[3] >> 16) & S21_MASK_8BIT;
}
/*
 * Установка масштаба числа
 * @param d указатель на decimal
 * @param scale значение масштаб (0-28)
 * @note Биты 16-23 обнуляются, затем записывается новое значение scale
 * @warning scale должен быть в диапазоне 0-28
 */
void s21_set_scale(s21_decimal *d, int scale) {
    if (scale < 0 || scale > 28) {
        return;  // или установить 0, или игнорировать
    }
    d->bits[3] = (d->bits[3] & ~(S21_MASK_8BIT << 16)) | ((scale & S21_MASK_8BIT) << 16);
}

// Обнуление decimal
void s21_zero_decimal(s21_decimal *d) {
    d->bits[0] = 0;
    d->bits[1] = 0;
    d->bits[2] = 0;
    d->bits[3] = 0;
}

// Сравнение абсолютных значений мантисс (без учёта знака и масштаба)
int s21_compare_abs(s21_decimal a, s21_decimal b) {
    if (a.bits[2] != b.bits[2]) {
        return (a.bits[2] > b.bits[2]) ? 1 : -1;
    }
    if (a.bits[1] != b.bits[1]) {
        return (a.bits[1] > b.bits[1]) ? 1 : -1;
    }
    if (a.bits[0] != b.bits[0]) {
        return (a.bits[0] > b.bits[0]) ? 1 : -1;
    }
    return 0;
}

// Вычитание мантисс (a - b), предполагается что a >= b
int s21_sub_mantissa(s21_decimal a, s21_decimal b, s21_decimal *result) {
    unsigned int borrow = 0;
    s21_decimal res = {0};
    
    for (int i = 0; i < 3; i++) {
        unsigned long long diff = (unsigned long long)a.bits[i] - b.bits[i] - borrow;
        
        if (diff > 0xFFFFFFFF) {
            diff += 0x100000000;
            borrow = 1;
        } else {
            borrow = 0;
        }
        
        res.bits[i] = diff & 0xFFFFFFFF;
    }
    
    if (borrow) {
        return 0;  // отрицательный результат (не должно быть при a >= b)
    }
    
    *result = res;
    return 1;
}
// Проверка на ноль
int s21_is_zero(s21_decimal d) {
    return (d.bits[0] == 0 && d.bits[1] == 0 && d.bits[2] == 0);
}
// Обработка нулевых чисел при сложении/вычитании
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


// Запись финального результата с установкой масштаба и знака
void s21_finalize_result(s21_decimal *temp, s21_decimal *result, int max_scale, int sign) {
    s21_set_scale(temp, max_scale);
    s21_set_sign(temp, sign);
    *result = *temp;
}

// Деление мантиссы на 10 с возвратом остатка (0-9)
int s21_divide_by_10_with_remainder(s21_decimal *d) {
    if (d == NULL) {
        return 0;
    }
    
    unsigned int remainder = 0;
    s21_decimal result = {0};
    
    for (int i = 2; i >= 0; i--) {
        unsigned long long num = ((unsigned long long)remainder << 32) | (unsigned int)d->bits[i];
        result.bits[i] = (unsigned int)(num / 10);
        remainder = (unsigned int)(num % 10);
    }
    
    // Копируем только мантиссу, bits[3] оставляем как в result (0)
    d->bits[0] = result.bits[0];
    d->bits[1] = result.bits[1];
    d->bits[2] = result.bits[2];
    // bits[3] не трогаем (оставляем масштаб и знак как были)
    
    return remainder;
}



// Извлечение 96-битной мантиссы как 3-х слов
void s21_get_mantissa(s21_decimal d, unsigned int *low, unsigned int *mid, unsigned int *high) {
    *low = d.bits[0];
    *mid = d.bits[1];
    *high = d.bits[2];
}

// Установка 96-битной мантиссы из 3-х слов
void s21_set_mantissa(s21_decimal *d, unsigned int low, unsigned int mid, unsigned int high) {
    d->bits[0] = low;
    d->bits[1] = mid;
    d->bits[2] = high;
}

// Сравнение мантисс (a > b)
int s21_is_greater_mantissa(s21_decimal a, s21_decimal b) {
    if (a.bits[2] != b.bits[2]) {
        return (a.bits[2] > b.bits[2]) ? 1 : 0;
    }
    if (a.bits[1] != b.bits[1]) {
        return (a.bits[1] > b.bits[1]) ? 1 : 0;
    }
    if (a.bits[0] != b.bits[0]) {
        return (a.bits[0] > b.bits[0]) ? 1 : 0;
    }
    return 0;
}

// Сравнение мантисс (a < b)
int s21_is_less_mantissa(s21_decimal a, s21_decimal b) {
    if (a.bits[2] != b.bits[2]) {
        return (a.bits[2] < b.bits[2]) ? 1 : 0;
    }
    if (a.bits[1] != b.bits[1]) {
        return (a.bits[1] < b.bits[1]) ? 1 : 0;
    }
    if (a.bits[0] != b.bits[0]) {
        return (a.bits[0] < b.bits[0]) ? 1 : 0;
    }
    return 0;
}

// Сложение двух 192-битных чисел (для умножения)
void s21_add_192(unsigned int *a, unsigned int *b, unsigned int *result) {
    unsigned int overflow = 0;
    for (int i = 0; i < 6; i++) {
        unsigned long long sum = (unsigned long long)a[i] + b[i] + overflow;
        result[i] = sum & 0xFFFFFFFF;
        overflow = sum >> 32;
    }
}

// Умножение 192-битного числа на 32-битное
void s21_mul_192_by_32(unsigned int *a, unsigned int b, unsigned int *result) {
    unsigned int carry = 0;
    for (int i = 0; i < 6; i++) {
        unsigned long long prod = (unsigned long long)a[i] * b + carry;
        result[i] = prod & 0xFFFFFFFF;
        carry = prod >> 32;
    }
}
// Деление 192-битного числа на 32-битное с остатком
unsigned int s21_div_192_by_32(unsigned int *a, unsigned int b, unsigned int *result) {
    unsigned int remainder = 0;
    for (int i = 5; i >= 0; i--) {
        unsigned long long num = ((unsigned long long)remainder << 32) | a[i];
        result[i] = num / b;
        remainder = num % b;
    }
    return remainder;
}


/*
 * Получение бита мантиссы по индексу
 * @param d decimal число
 * @param index индекс бита (0-95)
 * @return значение бита (0 или 1)
 * @note Индекс 0 соответствует младшему биту bits[0]
 * @warning index должен быть в диапазоне 0-95
 */
int s21_get_bit(s21_decimal d, int index) {
    assert(index >= 0 && index <= 95);  // проверка
    int word = index / 32; // целую часть получаем и на дробную сдвигаем
    int bit = index % 32; 
    return (d.bits[word] >> bit) & 1; // удаляем остатки через умнажение на 0b0001
}

// Установка бита мантиссы
void s21_set_bit(s21_decimal *d, int index, int bit) {
    assert(index >= 0 && index <= 95);  // проверка
    int word = index / 32;
    int bit_pos = index % 32;
    if (bit) {
        d->bits[word] |= (1u << bit_pos); //когда двигаем влево, важно что unsigned
    } else {
        d->bits[word] &= ~(1u << bit_pos);
    }
}

/*
 * Сдвиг мантиссы влево на 1 бит.
 * Возвращает 1, если был перенос из старшего бита (бит 95), иначе 0.
 * (Перенос сохраняется в старшем бите мантиссы.)
 */
int s21_left_shift_one(s21_decimal *d) {
    unsigned int overflow = 0;
    for (int j = 0; j < 3; ++j) {
        unsigned int new_overflow = (d->bits[j] >> 31) & 1;
        d->bits[j] = (d->bits[j] << 1) | overflow;
        overflow = new_overflow;
    }
    /*TO-DO Логика переполнения (НАВЕРНОЕ КАКТО ОПОВЕЩАТЬ НАДО ЧТО ВЫШЛО ЗА ГРАНИЦЫ)
    * пока будет считать как ни в чем не бывало
    */
    if (overflow) {
        d->bits[2] |= (1u << 31);
    }
    return overflow;
}
/*
 * Сдвиг мантиссы влево на shift бит 
 * по принципу bits[2] ← bits[1] ← bits[0]
 * @example shift = 5
 * Вход:  bits[0]=0b1000 0000 0000 0000 0000 0000 0000 0001
 *        bits[1]=0b1000 0000 0000 0000 0000 0000 0000 0001
 *        bits[2]=0b1000 0000 0000 0000 0000 0000 0000 0001
 * 
 * Выход: bits[0]=0b0000 0000 0000 0000 0000 0000 0010 0000  
 *        bits[1]=0b0000 0000 0000 0000 0000 0000 0011 0000  
 *        bits[2]=0b1000 0000 0000 0000 0000 0000 0011 0000  
 * 
 * @example shift = 31
 * Вход:  bits[0]=0b1111 1111 1111 1111 1111 1111 1111 1111
 *        bits[1]=0b0000 0000 0000 0000 0000 0000 0000 0000
 *        bits[2]=0b0000 0000 0000 0000 0000 0000 0000 0000
 * 
 * Выход: bits[0]=0b1000 0000 0000 0000 0000 0000 0000 0000
 *        bits[1]=0b0111 1111 1111 1111 1111 1111 1111 1111
 *        bits[2]=0b0000 0000 0000 0000 0000 0000 0000 0000
 * 
 */
void s21_left_shift_mantissa(s21_decimal *d, int shift) {
    for (int i = 0; i < shift; ++i) {
        s21_left_shift_one(d);
    }
}

/*
 * Сдвиг мантиссы вправо на 1 бит.
 * Возвращает 1, если был перенос из младшего бита (бит 0), иначе 0.
 * (Младший бит теряется, перенос только сигнализирует о потере)
 */
int s21_right_shift_one(s21_decimal *d) {
    unsigned int overflow = 0;
    for (int j = 2; j >= 0; --j) {
        unsigned int new_overflow = d->bits[j] & 1;          // сохраняем младший бит
        d->bits[j] = (d->bits[j] >> 1) | (overflow << 31);  // сдвигаем и добавляем бит справа
        overflow = new_overflow;                            // передаём в следующее слово
    }
    // Восстанавливать нельзя — это изменит значение (округлит вверх, а не вниз)
    // overflow показывает, был ли потерян бит (для округления)
    return overflow;
}

/*
 * Сдвиг мантиссы вправо на shift бит
 * по принципу bits[2] → bits[1] → bits[0]
 */
void s21_right_shift_mantissa(s21_decimal *d, int shift) {
    for (int i = 0; i < shift; ++i) {
        s21_right_shift_one(d);
    }
}

/*
 * Сложение двух мантисс (96 бит) без учёта масштаба и знака
 * 
 * Возвращает:
 *   1 — успешно (результат в d)
 *   0 — переполнение (сумма > 2^96 - 1)
 */
int s21_add_mantissa(s21_decimal a, s21_decimal b, s21_decimal *d) {
    unsigned int overflow = 0;
    s21_decimal result = {0};
    
    // Складываем по словам от младшего к старшему
    for (int i = 0; i < 3; i++) {
        unsigned long long sum = (unsigned long long)a.bits[i] + 
                                  b.bits[i] + 
                                  overflow;
        result.bits[i] = sum & 0xFFFFFFFF;
        overflow = sum >> 32;
    }
    
    // Если после сложения всех слов есть переполнение → результат > 96 бит
    if (overflow) {
        return 0;  // переполнение
    }
    
    *d = result;
    return 1;  // успешно
}
// Умножить мантиссу на 10, вернуть 1 при переполнении
int s21_multiply_by_10(s21_decimal *d) {
    s21_decimal temp = *d;
    
    // Умножение на 10 = (x << 3) + (x << 1)
    s21_left_shift_mantissa(&temp, 3);  // *8
    s21_decimal copy = *d;
    s21_left_shift_mantissa(&copy, 1);  // *2
    
    // Складываем
    return s21_add_mantissa(temp, copy, d);  // нужно реализовать сложение мантисс
}

// Умножение на 10^n (n от 0 до 28)
void s21_power_of_10(s21_decimal *d, int n) {
    for (int i = 0; i < n; i++) {
        if (!s21_multiply_by_10(d)) {
            break;  // переполнение
        }
    }
}


/*
 * Деление мантиссы на 10 (с округлением)
 * Используется при переполнении
 */
void s21_divide_by_10(s21_decimal *d) {
    unsigned int remainder = 0;
    s21_decimal result = {0};
    
    // Делим 96-битное число на 10 (от старших битов к младшим)
    for (int i = 2; i >= 0; i--) {
        unsigned long long num = ((unsigned long long)remainder << 32) | d->bits[i];
        result.bits[i] = num / 10;
        remainder = num % 10;
    }
    
    *d = result;
    
    // Банковское округление
    if (remainder >= 5) {
        // Прибавляем 1 к результату
        s21_decimal one = {0};
        one.bits[0] = 1;
        s21_add_mantissa(*d, one, d);
    }
}

// Обработка переполнения после арифметической операции
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
// Деление мантиссы на 10^scale (целочисленное деление)
void s21_divide_mantissa_by_power_of_10(s21_decimal *d, int scale) {
    for (int i = 0; i < scale; i++) {
        s21_divide_by_10(d);
    }
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

/*
 * Нормализация чисел (приведение к одинаковому масштабу)
 * При переполнении уменьшает масштаб и округляет
 * 
 * Возвращает:
 *   1 — нормализация успешна
 *   0 — ошибка (число не удалось нормализовать)
 */
int s21_normalize(s21_decimal *a, s21_decimal *b) {
    int scale_a = s21_get_scale(*a);
    int scale_b = s21_get_scale(*b);
    
    if (scale_a == scale_b) return 1;
    
    if (scale_a < scale_b) {
        int diff = scale_b - scale_a;
        for (int i = 0; i < diff; i++) {
            if (!s21_multiply_by_10(a)) {
                // Переполнение при умножении → нужно уменьшить масштаб
                if (scale_a > 0) {
                    // Уменьшаем масштаб a и делим мантиссу на 10
                    s21_divide_by_10(a);
                    scale_a--;
                    i--;  // повторяем умножение
                } else {
                    return 0;  // не удалось нормализовать
                }
            }
        }
        s21_set_scale(a, scale_b);
    } else {
        int diff = scale_a - scale_b;
        for (int i = 0; i < diff; i++) {
            if (!s21_multiply_by_10(b)) {
                if (scale_b > 0) {
                    s21_divide_by_10(b);
                    scale_b--;
                    i--;
                } else {
                    return 0;
                }
            }
        }
        s21_set_scale(b, scale_a);
    }
    
    return 1;
}


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

// Сдвиг вправо с банковским округлением
int s21_shift_right_mantissa_with_round(s21_decimal *d, int shift) {
    int remainder = 0;
    for (int i = 0; i < shift; i++) {
        remainder = s21_right_shift_one(d);
    }
    s21_round_mantissa(d, remainder);
    return remainder;
}
// Удаление конечных нулей (для нормализации после операций)
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

/*
 * Сложение или вычитание мантисс в зависимости от знаков
 * 
 * Возвращает:
 *   0 — успешно
 *   1 — переполнение при сложении
 */
int s21_add_sub_mantissa(s21_decimal a, s21_decimal b, s21_decimal *result, int sign, int *overflow) {
    int sign_a = s21_get_sign(a);
    int sign_b = s21_get_sign(b);
    
    // Знаки одинаковые → складываем
    if (sign_a == sign_b) {
        if (!s21_add_mantissa(a, b, result)) {
            return 1;  // переполнение
        }
        s21_set_sign(result, sign_a);
        return 0;
    }
    
    // Знаки разные → вычитаем
    int cmp = s21_compare_abs(a, b);
    
    if (cmp == 0) {
        // Числа равны по модулю → результат 0
        s21_zero_decimal(result);
        return 0;
    }
    
    if (cmp > 0) {
        // a > b по модулю
        s21_sub_mantissa(a, b, result);
        s21_set_sign(result, sign_a);
    } else {
        // b > a по модулю
        s21_sub_mantissa(b, a, result);
        s21_set_sign(result, sign_b);
    }
    
    return 0;
}

/*
 * Сравнение двух decimal чисел
 * Возвращает:
 *   1 — если a > b
 *   0 — если a == b
 *   -1 — если a < b
 */
int s21_compare(s21_decimal a, s21_decimal b) {
    // Получить знаки и масштабы
    int sign_a = s21_get_sign(a);
    int sign_b = s21_get_sign(b);
    
    // Если знаки разные
    if (sign_a != sign_b) {
        if (sign_a == 0 && sign_b == 1) return 1;  // a > b
        if (sign_a == 1 && sign_b == 0) return -1; // a < b
    }
    
    // Нормализация масштабов
    int scale_a = s21_get_scale(a);
    int scale_b = s21_get_scale(b);
    
    if (scale_a != scale_b) {
        s21_normalize(&a, &b);

    }
    
    // Сравнение мантисс
    int cmp = 0;
    if (a.bits[2] != b.bits[2]) {
        cmp = (a.bits[2] > b.bits[2]) ? 1 : -1;
    } else if (a.bits[1] != b.bits[1]) {
        cmp = (a.bits[1] > b.bits[1]) ? 1 : -1;
    } else if (a.bits[0] != b.bits[0]) {
        cmp = (a.bits[0] > b.bits[0]) ? 1 : -1;
    }
    
    // Если оба отрицательные — результат инвертировать
    if (sign_a == 1) {
        cmp = -cmp;
    }
    
    return cmp;
}
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
// =========================== АРИФМЕТИЧЕСКИЕ ОПЕРАТОРЫ ===========================

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
    return S21_OK;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    s21_decimal temp_value;
    
    // Меняем знак у value_2
    s21_negate(value_2, &temp_value);
    
    // Складываем
    return s21_add(value_1, temp_value, result);
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    (void)value_1;
    (void)value_2;
    (void)result;

    /*
    * 1. Проверить и обнулить result
    * 2. Проверка на ноль value_1 и value_2
    *    └─ При равеностве чего-то 0 → записать в s21_decimal result 0
    * 3. Определение знака результата sign_result = sign1 ^ sign2 (XOR)
    *    └─ 1 — разные, 0 — одинаковые
    * 4. Определение масштаба результата scale_result = scale1 + scale2
    * 5. Умножение мантисс (96 бит × 96 бит = 192 бита)
    *    - Использовать массив из 6 uint32_t для хранения 192-битного результата
    * 6. Приведение к 96 битам с уменьшением масштаба
    * 7. Банковское округление
    * 8. Проверка переполнения результата
    * 9. Запись результата
    * 10. Вернуть S21_OK
    */
    return S21_OK;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    (void)value_1;
    (void)value_2;
    (void)result;
    return S21_OK;
}

// =========================== ОПЕРАТОРЫ СРАВНЕНИЯ  ===========================

int s21_is_less(s21_decimal a, s21_decimal b) {
    return s21_compare(a, b) == -1;
}

int s21_is_less_or_equal(s21_decimal a, s21_decimal b) {
    int cmp = s21_compare(a, b);
    return cmp == -1 || cmp == 0;
}

int s21_is_greater(s21_decimal a, s21_decimal b) {
    return s21_compare(a, b) == 1;
}

int s21_is_greater_or_equal(s21_decimal a, s21_decimal b) {
    int cmp = s21_compare(a, b);
    return cmp == 1 || cmp == 0;
}

int s21_is_equal(s21_decimal a, s21_decimal b) {
    return s21_compare(a, b) == 0;
}

int s21_is_not_equal(s21_decimal a, s21_decimal b) {
    return s21_compare(a, b) != 0;
}

// =========================== ПРЕОБРАЗОВАТЕЛИ ===========================

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
    (void)src;
    (void)dst;
    /*
 * Преобразование int в decimal
 * 
 * Последовательность действий:
 * 1. Обнулить dst (все биты = 0)
 * 2. Если src отрицательный:
 *      - Установить знак (бит 31 = 1)
 *      - Взять абсолютное значение (-src)
 *    Иначе:
 *      - Знак = 0
 *      - Взять src как есть
 * 3. Записать абсолютное значение в bits[0]
 * 4. Масштаб = 0 (биты 16-23 = 0)
 * 5. Вернуть S21_OK
 */
    return S21_OK;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
    (void)src;
    (void)dst;
    /*
    * Преобразование float в decimal
    * 
    * Последовательность действий:
    * 1. Проверить особые случаи:
    *      - Если src == inf или nan → ошибка
    *      - Если |src| < 1e-28 → ошибка, dst = 0
    *      - Если |src| > 79,228,162,514,264,337,593,543,950,335 → ошибка
    * 2. Разобрать float на составляющие:
    *      - sign = (бит 31) ? 1 : 0
    *      - exponent = (биты 23-30) - 127
    *      - mantissa = (биты 0-22) | 0x800000 (добавить скрытую единицу)
    * 3. Преобразовать в десятичное число:
    *      - Вычислить целую и дробную части
    *      - Определить масштаб
    * 4. Округлить до 7 значащих цифр (банковское округление)
    * 5. Записать в dst (мантиссу в bits[0-2], масштаб в bits[16-23], знак в bit31)
    * 6. Вернуть S21_OK (или 1 при ошибке)
    */
    return S21_OK;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
    (void)src;
    (void)dst;
    /*
 * Преобразование decimal в int
 * 
 * Последовательность действий:
 * 1. Получить знак и масштаб из src
 * 2. Взять мантиссу (bits[0-2])
 * 3. Отбросить дробную часть:
 *      - Если масштаб > 0:
 *          - Разделить мантиссу на 10^scale (целочисленное деление)
 * 4. Проверить, помещается ли результат в int:
 *      - Если значение > 2,147,483,647 → ошибка
 *      - Если значение < -2,147,483,648 → ошибка
 * 5. Применить знак
 * 6. Записать в *dst
 * 7. Вернуть S21_OK (или 1 при ошибке)
 */
    return S21_OK;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
    (void)src;
    (void)dst;
    /*
 * Преобразование decimal в float
 * 
 * Последовательность действий:
 * 1. Получить знак, мантиссу и масштаб из src
 * 2. Вычислить значение:
 *      - value = (мантисса) / 10^scale
 * 3. Применить знак:
 *      - Если знак = 1 → value = -value
 * 4. Проверить, помещается ли в float:
 *      - Если |value| > 3.402823e+38 → ошибка
 *      - Если |value| < 1.175494e-38 → округлить до 0
 * 5. Записать в *dst
 * 6. Вернуть S21_OK (или 1 при ошибке)
 */
    return S21_OK;
}

// =========================== ДРУГИЕ ФУНКЦИИ ===========================

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


/*==============================================================================*/


/* ============================================================================= */

/* 
void print_decimal(s21_decimal d) {
       // Выводим знак
    if (s21_get_sign(d)) {
        putchar('-');
    }
    // Создаём копию
    s21_decimal temp = d;
    s21_set_scale(&temp, 0);
    s21_set_sign(&temp, 0);
    
    // Массив для цифр
    char digits[100] = {0};
    int count = 0;
    
    // Делим на 10, пока не станет 0
    while (!s21_is_zero(temp)) {
        int remainder = s21_divide_by_10_with_remainder(&temp);
        digits[count++] = '0' + remainder;
    }
    
    if (count == 0) {
        printf("0");
        return;
    }
    
    // Выводим в обратном порядке
    for (int i = count - 1; i >= 0; i--) {
        putchar(digits[i]);
    }
}
int main()
{
    s21_decimal max = {0};
    s21_decimal min = {0};
    max.bits[0] = 0xFFFFFFFF;
    max.bits[1] = 0xFFFFFFFF;
    max.bits[2] = 0xFFFFFFFF;
    min = max;
    s21_set_sign(&min,1);
    print_decimal(max);
    putchar('\n');
    print_decimal(min);
    return 0;
} */