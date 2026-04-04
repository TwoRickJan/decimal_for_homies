/*
 * test_arith.c — тесты арифметических операций
 * Файл реализации: s21_decimal_arith.c
 * Ответственный: Человек 3, 4
 * Функции: s21_add, s21_sub, s21_mul, s21_mod
 */

#include "../s21_decimal.h"
#include <check.h>

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

s21_decimal create_decimal(unsigned int low, unsigned int mid, unsigned int high, int scale, int sign) {
    s21_decimal d = {0};
    d.bits[0] = low;
    d.bits[1] = mid;
    d.bits[2] = high;
    s21_set_scale(&d, scale);
    s21_set_sign(&d, sign);
    return d;
}

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
    s21_decimal max = {0};
    max.bits[0] = 0xFFFFFFFFu;
    max.bits[1] = 0xFFFFFFFFu;
    max.bits[2] = 0xFFFFFFFFu;

    s21_decimal one = {0};
    one.bits[0] = 1u;

    s21_decimal result = {0};
    int code = s21_add(max, one, &result);

    ck_assert_int_eq(code, S21_TOO_LARGE);
}
END_TEST

START_TEST(test_add_max_plus_zero) {
    s21_decimal max = {0};
    max.bits[0] = 0xFFFFFFFFu;
    max.bits[1] = 0xFFFFFFFFu;
    max.bits[2] = 0xFFFFFFFFu;

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
    s21_decimal min = {0};
    min.bits[0] = 0xFFFFFFFFu;
    min.bits[1] = 0xFFFFFFFFu;
    min.bits[2] = 0xFFFFFFFFu;
    min.bits[3] = (1u << 31);

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
    s21_decimal min = {0};
    min.bits[0] = 0xFFFFFFFFu;
    min.bits[1] = 0xFFFFFFFFu;
    min.bits[2] = 0xFFFFFFFFu;
    min.bits[3] = (1u << 31);

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
    s21_decimal max = {0};
    max.bits[0] = 0xFFFFFFFF;
    max.bits[1] = 0xFFFFFFFF;
    max.bits[2] = 0xFFFFFFF;

    s21_decimal result = {0};
    int code = s21_sub(max, max, &result);

    ck_assert_int_eq(code, S21_OK);
    ck_assert_int_eq(result.bits[0], 0);
    ck_assert_int_eq(result.bits[1], 0);
    ck_assert_int_eq(result.bits[2], 0);
}
END_TEST
/*==============================================
    Тесты для деления (s21_div)
================================================*/

START_TEST(test_div_by_one) {
    s21_decimal a = {0};
    a.bits[0] = 12345;

    s21_decimal b = {0};
    b.bits[0] = 1;

    s21_decimal result = {0};
    int code = s21_div(a, b, &result);

    ck_assert_int_eq(code, S21_OK);
    ck_assert_uint_eq((unsigned int)result.bits[0], 12345);
    ck_assert_uint_eq((unsigned int)result.bits[1], 0);
    ck_assert_uint_eq((unsigned int)result.bits[2], 0);
}
END_TEST

/*==============================================
    Тесты для умножения (s21_mul)
================================================*/

/*==============================================
    Тесты для остатка от деления (s21_mod)
================================================*/

/*==============================================
    Создание набора тестов
================================================*/

Suite *test_arith_suite(void) {
    Suite *s = suite_create("arith");

    TCase *add = tcase_create("add");
    tcase_add_test(add, test_add_max_plus_one_overflow);
    tcase_add_test(add, test_add_max_plus_zero);
    tcase_add_test(add, test_add_min_plus_zero);
    tcase_add_test(add, test_add_max_plus_max_overflow);
    tcase_add_test(add, test_add_min_plus_min_overflow);
    tcase_add_test(add, test_add_min_minus_one_overflow);

    TCase *sub = tcase_create("sub");
    tcase_add_test(sub, test_sub_max_minus_max);

    TCase *mul = tcase_create("mul");

    TCase *div = tcase_create("div");
    tcase_add_test(div, test_div_by_one);

    TCase *mod = tcase_create("mod");

    suite_add_tcase(s, add);
    suite_add_tcase(s, sub);
    suite_add_tcase(s, mul);
    suite_add_tcase(s, div);
    suite_add_tcase(s, mod);

    return s;
}

int main(void) {
    Suite *s = test_arith_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    int failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (failed == 0) ? 0 : 1;
}
