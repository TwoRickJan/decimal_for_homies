#include "../s21_decimal.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
/* 
├── s21_decimal_math.c
│   # ===== ВСЯ МАТЕМАТИКА =====
│
│   # --- mantissa (96 бит) ---
│   static int s21_add_mantissa
│   static int s21_sub_mantissa
│   static int s21_compare_abs
│   static int s21_is_greater_mantissa
│   static int s21_is_less_mantissa
│
│   # --- 192 бит (mul/div) ---
│   static void         s21_add_192
│   static void         s21_mul_192_by_32
│   static unsigned int s21_div_192_by_32
│
│   # --- работа с 10 ---
│   static int  s21_multiply_by_10
│   static void s21_power_of_10
│   static void s21_divide_by_10
│   static int  s21_divide_by_10_with_remainder
│
│   # --- rounding ---
│   static void s21_round_mantissa
│   static void s21_bank_round
│

 */

/*
 *              --- mantissa (96 бит) ---
 */ 

/*
 * Сложение двух мантисс (96 бит) без учёта масштаба и знака
 * 
 * Возвращает:
 *   0 S21_OK — успешно (результат в d)
 *   1 S21_TOO_LARGE — переполнение (сумма > 2^96 - 1) 
 */
int s21_add_mantissa(s21_decimal a, s21_decimal b, s21_decimal *d) {
    unsigned int overflow = 0;
    s21_decimal result = {0};
    // Складываем по словам от младшего к старшему
    // для справки unsigned long long 2 слова 64 бита
    //  в него складываем результат суммы  a.bits[i]+a.bits[i] , может стать 33 бита
    // все что старше 32 битов сохраняем в overflow
    // далее приводим его обратно к формату 32 бита
    
    for (int i = 0; i < 3; i++) {
        unsigned long long sum = (unsigned long long)a.bits[i] + 
                                  b.bits[i] + 
                                  overflow;
        result.bits[i] = sum & S21_MASK_32BIT; //— отбрасываем всё, что не влезло в 32 бита
        overflow = sum >> 32; // сохраняем превшение больше 32 бит
    }
    
    // Если после сложения всех слов есть переполнение → результат > 96 бит
    if (overflow) {
        return S21_TOO_LARGE;  // переполнение
    }
    
    *d = result;
    return S21_OK;  // успешно
}

/*
 * Сложение двух мантисс (96 бит) без учёта масштаба и знака
 * 
 * Возвращает:
 *   0 S21_OK — успешно (результат в d)
 *   1 S21_TOO_LARGE — переполнение (сумма > 2^96 - 1) 
 */
int s21_add_mantissa(s21_decimal a, s21_decimal b, s21_decimal *d) {
    unsigned int overflow = 0;
    s21_decimal result = {0};
    // Складываем по словам от младшего к старшему
    // для справки unsigned long long 2 слова 64 бита
    //  в него складываем результат суммы  a.bits[i]+a.bits[i] , может стать 33 бита
    // все что старше 32 битов сохраняем в overflow
    // далее приводим его обратно к формату 32 бита
    
    for (int i = 0; i < 3; i++) {
        unsigned long long sum = (unsigned long long)a.bits[i] + 
                                  b.bits[i] + 
                                  overflow;
        result.bits[i] = sum & S21_MASK_32BIT; //— отбрасываем всё, что не влезло в 32 бита
        overflow = sum >> 32; // сохраняем превшение больше 32 бит
    }
    
    // Если после сложения всех слов есть переполнение → результат > 96 бит
    if (overflow) {
        return S21_TOO_LARGE;  // переполнение
    }
    
    *d = result;
    return S21_OK;  // успешно
}

/* Умножить мантиссу на 10, вернуть 1 при переполнении
Релизует формулу x * 10 = x * (8 + 2) = x*8 + x*2 = x * 2^3 + x * 2^2
Возвращает: 
0 S21_OK — успешно (результат в d)
1 S21_TOO_LARGE — переполнение (сумма > 2^96 - 1)*/
int s21_multiply_by_10(s21_decimal *d) {
    s21_decimal temp = *d;
    
    // Умножение на 10 = (x << 3) + (x << 1)
    // x * 10 = x * (8 + 2) = x*8 + x*2
    s21_left_shift_mantissa(&temp, 3);  // (x << 3) = x*8
    s21_decimal copy = *d;
    s21_left_shift_mantissa(&copy, 1);  // (x << 1) = x*2
    
    // Складываем (x << 3) + (x << 1)
    return s21_add_mantissa(temp, copy, d);  
}

// Умножение на 10^n (n от 0 до 28)
void s21_power_of_10(s21_decimal *d, int n) {
    for (int i = 0; i < n; i++) {
        if (s21_multiply_by_10(d) == S21_TOO_LARGE) {
            break;  // переполнение
        }
    }
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

/*
 * Сложение или вычитание мантисс в зависимости от знаков
 * 
 * Возвращает:
 *   0 S21_OK — успешно
 *   1 S21_TOO_LARGE — переполнение при сложении
 */
int s21_add_sub_mantissa(s21_decimal a, s21_decimal b, s21_decimal *result, int sign, int *overflow) {
    int sign_a = s21_get_sign(a);
    int sign_b = s21_get_sign(b);
    
    // Знаки одинаковые → складываем
    if (sign_a == sign_b) {
        if (s21_add_mantissa(a, b, result)) {
            return S21_TOO_LARGE;  // переполнение
        }
        s21_set_sign(result, sign_a);
        return S21_OK;
    }
    
    // Знаки разные → вычитаем
    int cmp = s21_compare_abs(a, b);
    
    if (cmp == 0) {
        // Числа равны по модулю → результат 0
        s21_zero_decimal(result);
        return S21_OK;
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
    
    return S21_OK;
}

// Деление мантиссы на 10^scale (целочисленное деление)
void s21_divide_mantissa_by_power_of_10(s21_decimal *d, int scale) {
    for (int i = 0; i < scale; i++) {
        s21_divide_by_10(d);
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