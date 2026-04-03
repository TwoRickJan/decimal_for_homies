#ifndef _SRC_S21_DECIMAL_H
#define _SRC_S21_DECIMAL_H
#include <math.h>
#include <stdint.h>
#include <stdio.h>
// Если в числе типа decimal есть дробная часть, то её следует отбросить
// (например, 0.9 преобразуется 0)
#define RESET "\033[0m"
#define RED "\033[1;31m"
#define YELLOW "\033[1;33m"
#define WHITE "\033[1;37m"
#define GREEN "\033[1;32m"
#define PURPLE "\033[1;35m"
#define CYAN "\033[1;36m"

#define START_LOW 0
#define START_MID 32
#define START_HIGHT 64
#define START_INFO 96

#define MAX_DECIMAL powl(2.0, 96)
#define MIN_DECIMAL -1 * MAX_DECIMAL

#define MAX_POW 28
#define FLOAT_ACCURACY 7
#define S21_MAX_UINT 4294967295

// typedef enum {
//   // S21_NULL,
//   S21_NORMAL,
//   S21_PLUS_INF,
//   S21_MINUS_INF,
//   S21_NAN,
//   S21_DOP
// } State;

union float_bits {
    uint32_t bits;
    float num_f;
};
typedef struct s21_decimal {
  unsigned int bits[4];
  // State state;
} s21_decimal;

int s21_add(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result);  // Сложение debug ++
int s21_sub(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result);  // Вычитание debug ++
int s21_mul(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result);  // Умножение
int s21_div(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result);  // Деление
int s21_mod(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result);  // Остаток от деления
int s21_is_less(s21_decimal, s21_decimal);  //Меньше debug ++
int s21_is_less_or_equal(s21_decimal, s21_decimal);  //Меньше или равно debug ++
int s21_is_greater(s21_decimal, s21_decimal);  // Больше debug ++
int s21_is_greater_or_equal(s21_decimal,
                            s21_decimal);  // Больше или равно debug ++
int s21_is_equal(s21_decimal, s21_decimal);      // Равно debug ++
int s21_is_not_equal(s21_decimal, s21_decimal);  // Не равно debug ++
int s21_from_int_to_decimal(int src, s21_decimal *dst);  // Из int debug ++
int s21_from_float_to_decimal(float src,
                              s21_decimal *dst);         // Из float debug ++
int s21_from_decimal_to_int(s21_decimal src, int *dst);  // В int debug ++
int s21_from_decimal_to_float(s21_decimal src, float *dst);  // В float debug ++
int s21_floor(s21_decimal value,
              s21_decimal *result);  // Округляет указанное Decimal число до
                                     // ближайшего целого числа в сторону
                                     // отрицательной бесконечности.
int s21_round(s21_decimal value,
              s21_decimal *result);  // Округляет Decimal до ближайшего целого
                                     // числа. debug ++
int s21_truncate(s21_decimal value,
                 s21_decimal *result);  // Возвращает целые цифры указанного
                                        // Decimal числа; любые дробные цифры
                                        // отбрасываются, включая конечные нули.
int s21_negate(s21_decimal value,
               s21_decimal *result);  // Возвращает результат умножения
                                      // указанного Decimal на -1. ++

void print_dec(s21_decimal decl);                         //++
void print_float(float decl);                             // ++
void s21_set_bit(s21_decimal *decl, int index, int bit);  // ++
int s21_get_bit(s21_decimal decl, int index);             // ++
void s21_set_sign(s21_decimal *decl, int sign);           // ++
int s21_get_sign(s21_decimal decl);                       // ++
int s21_get_scale(s21_decimal decl);                      // ++
void s21_set_scale(s21_decimal *decl, int scale);         // ++
void s21_normalise(s21_decimal *decil);                   // ++
void s21_decl_to_null(s21_decimal *decl);                 // ++
void s21_right_shift(s21_decimal *decl);                  // ++
void s21_left_shift(s21_decimal *decl);                   //++
int s21_simple_add(s21_decimal var1, s21_decimal var2,
                   s21_decimal *res);  // simple summ ++
int s21_simple_sub(s21_decimal var1, s21_decimal var2,
                   s21_decimal *result);           // simple sub ++
void s21_copy(s21_decimal *dst, s21_decimal src);  // simple copy ++
int s21_simple_mul(s21_decimal value1, s21_decimal value2,
                   s21_decimal *result);  // sumple mul ++
int s21_get_high_bits(s21_decimal value1, s21_decimal value2, s21_decimal *result_of_search, s21_decimal *final_result, int prev_i);
int scale_equalize(s21_decimal *first, s21_decimal *second);
int bit_addition(s21_decimal *a, s21_decimal *b, s21_decimal *res);
void convert(s21_decimal *d);
void s21_scale_decrease(s21_decimal *value, int shift);
void s21_scale_increase(s21_decimal *value, int shift);
void left(s21_decimal *d, int g);
int last_bit(s21_decimal d);
int s21_is_zero(s21_decimal value);
int s21_is_one(s21_decimal value);
int s21_natural_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

#endif  // _SRC_S21_DECIMAL_H