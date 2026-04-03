#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

// Полезные маски для работы с decimal
#define S21_MASK_8BIT  0xFF      // 0b11111111                     - 8 бит (для масштаба)
#define S21_MASK_16BIT 0xFFFF    // 0b1111111111111111             - 16 бит (для резерва)
#define S21_MASK_9BIT  0x1FF     // 0b111111111                    - 9 бит
#define S21_MASK_32BIT 0xFFFFFFFF // 0b11111111111111111111111111111111 - 32 бита
#define S21_MASK_BIT31 (1 << 31) // 0b10000000000000000000000000000000 - бит знака


typedef struct {
    int bits[4];  // 32*4 => ИСПРАВИТЬ НА unsigned int сразу со связями инче не сложно днбажить.
} s21_decimal;

// Коды возврата для арифметических операций
#define S21_OK 0
#define S21_TOO_LARGE 1      // число слишком велико или равно бесконечности
#define S21_TOO_SMALL 2      // число слишком мало или равно отрицательной бесконечности
#define S21_DIVISION_BY_ZERO 3
#define S21_NORMOLIZATION_ERROR 4

// Коды возврата для операций сравнения
#define S21_FALSE 0
#define S21_TRUE 1

// Арифметические операторы
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// Операторы сравнения
int s21_is_less(s21_decimal a, s21_decimal b);
int s21_is_less_or_equal(s21_decimal a, s21_decimal b);
int s21_is_greater(s21_decimal a, s21_decimal b);
int s21_is_greater_or_equal(s21_decimal a, s21_decimal b);
int s21_is_equal(s21_decimal a, s21_decimal b);
int s21_is_not_equal(s21_decimal a, s21_decimal b);

// Преобразователи
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// Другие функции
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

// Банковское округление
//void s21_bank_round(s21_decimal *value, int remainder);
// =========================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ (ПОТОМ СДЕЛАТЬ STATIC) ===========================
int s21_get_sign(s21_decimal d);
void s21_set_sign(s21_decimal *d, int sign);
int s21_get_scale(s21_decimal d);
void s21_set_scale(s21_decimal *d, int scale);
void s21_zero_decimal(s21_decimal *d);
int s21_compare_abs(s21_decimal a, s21_decimal b);
int s21_sub_mantissa(s21_decimal a, s21_decimal b, s21_decimal *result);
int s21_is_zero(s21_decimal d);
int s21_handle_zeros(s21_decimal *a, s21_decimal *b, s21_decimal *result);
void s21_finalize_result(s21_decimal *temp, s21_decimal *result, int max_scale, int sign);
int s21_divide_by_10_with_remainder(s21_decimal *d);
void s21_get_mantissa(s21_decimal d, unsigned int *low, unsigned int *mid, unsigned int *high);
void s21_set_mantissa(s21_decimal *d, unsigned int low, unsigned int mid, unsigned int high);
int s21_is_greater_mantissa(s21_decimal a, s21_decimal b);
int s21_is_less_mantissa(s21_decimal a, s21_decimal b);
void s21_add_192(unsigned int *a, unsigned int *b, unsigned int *result);
void s21_mul_192_by_32(unsigned int *a, unsigned int b, unsigned int *result);
unsigned int s21_div_192_by_32(unsigned int *a, unsigned int b, unsigned int *result);
int s21_get_bit(s21_decimal d, int index);
void s21_set_bit(s21_decimal *d, int index, int bit);
int s21_left_shift_one(s21_decimal *d);
void s21_left_shift_mantissa(s21_decimal *d, int shift);
int s21_right_shift_one(s21_decimal *d);
void s21_right_shift_mantissa(s21_decimal *d, int shift);
int s21_add_mantissa(s21_decimal a, s21_decimal b, s21_decimal *d);
int s21_multiply_by_10(s21_decimal *d);
void s21_power_of_10(s21_decimal *d, int n);
void s21_divide_by_10(s21_decimal *d);
int s21_handle_operation_overflow(int was_overflow, s21_decimal *temp, int *max_scale, int sign);
void s21_divide_mantissa_by_power_of_10(s21_decimal *d, int scale);
void s21_round_mantissa(s21_decimal *d, int remainder);
int s21_normalize(s21_decimal *a, s21_decimal *b);
int s21_shift_right_mantissa_with_round(s21_decimal *d, int shift);
void s21_remove_trailing_zeros(s21_decimal *d);
int s21_add_sub_mantissa(s21_decimal a, s21_decimal b, s21_decimal *result, int sign, int *overflow);
int s21_compare(s21_decimal a, s21_decimal b, int *cmp);
void s21_bank_round(s21_decimal *value, int remainder);
#endif  // S21_DECIMAL_H

/*// =========================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ===========================
// Уровень 0-1: базовые (используются везде)

// s21_get_sign - используется во всех арифметических и сравнительных функциях: s21_add, s21_sub, s21_mul, s21_div, s21_is_less, s21_is_equal, s21_compare, s21_negate, s21_floor, s21_round, s21_truncate
int s21_get_sign(s21_decimal d);

// s21_set_sign - используется в: s21_add, s21_sub, s21_mul, s21_div, s21_negate, s21_floor, s21_round, s21_truncate, s21_add_sub_mantissa, s21_finalize_result
void s21_set_sign(s21_decimal *d, int sign);

// s21_get_scale - используется в: s21_add, s21_sub, s21_mul, s21_div, s21_compare, s21_normalize, s21_floor, s21_round, s21_truncate, s21_from_float_to_decimal, s21_from_decimal_to_float
int s21_get_scale(s21_decimal d);

// s21_set_scale - используется в: s21_add, s21_sub, s21_mul, s21_div, s21_normalize, s21_floor, s21_round, s21_truncate, s21_from_float_to_decimal, s21_finalize_result
void s21_set_scale(s21_decimal *d, int scale);

// s21_zero_decimal - используется в: s21_add, s21_sub, s21_mul, s21_div, s21_from_int_to_decimal, s21_from_float_to_decimal, s21_negate, s21_floor, s21_round, s21_truncate, s21_handle_zeros
void s21_zero_decimal(s21_decimal *d);

// s21_is_zero - используется в: s21_add, s21_sub, s21_mul, s21_div, s21_compare, s21_handle_zeros, s21_from_decimal_to_int, s21_floor, s21_round, s21_truncate, print_decimal
int s21_is_zero(s21_decimal d);

// =========================== УРОВЕНЬ 2: РАБОТА С МАНТИССОЙ ===========================

// s21_compare_abs - используется в: s21_add_sub_mantissa, s21_compare
int s21_compare_abs(s21_decimal a, s21_decimal b);

// s21_sub_mantissa - используется в: s21_add_sub_mantissa
int s21_sub_mantissa(s21_decimal a, s21_decimal b, s21_decimal *result);

// s21_add_mantissa - используется в: s21_multiply_by_10, s21_add_sub_mantissa, s21_round_mantissa, s21_bank_round
int s21_add_mantissa(s21_decimal a, s21_decimal b, s21_decimal *d);

// s21_get_mantissa - используется в: s21_mul, s21_div
void s21_get_mantissa(s21_decimal d, unsigned int *low, unsigned int *mid, unsigned int *high);

// s21_set_mantissa - используется в: s21_mul, s21_div
void s21_set_mantissa(s21_decimal *d, unsigned int low, unsigned int mid, unsigned int high);

// s21_is_greater_mantissa - используется в: s21_compare (альтернатива compare_abs)
int s21_is_greater_mantissa(s21_decimal a, s21_decimal b);

// s21_is_less_mantissa - используется в: s21_compare (альтернатива compare_abs)
int s21_is_less_mantissa(s21_decimal a, s21_decimal b);

// =========================== УРОВЕНЬ 3: БИТОВЫЕ ОПЕРАЦИИ ===========================

// s21_get_bit - используется в: s21_mul, s21_div (для поразрядных операций)
int s21_get_bit(s21_decimal d, int index);

// s21_set_bit - используется в: s21_mul, s21_div
void s21_set_bit(s21_decimal *d, int index, int bit);

// s21_left_shift_one - используется в: s21_left_shift_mantissa
int s21_left_shift_one(s21_decimal *d);

// s21_left_shift_mantissa - используется в: s21_multiply_by_10, s21_power_of_10, s21_mul, s21_shift_right_mantissa_with_round
void s21_left_shift_mantissa(s21_decimal *d, int shift);

// s21_right_shift_one - используется в: s21_right_shift_mantissa
int s21_right_shift_one(s21_decimal *d);

// s21_right_shift_mantissa - используется в: s21_shift_right_mantissa_with_round, s21_divide_mantissa_by_power_of_10
void s21_right_shift_mantissa(s21_decimal *d, int shift);

// =========================== УРОВЕНЬ 4: РАБОТА С 10 ===========================

// s21_multiply_by_10 - используется в: s21_power_of_10, s21_normalize
int s21_multiply_by_10(s21_decimal *d);

// s21_power_of_10 - используется в: s21_normalize, s21_from_float_to_decimal
void s21_power_of_10(s21_decimal *d, int n);

// s21_divide_by_10_with_remainder - используется в: s21_divide_by_10, s21_divide_mantissa_by_power_of_10, print_decimal
int s21_divide_by_10_with_remainder(s21_decimal *d);

// s21_divide_by_10 - используется в: s21_normalize, s21_handle_operation_overflow, s21_divide_mantissa_by_power_of_10
void s21_divide_by_10(s21_decimal *d);

// s21_round_mantissa - используется в: s21_divide_by_10, s21_shift_right_mantissa_with_round
void s21_round_mantissa(s21_decimal *d, int remainder);

// s21_bank_round - используется в: s21_divide_by_10, s21_round, s21_from_float_to_decimal
void s21_bank_round(s21_decimal *value, int remainder);

// s21_divide_mantissa_by_power_of_10 - используется в: s21_truncate, s21_floor, s21_round, s21_from_decimal_to_int
void s21_divide_mantissa_by_power_of_10(s21_decimal *d, int scale);

// s21_shift_right_mantissa_with_round - используется в: s21_div, s21_divide_mantissa_by_power_of_10
int s21_shift_right_mantissa_with_round(s21_decimal *d, int shift);

// =========================== УРОВЕНЬ 5: НОРМАЛИЗАЦИЯ И ОБРАБОТКА ===========================

// s21_handle_zeros - используется в: s21_add, s21_sub
int s21_handle_zeros(s21_decimal *a, s21_decimal *b, s21_decimal *result);

// s21_normalize - используется в: s21_add, s21_sub, s21_compare
int s21_normalize(s21_decimal *a, s21_decimal *b);

// s21_remove_trailing_zeros - используется в: s21_div, s21_from_float_to_decimal
void s21_remove_trailing_zeros(s21_decimal *d);

// s21_handle_operation_overflow - используется в: s21_add, s21_sub
int s21_handle_operation_overflow(int was_overflow, s21_decimal *temp, int *max_scale, int sign);

// s21_finalize_result - используется в: s21_add, s21_sub
void s21_finalize_result(s21_decimal *temp, s21_decimal *result, int max_scale, int sign);

// =========================== УРОВЕНЬ 6: КОМБИНИРОВАННЫЕ ОПЕРАЦИИ ===========================

// s21_add_sub_mantissa - используется в: s21_add, s21_sub
int s21_add_sub_mantissa(s21_decimal a, s21_decimal b, s21_decimal *result, int sign, int *overflow);

// s21_compare - используется в: s21_is_less, s21_is_equal, s21_is_greater, s21_is_less_or_equal, s21_is_greater_or_equal, s21_is_not_equal
int s21_compare(s21_decimal a, s21_decimal b, int *cmp);

// =========================== УРОВЕНЬ 7: 192-БИТНЫЕ ОПЕРАЦИИ (ДЛЯ MUL/DIV) ===========================

// s21_add_192 - используется в: s21_mul
void s21_add_192(unsigned int *a, unsigned int *b, unsigned int *result);

// s21_mul_192_by_32 - используется в: s21_mul
void s21_mul_192_by_32(unsigned int *a, unsigned int b, unsigned int *result);

// s21_div_192_by_32 - используется в: s21_div
unsigned int s21_div_192_by_32(unsigned int *a, unsigned int b, unsigned int *result);*/
/* ## Уровни функций от базовых к сложным

---

### Уровень 0 — Базовые битовые операции (самые низкоуровневые)
```c
int s21_get_bit(s21_decimal d, int index);
void s21_set_bit(s21_decimal *d, int index, int bit);
int s21_left_shift_one(s21_decimal *d);
void s21_left_shift_mantissa(s21_decimal *d, int shift);
int s21_right_shift_one(s21_decimal *d);
void s21_right_shift_mantissa(s21_decimal *d, int shift);
```

---

### Уровень 1 — Работа со структурой decimal
```c
int s21_get_sign(s21_decimal d);
void s21_set_sign(s21_decimal *d, int sign);
int s21_get_scale(s21_decimal d);
void s21_set_scale(s21_decimal *d, int scale);
void s21_zero_decimal(s21_decimal *d);
int s21_is_zero(s21_decimal d);
void s21_get_mantissa(s21_decimal d, unsigned int *low, unsigned int *mid, unsigned int *high);
void s21_set_mantissa(s21_decimal *d, unsigned int low, unsigned int mid, unsigned int high);
```

---

### Уровень 2 — Арифметика мантисс (96 бит)
```c
int s21_compare_abs(s21_decimal a, s21_decimal b);
int s21_is_greater_mantissa(s21_decimal a, s21_decimal b);
int s21_is_less_mantissa(s21_decimal a, s21_decimal b);
int s21_add_mantissa(s21_decimal a, s21_decimal b, s21_decimal *d);
int s21_sub_mantissa(s21_decimal a, s21_decimal b, s21_decimal *result);
```

---

### Уровень 3 — Работа с десятичным масштабом (умножение/деление на 10)
```c
int s21_multiply_by_10(s21_decimal *d);
void s21_power_of_10(s21_decimal *d, int n);
int s21_divide_by_10_with_remainder(s21_decimal *d);
void s21_divide_by_10(s21_decimal *d);
void s21_round_mantissa(s21_decimal *d, int remainder);
void s21_bank_round(s21_decimal *value, int remainder);
void s21_divide_mantissa_by_power_of_10(s21_decimal *d, int scale);
int s21_shift_right_mantissa_with_round(s21_decimal *d, int shift);
```

---

### Уровень 4 — Нормализация и вспомогательные операции
```c
int s21_handle_zeros(s21_decimal *a, s21_decimal *b, s21_decimal *result);
int s21_normalize(s21_decimal *a, s21_decimal *b);
void s21_remove_trailing_zeros(s21_decimal *d);
void s21_finalize_result(s21_decimal *temp, s21_decimal *result, int max_scale, int sign);
int s21_handle_operation_overflow(int was_overflow, s21_decimal *temp, int *max_scale, int sign);
```

---

### Уровень 5 — Операции с 192 битами (для умножения)
```c
void s21_add_192(unsigned int *a, unsigned int *b, unsigned int *result);
void s21_mul_192_by_32(unsigned int *a, unsigned int b, unsigned int *result);
unsigned int s21_div_192_by_32(unsigned int *a, unsigned int b, unsigned int *result);
```

---

### Уровень 6 — Комбинированные операции
```c
int s21_add_sub_mantissa(s21_decimal a, s21_decimal b, s21_decimal *result, int sign, int *overflow);
int s21_compare(s21_decimal a, s21_decimal b);
```

---

### Уровень 7 — Публичные функции API (самые сложные)
```c
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_is_less(s21_decimal a, s21_decimal b);
int s21_is_less_or_equal(s21_decimal a, s21_decimal b);
int s21_is_greater(s21_decimal a, s21_decimal b);
int s21_is_greater_or_equal(s21_decimal a, s21_decimal b);
int s21_is_equal(s21_decimal a, s21_decimal b);
int s21_is_not_equal(s21_decimal a, s21_decimal b);
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);
```

---

## Иерархия зависимостей

```
Уровень 0 (биты)
    ↓
Уровень 1 (структура)
    ↓
Уровень 2 (арифметика мантисс)
    ↓
Уровень 3 (работа с 10)
    ↓
Уровень 4 (нормализация)
    ↓
Уровень 5 (192 бита для mul/div)
    ↓
Уровень 6 (комбинированные)
    ↓
Уровень 7 
``` */