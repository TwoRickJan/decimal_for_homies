#include "../s21_decimal.h"
#include <check.h>
#include <stdio.h>
#include <stdlib.h>

/*
    КАК ДОБАВЛЯТЬ ТЕСТЫ:

    1. Один тест = один кейс
    2. Не используйте test1, test2 — давайте осмысленные имена:
       test_add_positive
       test_add_negative
       test_add_overflow
    3. На каждый новый кейс — новый START_TEST
    4. Не забывайте добавлять новые тесты в свои подгруппы
*/

/*==============================================
    Вспомогательные функции для тестов
================================================*/

// Создание decimal из целого числа
s21_decimal s21_create_decimal(int value, int scale, int sign) {
    s21_decimal d = {0};
    d.bits[0] = abs(value);
    d.bits[3] = (sign << 31) | (scale << 16);
    return d;
}

// Проверка двух decimal на равенство
void assert_decimal_eq(s21_decimal a, s21_decimal b) {
    ck_assert_msg(a.bits[0] == b.bits[0] && 
                  a.bits[1] == b.bits[1] && 
                  a.bits[2] == b.bits[2] && 
                  a.bits[3] == b.bits[3],
                  "Decimal не равны: {0x%X, 0x%X, 0x%X, 0x%X} != {0x%X, 0x%X, 0x%X, 0x%X}",
                  a.bits[0], a.bits[1], a.bits[2], a.bits[3],
                  b.bits[0], b.bits[1], b.bits[2], b.bits[3]);
}

/*==============================================
    Тесты для сложения (s21_add)
================================================*/


START_TEST(test_add_max_plus_one_overflow) {
    // Максимальное число
    s21_decimal max = {0};
    max.bits[0] = 0xFFFFFFFFu;
    max.bits[1] = 0xFFFFFFFFu;
    max.bits[2] = 0xFFFFFFFFu;
    
    // Единица
    s21_decimal one = {0};
    one.bits[0] = 1u;
    
    s21_decimal result = {0};
    int code = s21_add(max, one, &result);
    
    // max + 1 = переполнение
    ck_assert_int_eq(code, S21_TOO_LARGE);
}
END_TEST

START_TEST(test_add_max_plus_zero) {
    // Максимальное число
    s21_decimal max = {0};
    max.bits[0] = 0xFFFFFFFFu;
    max.bits[1] = 0xFFFFFFFFu;
    max.bits[2] = 0xFFFFFFFFu;
    
    // Ноль
    s21_decimal zero = {0};
    
    s21_decimal result = {0};
    int code = s21_add(max, zero, &result);
    
    ck_assert_int_eq(code, S21_OK);
    ck_assert_uint_eq((unsigned int)result.bits[0], 0xFFFFFFFFu);
    ck_assert_uint_eq((unsigned int)result.bits[1], 0xFFFFFFFFu);
    ck_assert_uint_eq((unsigned int)result.bits[2], 0xFFFFFFFFu);
    ck_assert_uint_eq((unsigned int)result.bits[3], 0u);
}
END_TEST

START_TEST(test_add_min_plus_zero) {
    // Минимальное число (-79,228,162,514,264,337,593,543,950,335)
    s21_decimal min = {0};
    min.bits[0] = 0xFFFFFFFFu;
    min.bits[1] = 0xFFFFFFFFu;
    min.bits[2] = 0xFFFFFFFFu;
    min.bits[3] = (1u << 31);  // знак минус
    
    // Ноль
    s21_decimal zero = {0};
    
    s21_decimal result = {0};
    int code = s21_add(min, zero, &result);
    
    ck_assert_int_eq(code, S21_OK);
    ck_assert_uint_eq((unsigned int)result.bits[0], 0xFFFFFFFFu);
    ck_assert_uint_eq((unsigned int)result.bits[1], 0xFFFFFFFFu);
    ck_assert_uint_eq((unsigned int)result.bits[2], 0xFFFFFFFFu);
    ck_assert_uint_eq((unsigned int)result.bits[3], (1u << 31));
}
END_TEST

START_TEST(test_add_max_plus_max_overflow) {
    // Максимальное число
    s21_decimal max = {0};
    max.bits[0] = 0xFFFFFFFFu;
    max.bits[1] = 0xFFFFFFFFu;
    max.bits[2] = 0xFFFFFFFFu;
    
    s21_decimal result = {0};
    int code = s21_add(max, max, &result);
    
    ck_assert_int_eq(code, S21_TOO_LARGE);
}
END_TEST

START_TEST(test_add_min_plus_min_overflow) {
    // Минимальное число
    s21_decimal min = {0};
    min.bits[0] = 0xFFFFFFFFu;
    min.bits[1] = 0xFFFFFFFFu;
    min.bits[2] = 0xFFFFFFFFu;
    min.bits[3] = (1u << 31);
    
    s21_decimal result = {0};
    int code = s21_add(min, min, &result);
    
    ck_assert_int_eq(code, S21_TOO_SMALL);
}
END_TEST

START_TEST(test_add_min_minus_one_overflow) {
    // Минимальное число
    s21_decimal min = {0};
    min.bits[0] = 0xFFFFFFFFu;
    min.bits[1] = 0xFFFFFFFFu;
    min.bits[2] = 0xFFFFFFFFu;
    min.bits[3] = (1u << 31);
    
    // Минус единица
    s21_decimal minus_one = {0};
    minus_one.bits[0] = 1u;
    minus_one.bits[3] = (1u << 31);
    
    s21_decimal result = {0};
    int code = s21_add(min, minus_one, &result);
    
    ck_assert_int_eq(code, S21_TOO_SMALL);
}
END_TEST


/*==============================================
    Тесты для вычитания (s21_sub)
================================================*/
START_TEST(test_sub_max_minus_max) {
    // Создаём максимальное число 79,228,162,514,264,337,593,543,950,335
    s21_decimal max = {0};
    max.bits[0] = 0xFFFFFFFF;
    max.bits[1] = 0xFFFFFFFF;
    max.bits[2] = 0xFFFFFFF;
    
    
    s21_decimal result = {0};
    int code = s21_sub(max, max, &result);
    
    ck_assert_int_eq(code, S21_OK);
    // Проверяем, что результат равен 0
    ck_assert_int_eq(result.bits[0], 0);
    ck_assert_int_eq(result.bits[1], 0);
    ck_assert_int_eq(result.bits[2], 0);
}
END_TEST

/*==============================================
    Тесты для умножения (s21_mul)
================================================*/


/*==============================================
    Тесты для деления (s21_div)
================================================*/



/*==============================================
    Тесты для сравнения (s21_is_less, s21_is_equal и др.)
================================================*/
// s21_is_equal
START_TEST(test_is_equal_equal) {
    s21_decimal a = s21_create_decimal(12345, 2, 0);  // 123.45
    s21_decimal b = s21_create_decimal(12345, 2, 0);  // 123.45
    
    ck_assert_int_eq(s21_is_equal(a, b), S21_TRUE);
}
END_TEST

START_TEST(test_is_equal_different_scale) {
    s21_decimal a = s21_create_decimal(12345, 2, 0);  // 123.45
    s21_decimal b = s21_create_decimal(1234500, 4, 0); // 123.45
    
    ck_assert_int_eq(s21_is_equal(a, b), S21_TRUE);
}
END_TEST

START_TEST(test_is_equal_not_equal) {
    s21_decimal a = s21_create_decimal(100, 0, 0);    // 100
    s21_decimal b = s21_create_decimal(200, 0, 0);    // 200
    
    ck_assert_int_eq(s21_is_equal(a, b), S21_FALSE);
}
END_TEST

START_TEST(test_is_equal_negative) {
    s21_decimal a = s21_create_decimal(100, 0, 1);    // -100
    s21_decimal b = s21_create_decimal(100, 0, 1);    // -100
    
    ck_assert_int_eq(s21_is_equal(a, b), S21_TRUE);
}
END_TEST

START_TEST(test_is_equal_different_sign) {
    s21_decimal a = s21_create_decimal(100, 0, 0);    // 100
    s21_decimal b = s21_create_decimal(100, 0, 1);    // -100
    
    ck_assert_int_eq(s21_is_equal(a, b), S21_FALSE);
}
END_TEST




/*==============================================
    Тесты для преобразователей
================================================*/



/*==============================================
    Тесты для округления
================================================*/

/*==============================================
    Тесты для float преобразований
================================================*/



/*==============================================
    Тесты для граничных значений
================================================*/



/*==============================================
    Создание набора тестов
================================================*/

Suite *decimal_suite(void) {
    Suite *s = suite_create("decimal");

    // Арифметические операции
    TCase *add = tcase_create("add");
    /* Проверки с переполнениями */
    tcase_add_test(add, test_add_max_plus_zero);           // max + 0 = max
    tcase_add_test(add, test_add_min_plus_zero);           // min + 0 = min
    tcase_add_test(add, test_add_max_plus_max_overflow); // max + max
    tcase_add_test(add, test_add_max_plus_one_overflow); // max +1
    tcase_add_test(add, test_add_min_plus_min_overflow);   // min + min → переполнение (S21_TOO_SMALL)
    tcase_add_test(add, test_add_min_minus_one_overflow);  // min + (-1) → переполнение (S21_TOO_SMALL)
    
    TCase *sub = tcase_create("sub");
    tcase_add_test(sub, test_sub_max_minus_max);

    TCase *mul = tcase_create("mul");


    TCase *div = tcase_create("div");


    // Операции сравнения
    TCase *compare = tcase_create("compare");
    // s21_is_equal
    tcase_add_test(compare, test_is_equal_equal);
    tcase_add_test(compare, test_is_equal_different_scale);
    tcase_add_test(compare, test_is_equal_not_equal);
    tcase_add_test(compare, test_is_equal_negative);
    tcase_add_test(compare, test_is_equal_different_sign);
   //tcase_add_test(compare, test_compare_normalization_overflow);

/*     // s21_is_less
    tcase_add_test(compare, test_is_less_positive);
    tcase_add_test(compare, test_is_less_negative);
    tcase_add_test(compare, test_is_less_different_sign);
    tcase_add_test(compare, test_is_less_with_scale);

    // s21_is_greater
    tcase_add_test(compare, test_is_greater_positive);
    tcase_add_test(compare, test_is_greater_negative);

    // s21_is_less_or_equal
    tcase_add_test(compare, test_is_less_or_equal_less);
    tcase_add_test(compare, test_is_less_or_equal_equal);
    tcase_add_test(compare, test_is_less_or_equal_greater);

    // s21_is_greater_or_equal
    tcase_add_test(compare, test_is_greater_or_equal_greater);
    tcase_add_test(compare, test_is_greater_or_equal_equal);

    // s21_is_not_equal
    tcase_add_test(compare, test_is_not_equal_different);
    tcase_add_test(compare, test_is_not_equal_equal); */

    // Преобразователи
    TCase *converters = tcase_create("converters");

    // Округление и другие функции
    TCase *other = tcase_create("other");


    // Float преобразования
    TCase *float_conv = tcase_create("float_conv");


    // Граничные значения
    TCase *boundary = tcase_create("boundary");


    // Добавляем все подгруппы в suite
    suite_add_tcase(s, add);
    suite_add_tcase(s, sub);
    suite_add_tcase(s, mul);
    suite_add_tcase(s, div);
    suite_add_tcase(s, compare);
    suite_add_tcase(s, converters);
    suite_add_tcase(s, other);
    suite_add_tcase(s, float_conv);
    suite_add_tcase(s, boundary);

    return s;
}

int main(void) {
    Suite *s = decimal_suite();
    SRunner *sr = srunner_create(s);
    
    srunner_run_all(sr, CK_NORMAL);
    int failed = srunner_ntests_failed(sr);
    
    srunner_free(sr);
    
    return (failed == 0) ? 0 : 1;
}