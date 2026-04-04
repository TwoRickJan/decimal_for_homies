#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H
#include <stdio.h>
/**
 * @brief Маски для работы с битами.
 */
/* === МАСКИ === */
#define S21_MASK_8BIT  0xFF      // Маска для 8-битного значения
#define S21_MASK_16BIT 0xFFFF    // Маска для 16-битного значения
#define S21_MASK_9BIT  0x1FF     // Маска для 9-битного значения
#define S21_MASK_32BIT 0xFFFFFFFF// Маска для 32-битного значения
#define S21_MASK_BIT31 (1 << 31) // Маска для старшего бита

/**
 * @brief Структуры данных.
 */
/* === ТИПЫ === */
typedef struct {
    unsigned bits[4]; // Массив из 4-х беззнаковых целых чисел для хранения мантиссы и знака
} s21_decimal;

typedef struct {
    unsigned bits[8]; // Массив из 8-ми беззнаковых целых чисел для больших операций
} s21_u_int_192;

/**
 * @brief Коды возврата функций.
 */
/* === КОДЫ ВОЗВРАТА === */
#define S21_OK 0                // Успешное выполнение
#define S21_TOO_LARGE 1         // Значение слишком велико для представления
#define S21_TOO_SMALL 2         // Значение слишком мало для представления
#define S21_DIVISION_BY_ZERO 3  // Деление на ноль
#define S21_NORMOLIZATION_ERROR 4// Ошибка нормализации

#define S21_FALSE 0 // Ложь
#define S21_TRUE 1  // Истина

/**
 * @brief Арифметические операции.
 */
/* === API: АРИФМЕТИКА === */
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result); // Сложение двух децималов
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result); // Вычитание второго децимала из первого
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result); // Умножение двух децималов
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result); // Деление первого децимала на второй
int s21_negate(s21_decimal value, s21_decimal *result);                     // Инверсия знака

/**
 * @brief Сравнения.
 */
/* === API: СРАВНЕНИЕ === */
int s21_is_less(s21_decimal a, s21_decimal b);            // Проверка меньше
int s21_is_less_or_equal(s21_decimal a, s21_decimal b);    // Проверка меньше или равно
int s21_is_greater(s21_decimal a, s21_decimal b);         // Проверка больше
int s21_is_greater_or_equal(s21_decimal a, s21_decimal b);// Проверка больше или равно
int s21_is_equal(s21_decimal a, s21_decimal b);            // Проверка равенства
int s21_is_not_equal(s21_decimal a, s21_decimal b);        // Проверка неравенства

/**
 * @brief Конвертации.
 */
/* === API: КОНВЕРТАЦИЯ === */
int s21_from_int_to_decimal(int src, s21_decimal *dst);     // Преобразование из int в decimal
int s21_from_float_to_decimal(float src, s21_decimal *dst);// Преобразование из float в decimal
int s21_from_decimal_to_int(s21_decimal src, int *dst);    // Преобразование из decimal в int
int s21_from_decimal_to_float(s21_decimal src, float *dst);// Преобразование из decimal в float

/**
 * @brief Округления.
 */
/* === API: ОКРУГЛЕНИЕ === */
int s21_floor(s21_decimal value, s21_decimal *result);      // Округление вниз
int s21_round(s21_decimal value, s21_decimal *result);      // Округление до ближайшего целого
int s21_truncate(s21_decimal value, s21_decimal *result);   // Обрезание до целой части

/**
 * @brief Вспомогательные функции для работы со структурой.
 */
/* === HELPER: СТРУКТУРА === */
int s21_get_sign(s21_decimal d);                 // Получение знака
void s21_set_sign(s21_decimal *d, int sign);      // Установка знака
int s21_get_scale(s21_decimal d);                // Получение масштаба
void s21_set_scale(s21_decimal *d, int scale);    // Установка масштаба
void s21_zero_decimal(s21_decimal *d);           // Обнуление децимала
int s21_is_zero(s21_decimal d);                  // Проверка на ноль

/**
 * @brief Вспомогательные функции для работы с битами.
 */
/* === HELPER: БИТЫ === */
void s21_get_mantissa(s21_decimal d, unsigned int *low, unsigned int *mid, unsigned int *high); // Получение мантиссы
void s21_set_mantissa(s21_decimal *d, unsigned int low, unsigned int mid, unsigned int high);// Установка мантиссы

/**
 * @brief Вспомогательные функции для сдвига мантиссы.
 */
int s21_left_shift_one(s21_decimal *d);           // Сдвиг мантиссы влево на один разряд
void s21_left_shift_mantissa(s21_decimal *d, int shift);// Сдвиг мантиссы влево на указанное количество разрядов
int s21_right_shift_one(s21_decimal *d);          // Сдвиг мантиссы вправо на один разряд

/**
 * @brief Вспомогательные функции для работы с мантиссой.
 */
/* === HELPER: МАНТИССА === */
int s21_add_mantissa(s21_decimal a, s21_decimal b, s21_decimal *d);// Сложение мантисс
int s21_sub_mantissa(s21_decimal a, s21_decimal b, s21_decimal *result); // Вычитание мантисс
int s21_compare_abs(s21_decimal a, s21_decimal b);              // Сравнение абсолютных значений мантисс

/**
 * @brief Вспомогательные функции для работы с 192 битами.
 */
/* === HELPER: 192 БИТА === */
void s21_add_192(unsigned int *a, unsigned int *b, unsigned int *result); // Сложение двух 192-битных чисел
void s21_mul_192_by_32(unsigned int *a, unsigned int b, unsigned int *result);// Умножение 192-битного числа на 32-битное
unsigned int s21_div_192_by_32(unsigned int *a, unsigned int b, unsigned int *result);// Деление 192-битного числа на 32-битное

/**
 * @brief Вспомогательные функции для работы с основой 10.
 */
/* === HELPER: РАБОТА С 10 === */
int s21_multiply_by_10(s21_decimal *d);          // Умножение децимала на 10
void s21_power_of_10(s21_decimal *d, int n);      // Возведение децимала в степень 10
void s21_divide_by_10(s21_decimal *d);            // Деление децимала на 10
int s21_divide_by_10_with_remainder(s21_decimal *d);// Деление децимала на 10 с остатком

/**
 * @brief Вспомогательные функции для нормализации.
 */
/* === HELPER: НОРМАЛИЗАЦИЯ === */
int s21_normalize(s21_decimal *a, s21_decimal *b); // Нормализация двух децималов
void s21_remove_trailing_zeros(s21_decimal *d);    // Удаление конечных нулей из мантиссы
int s21_handle_zeros(s21_decimal *a, s21_decimal *b, s21_decimal *result);// Обработка нулевых значений
int s21_handle_operation_overflow(int was_overflow, s21_decimal *temp, int *max_scale, int sign); // Обработка переполнения операции
void s21_finalize_result(s21_decimal *temp, s21_decimal *result, int max_scale, int sign);// Завершение результата после операции
void s21_divide_mantissa_by_power_of_10(s21_decimal *d, int scale); // Деление мантиссы на степень 10

/**
 * @brief Вспомогательные функции для округления.
 */
/* === HELPER: ОКРУГЛЕНИЕ === */
void s21_round_mantissa(s21_decimal *d, int scale); // Округление мантиссы до указанного масштаба

/**
 * @brief Вспомогательные функции для комбинированных операций.
 */
/* === HELPER: КОМБИНИРОВАННЫЕ ОПЕРАЦИИ === */
int s21_add_sub_mantissa(s21_decimal a, s21_decimal b, s21_decimal *result, int sign, int *overflow); // Сложение/вычитание мантисс
int s21_compare(s21_decimal a, s21_decimal b, int *cmp);         // Сравнение двух чисел
int s21_shift_right_mantissa_with_round(s21_decimal *d, int shift); // Сдвиг вправо с округлением
void s21_bank_round(s21_decimal *value, int remainder);           // Банковское округление

#endif // S21_DECIMAL_H
